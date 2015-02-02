#define __NAVBALL__
#include <navball.h>

static void draw_interface(Layer *this_layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(this_layer);


  // Get the center of the navball
  int radius = bounds.size.w / 2 - 1;
  GPoint center = GPoint(radius, (bounds.size.h-radius));
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, center, radius);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, center, radius-1);

  // Draw the 'stalk'
//  graphics_context_set_fill_color(ctx, GColorBlack);
//  graphics_fill_rect(ctx, GRect(32, 40, 5, 100), 0, GCornerNone);
}

static bool interface_required = true;

void navball_update_proc(Layer *this_layer, GContext *ctx) {
    if (interface_required){
//	interface_required = false;
	draw_interface(this_layer, ctx);
    }
}