#include <pebble.h>

#ifndef __NAVBALL__
#define __NAVBALL__ extern
#endif

#ifndef __MAIN__
#define __MAIN__ extern
#endif

//__MAIN__  Window *window;
__NAVBALL__ BitmapLayer *s_canvas_layer;

__NAVBALL__ void navball_init(BitmapLayer *layer);
//__NAVBALL__ void navball_update_proc(BitmapLayer *this_layer, GContext *ctx);