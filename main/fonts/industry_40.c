/*******************************************************************************
 * Size: 40 px
 * Bpp: 1
 * Opts: --bpp 1 --size 40 --no-compress --font Industry-Black.ttf --symbols °CF --format lvgl -o industry_40.c
 ******************************************************************************/

#include "lvgl.h"

#ifndef INDUSTRY_40
#define INDUSTRY_40 1
#endif

#if INDUSTRY_40

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0043 "C" */
    0x1f, 0xff, 0x83, 0xff, 0xfc, 0x7f, 0xff, 0xef,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0x3, 0xff, 0xc0, 0x3f, 0xfc, 0x3, 0xff, 0xc0,
    0x3f, 0xfc, 0x3, 0xff, 0xc0, 0x0, 0xfc, 0x0,
    0xf, 0xc0, 0x0, 0xfc, 0x0, 0xf, 0xc0, 0x0,
    0xfc, 0x0, 0xf, 0xc0, 0x0, 0xfc, 0x3, 0xff,
    0xc0, 0x3f, 0xfc, 0x3, 0xff, 0xc0, 0x3f, 0xfc,
    0x3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x7f, 0xff, 0xe3, 0xff, 0xfc, 0x1f, 0xff,
    0x80,

    /* U+0046 "F" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0,
    0x7, 0xe0, 0x0, 0xfc, 0x0, 0x1f, 0x80, 0x3,
    0xf0, 0x0, 0x7f, 0xff, 0xcf, 0xff, 0xf9, 0xff,
    0xff, 0x3f, 0xff, 0xe7, 0xff, 0xfc, 0xff, 0xff,
    0x9f, 0x80, 0x3, 0xf0, 0x0, 0x7e, 0x0, 0xf,
    0xc0, 0x1, 0xf8, 0x0, 0x3f, 0x0, 0x7, 0xe0,
    0x0, 0xfc, 0x0, 0x1f, 0x80, 0x3, 0xf0, 0x0,
    0x7e, 0x0, 0xf, 0xc0, 0x0,

    /* U+00B0 "°" */
    0x7f, 0x7f, 0xf8, 0xfc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xff, 0x7f, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 397, .box_w = 20, .box_h = 29, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 73, .adv_w = 372, .box_w = 19, .box_h = 29, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 204, .box_w = 9, .box_h = 9, .ofs_x = 2, .ofs_y = 21}
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
    -5
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
const lv_font_t industry_40 = {
#else
lv_font_t industry_40 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 30,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -7,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if INDUSTRY_40*/

