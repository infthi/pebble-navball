#define __NAVBALL__
#include <navball.h>

static const GBitmap *navball_bitmap = NULL;
uint8_t* bitmap_data = NULL;

TextLayer *power_layer, *acc_layer, *acc_data_layer;

void refresh(){
  int ri  = 0;
  char b;

  int i = 0;
  for (i=0; i<navball_bitmap->row_size_bytes*144;i++){
    ri = rand();
    b = ri;
    bitmap_data[i] = b;
  }
  layer_mark_dirty(bitmap_layer_get_layer(s_canvas_layer));
}


static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z
  );

  //Show the data
  text_layer_set_text(acc_data_layer, s_buffer);
  refresh();
}


static void draw_interface(BitmapLayer *this_layer, GContext *ctx) {
/*  GRect bounds = layer_get_bounds(this_layer);


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
*/
}

static bool interface_required = true;

/*void navball_update_proc(BitmapLayer *this_layer, GContext *ctx) {
    if (interface_required){
	interface_required = false;
	draw_interface(this_layer, ctx);
    }
}*/

TextLayer *text_layer_configure(GRect rect){
  TextLayer *layer = text_layer_create(rect);
  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  
  Layer *parent = bitmap_layer_get_layer(s_canvas_layer);
  layer_add_child(parent, text_layer_get_layer(layer));

  return layer;
}

void navball_init(BitmapLayer *this_layer) {
  GSize size = layer_get_bounds(bitmap_layer_get_layer(this_layer)).size;
  navball_bitmap = gbitmap_create_blank(size);
  bitmap_data = (uint8_t*)navball_bitmap->addr;
  bitmap_layer_set_bitmap(this_layer, navball_bitmap);

  power_layer = text_layer_configure(GRect(0, size.h-21, 84, 21));
  acc_layer = text_layer_configure(GRect(72, size.h-21, 72, 21));
  acc_data_layer = text_layer_configure(GRect(0, size.h-118, size.w, 50));

  uint32_t num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
  text_layer_set_text(power_layer, "50%");
  text_layer_set_text(acc_layer, "1G");
  text_layer_set_text_alignment(acc_layer, GTextAlignmentRight);
}