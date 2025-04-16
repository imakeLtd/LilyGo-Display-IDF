/*******************************************************************************
 * Size: 50 px
 * Bpp: 1
 * Opts: --bpp 1 --size 50 --no-compress --font Industry-Black.ttf --symbols °CF --format lvgl -o industry_50.c
 ******************************************************************************/

#include "lvgl.h"

#ifndef INDUSTRY_50
#define INDUSTRY_50 1
#endif

#if INDUSTRY_50

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0043 "C" */
    0xf, 0xff, 0xfc, 0x7, 0xff, 0xff, 0x83, 0xff,
    0xff, 0xf1, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x0, 0x3f, 0xff, 0xc0, 0xf,
    0xff, 0xf0, 0x3, 0xff, 0xfc, 0x0, 0xff, 0xff,
    0x0, 0x3f, 0xff, 0xc0, 0x0, 0xf, 0xf0, 0x0,
    0x3, 0xfc, 0x0, 0x0, 0xff, 0x0, 0x0, 0x3f,
    0xc0, 0x0, 0xf, 0xf0, 0x0, 0x3, 0xfc, 0x0,
    0x0, 0xff, 0x0, 0x0, 0x3f, 0xc0, 0x0, 0xf,
    0xf0, 0x0, 0x3, 0xfc, 0x0, 0xff, 0xff, 0x0,
    0x3f, 0xff, 0xc0, 0xf, 0xff, 0xf0, 0x3, 0xff,
    0xfc, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x7f, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xc1, 0xff,
    0xff, 0xe0, 0x3f, 0xff, 0xf0,

    /* U+0046 "F" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff, 0x0,
    0x0, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff,
    0x0, 0x0, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8,
    0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff,
    0xf8, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf8, 0xff,
    0xff, 0xf8, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0,
    0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff, 0x0,
    0x0, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff,
    0x0, 0x0, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0,
    0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff, 0x0,
    0x0, 0xff, 0x0, 0x0,

    /* U+00B0 "°" */
    0x7f, 0xdf, 0xff, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xff, 0xef, 0xf8
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 496, .box_w = 26, .box_h = 36, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 117, .adv_w = 465, .box_w = 24, .box_h = 36, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 255, .box_w = 11, .box_h = 10, .ofs_x = 3, .ofs_y = 25}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x3, 0x6d
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 67, .range_length = 110, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    2, 1
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -6
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 1,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t industry_50 = {
#else
lv_font_t industry_50 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 36,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -9,
    .underline_thickness = 3,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if INDUSTRY_50*/

