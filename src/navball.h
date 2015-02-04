#include <pebble.h>
#include <rcs.h>

#ifndef __NAVBALL__
#define __NAVBALL__ extern
#endif

#ifndef __MAIN__
#define __MAIN__ extern
#endif

__NAVBALL__ BitmapLayer *s_canvas_layer;
__NAVBALL__ GBitmap *navball_bitmap;
__NAVBALL__ uint8_t *bitmap_data;

//__MAIN__  Window *window;

__NAVBALL__ void init_navball(BitmapLayer *layer);
//__NAVBALL__ void navball_update_proc(BitmapLayer *this_layer, GContext *ctx);

