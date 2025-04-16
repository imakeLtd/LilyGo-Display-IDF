/**
 * @file      main.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2024-01-07
 *
 */

#define LV_CONF_INCLUDE_SIMPLE 1
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "amoled_driver.h"
#include "touch_driver.h"
#include "i2c_driver.h"
#include "power_driver.h"
#include "demos/lv_demos.h"
#include "tft_driver.h"
#include "product_pins.h"
// #define LV_LVGL_H_INCLUDE_SIMPLE 1
// #include "fonts/industry_black_100.c"
// #include "fonts/industry_black_60.c"
// #include "fonts/Industry_bold_60.c"

static const char *TAG = "main";

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2

#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 1
#define EXAMPLE_LVGL_TASK_STACK_SIZE (4 * 1024)
#define EXAMPLE_LVGL_TASK_PRIORITY 2

#define USB_SYMBOL "\xEF\x8A\x87"
#define WIFI_SYMBOL "\xEF\x87\xAB"

static SemaphoreHandle_t lvgl_mux = NULL;

static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)

extern "C" {
    lv_disp_drv_t disp_drv;      // contains callback functions
}


static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
#if DISPLAY_FULLRESH
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
    display_push_colors(area->x1, area->y1, w, h, (uint16_t *)color_map);
    lv_disp_flush_ready( drv );
#else
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    display_push_colors(offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, (uint16_t *)color_map);
#endif
}


#if BOARD_HAS_TOUCH
static void example_lvgl_touch_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    int16_t touchpad_x[1] = {0};
    int16_t touchpad_y[1] = {0};
    uint8_t touchpad_cnt = 0;

    /* Get coordinates */
    touchpad_cnt = touch_get_data(touchpad_x, touchpad_y, 1);

    if (touchpad_cnt > 0) {
        data->point.x = touchpad_x[0];
        data->point.y = touchpad_y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
#endif

static void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

bool example_lvgl_lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void example_lvgl_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_mux);
}

static void example_lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    while (1) {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (example_lvgl_lock(-1)) {
            task_delay_ms = lv_timer_handler();
            // Release the mutex
            example_lvgl_unlock();
        }
        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}
extern "C" {
    void example_lvgl_demo_ui(lv_disp_t *disp);
}

static lv_obj_t *current_temp_label;
static lv_obj_t *current_temp_text_label;
static lv_obj_t *target_temp_label;
static lv_obj_t *target_temp_text_label;
static lv_obj_t *timer_label;
static lv_obj_t *timer_dash1;
static lv_obj_t *wifi_label;
static lv_obj_t *icon_label;
static lv_obj_t *current_unit_label;
static lv_obj_t *target_unit_label;
static lv_obj_t *heat_status_label;

static lv_style_t current_temp_style;
static lv_style_t subtitle_text_style;
static lv_style_t target_temp_style;
static lv_style_t fa_symbol_style;
static lv_style_t timer_style;
static lv_style_t unit_style;
static lv_style_t unit_white_style;
static lv_style_t style_bg;
static lv_style_t style_indic;
static lv_style_t heat_status_style;

static void set_temp(void * bar, int32_t level)
{
    lv_bar_set_value((lv_obj_t *)bar, level, LV_ANIM_ON);
    // ESP_LOGI(TAG, "Set bar value: %d", (int) level);
    // Update the label with the heat level e.g. "HEAT\n50%", "HEAT\n%d%" -> level
    char temp_str[20];
    snprintf(temp_str, sizeof(temp_str), "HEAT\n%d%%", (int)level);
    lv_label_set_text(heat_status_label, temp_str);
}

