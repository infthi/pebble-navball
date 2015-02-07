#include <pebble.h>
#include <include/geometry.h>

#ifndef __GUI__
#define __GUI__ extern
#endif

#ifndef __MAIN__
#define __MAIN__ extern
#endif

__GUI__ BitmapLayer *s_canvas_layer;
__GUI__ GBitmap *navball_bitmap;
__GUI__ uint8_t *bitmap_data;
__GUI__ uint8_t row_size;

__GUI__ void init_gui(BitmapLayer *layer);
__GUI__ void deinit_gui();
__GUI__ void refresh_gui();
__GUI__ void draw_circle_part(int i, int k);

__GUI__ TextLayer* text_layer_configure(GRect rect);

