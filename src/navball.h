#include <pebble.h>

#ifndef __NAVBALL__
#define __NAVBALL__ extern
#endif

#ifndef __MAIN__
#define __MAIN__ extern
#endif

__MAIN__  Window *window;
__MAIN__ Layer *s_canvas_layer;

__NAVBALL__ void canvas_update_proc(Layer *this_layer, GContext *ctx);