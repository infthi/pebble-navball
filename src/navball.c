#define __NAVBALL__
#include <navball.h>

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d\n1 %d,%d,%d\n2 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z, 
    data[1].x, data[1].y, data[1].z, 
    data[2].x, data[2].y, data[2].z
  );

  //Show the data
//  text_layer_set_text(s_output_layer, s_buffer);
}


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

void navball_init(Layer *this_layer, GContext *ctx) {
  uint32_t num_samples = 3;
  accel_data_service_subscribe(num_samples, data_handler);

  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}