void ui_init() {
    lv_obj_t *screen = lv_scr_act();
    // lv_obj_set_style_bg_color(screen, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);

    lv_style_init(&current_temp_style);
    lv_style_set_text_color(&current_temp_style, lv_color_hex(0xFFFF00));
    lv_style_set_text_font(&current_temp_style, &industry_120);

    lv_style_init(&subtitle_text_style);
    lv_style_set_text_color(&subtitle_text_style, lv_color_hex(0xFFFFFF));
    lv_style_set_text_font(&subtitle_text_style, &lv_font_montserrat_24);

    lv_style_init(&target_temp_style);
    lv_style_set_text_color(&target_temp_style, lv_color_hex(0xFFFFFF));
    lv_style_set_text_font(&target_temp_style, &industry_90);

    lv_style_init(&fa_symbol_style);
    lv_style_set_text_color(&fa_symbol_style, lv_color_hex(0xFFFFFF));
    lv_style_set_text_font(&fa_symbol_style, &fa_symbol_40);

    lv_style_init(&timer_style);
    lv_style_set_text_color(&timer_style, lv_color_hex(0xFFFFFF));
    lv_style_set_text_font(&timer_style, &industry_90);

    lv_style_init(&unit_style);
    lv_style_set_text_color(&unit_style, lv_color_hex(0xFFFF00));
    lv_style_set_text_font(&unit_style, &industry_40);

    lv_style_init(&unit_white_style);
    lv_style_set_text_color(&unit_white_style, lv_color_hex(0xFFFFFF));
    lv_style_set_text_font(&unit_white_style, &industry_40);

    lv_style_init(&heat_status_style);
    lv_style_set_text_color(&heat_status_style, lv_color_hex(0xFF0000));
    lv_style_set_text_font(&heat_status_style, &lv_font_montserrat_14);

    lv_style_init(&style_bg);
    lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_border_width(&style_bg, 2);
    lv_style_set_pad_all(&style_bg, 6); /*To make the indicator smaller*/
    lv_style_set_radius(&style_bg, 6);
    // lv_style_set_anim_duration(&style_bg, 1000);

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_radius(&style_indic, 3);
    // lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    // lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    current_temp_label = lv_label_create(screen);
    lv_obj_add_style(current_temp_label, &current_temp_style, 0);
    lv_label_set_text(current_temp_label, "89.5");
    lv_obj_align(current_temp_label, LV_ALIGN_TOP_MID, -20, 1);

    current_unit_label = lv_label_create(screen);
    lv_obj_add_style(current_unit_label, &unit_style, 0);
    lv_label_set_text(current_unit_label, "°C");
    lv_obj_align(current_unit_label, LV_ALIGN_TOP_MID, 130, 2);

    current_temp_text_label = lv_label_create(screen);
    lv_obj_add_style(current_temp_text_label, &subtitle_text_style, 0);
    lv_label_set_text(current_temp_text_label, "CURRENT TEMP");
    lv_obj_align(current_temp_text_label, LV_ALIGN_TOP_MID, -20, 94);

    target_temp_label = lv_label_create(screen);
    lv_obj_add_style(target_temp_label, &target_temp_style, 0);
    lv_label_set_text(target_temp_label, "94.0");
    lv_obj_align(target_temp_label, LV_ALIGN_BOTTOM_LEFT, 10, -35);

    target_unit_label = lv_label_create(screen);
    lv_obj_add_style(target_unit_label, &unit_white_style, 0);
    lv_label_set_text(target_unit_label, "°C");
    lv_obj_align(target_unit_label, LV_ALIGN_BOTTOM_LEFT, 210, -70);

    target_temp_text_label = lv_label_create(screen);
    lv_obj_add_style(target_temp_text_label, &subtitle_text_style, 0);
    lv_label_set_text(target_temp_text_label, "TARGET TEMP");
    lv_obj_align(target_temp_text_label, LV_ALIGN_BOTTOM_LEFT, 22, 0);

    timer_label = lv_label_create(screen);
    lv_obj_add_style(timer_label, &timer_style, 0);
    lv_label_set_text(timer_label, "5:36");
    lv_obj_align(timer_label, LV_ALIGN_BOTTOM_RIGHT, -60, -35);

    timer_dash1 = lv_label_create(screen);
    lv_obj_add_style(timer_dash1, &subtitle_text_style, 0);
    lv_label_set_text(timer_dash1, "TIME LEFT");
    lv_obj_align(timer_dash1, LV_ALIGN_BOTTOM_RIGHT, -90, 0);

    wifi_label = lv_label_create(screen);
    lv_obj_add_style(wifi_label, &fa_symbol_style, 0);
    lv_label_set_text(wifi_label, WIFI_SYMBOL);
    lv_obj_align(wifi_label, LV_ALIGN_TOP_LEFT, 5, 5);

    heat_status_label = lv_label_create(screen);
    lv_obj_add_style(heat_status_label, &heat_status_style, 0);
    lv_label_set_text(heat_status_label, "HEAT\nPOWER");
    lv_obj_align(heat_status_label, LV_ALIGN_TOP_RIGHT, -3, 0);
    lv_obj_set_style_text_align(heat_status_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_obj_t * bar = lv_bar_create(screen);
    lv_obj_remove_style_all(bar);  /*To have a clean start*/
    lv_obj_add_style(bar, &style_bg, 0);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 40, 205);
    // lv_obj_center(bar);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_RIGHT, -2, 0);
    lv_bar_set_range(bar, 0, 100);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    // lv_anim_set_duration(&a, 3000);
    // lv_anim_set_reverse_duration(&a, 3000);
    lv_anim_set_time(&a, 5000); // Sets the total duration
    lv_anim_set_playback_time(&a, 5000); // Sets the reverse duration
    lv_anim_set_var(&a, bar);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

void ui_update() {
    static int current_temp = 80;
    static int time = 0;
    char temp_str[20];
    snprintf(temp_str, sizeof(temp_str), "%d.%d", current_temp, 5);
    lv_label_set_text(current_temp_label, temp_str);

    current_temp++;
    time++;
    if (current_temp > 98) {
        current_temp = 60;
    }
    if (time > 200) {
        time = 0;
    }
}

extern "C" void app_main(void)
{

    ESP_LOGI(TAG, "------ Initialize I2C.");
    i2c_driver_init();

    ESP_LOGI(TAG, "------ Initialize PMU.");
    if (!power_driver_init()) {
        ESP_LOGE(TAG, "ERROR :No find PMU ....");
    }

    ESP_LOGI(TAG, "------ Initialize TOUCH.");
    touch_init();

    ESP_LOGI(TAG, "------ Initialize DISPLAY.");
    display_init();


    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();


    // alloc draw buffers used by LVGL
    // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
#if CONFIG_SPIRAM

// #if CONFIG_LILYGO_T_RGB
//     // initialize LVGL draw buffers

//     extern void *buf1;
//     extern void *buf2;

//     lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISPLAY_BUFFER_SIZE);
// #else
    lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf1);

    lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf2);

    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISPLAY_BUFFER_SIZE);
// #endif
#else
    lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(AMOLED_HEIGHT * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = (lv_color_t *) heap_caps_malloc(AMOLED_HEIGHT * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, AMOLED_HEIGHT * 20);
#endif

    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = AMOLED_HEIGHT;
    disp_drv.ver_res = AMOLED_WIDTH;
    disp_drv.flush_cb = example_lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.full_refresh = DISPLAY_FULLRESH;
    lv_disp_drv_register(&disp_drv);

    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "lvgl_tick",
        .skip_unhandled_events = false
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));

#if BOARD_HAS_TOUCH
    ESP_LOGI(TAG, "Register touch driver to LVGL");
    static lv_indev_drv_t indev_drv; // Input device driver (Touch)
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = example_lvgl_touch_cb;
    lv_indev_drv_register(&indev_drv);
#endif

    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    assert(lvgl_mux);


    ESP_LOGI(TAG, "Display LVGL");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (example_lvgl_lock(-1)) {
        ESP_LOGI(TAG, "Initialize UI");
        ui_init();
        // Release the mutex
        example_lvgl_unlock();
    }
    // tskIDLE_PRIORITY,
    ESP_LOGI(TAG, "Create LVGL task");
    // xTaskCreate(example_lvgl_port_task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE, NULL, EXAMPLE_LVGL_TASK_PRIORITY, NULL);
    xTaskCreate(example_lvgl_port_task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);


    esp_timer_handle_t ui_timer;
    const esp_timer_create_args_t ui_timer_args = {
        .callback = (void (*)(void*))ui_update,
        .name = "ui_timer"
    };
    esp_timer_create(&ui_timer_args, &ui_timer);
    esp_timer_start_periodic(ui_timer, 1000000); // 1 second

}
