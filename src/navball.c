#define __NAVBALL__
#include <navball.h>
#include <rcs.h>
#include <battery.h>
#include <acc_service.h>

//#define ACC_ENABLED

TextLayer *acc_layer, *sas_layer;
TextLayer *acc_data_layer;

void draw_part(int row_size, int i, int k) {
  if ((k<0)||(k>=144)){
    return;
  }
  int offset = row_size*(i)+(k/8);
  uint8_t flag = 1<<(k%8);
  bitmap_data[offset] |= flag;
}


void refresh(){
  layer_mark_dirty(bitmap_layer_get_layer(s_canvas_layer));
}

#ifdef ACC_ENABLED
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
#endif


static void draw_interface() {
  int i,k;
  int row_size = navball_bitmap->row_size_bytes;
  for (i=1; i<72; i++){
    uint8_t begin = circle_144[i];
    uint8_t end = circle_144[i-1];
    if (begin==end)
      end++;
    for (k=begin; k<end; k++){
//top left
      draw_part(row_size, i, k);
//top right
      draw_part(row_size, i, 143-k);
//bottom right
      draw_part(row_size, 144-i, 143-k);
//bottom left
      draw_part(row_size, 144-i, k);
//G bar
      draw_part(row_size, 144-i, 151-k);
//power bar
      draw_part(row_size, 144-i, k-8);
    }
  }
}

TextLayer *text_layer_configure(GRect rect){
  TextLayer *layer = text_layer_create(rect);
  text_layer_set_text_color(layer, GColorWhite);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  
  Layer *parent = bitmap_layer_get_layer(s_canvas_layer);
  layer_add_child(parent, text_layer_get_layer(layer));
  return layer;
}

void init_navball(BitmapLayer *this_layer) {
  GSize size = layer_get_bounds(bitmap_layer_get_layer(this_layer)).size;
  navball_bitmap = gbitmap_create_blank(size);
  bitmap_data = (uint8_t*)navball_bitmap->addr;
  
  draw_interface();

  bitmap_layer_set_bitmap(this_layer, navball_bitmap);

  acc_layer = text_layer_configure(GRect(72, size.h-18, 72, 18));
  sas_layer = text_layer_configure(GRect(72, -4, 72, 18));

  acc_data_layer = text_layer_configure(GRect(0, size.h-118, size.w, 50));

  text_layer_set_text(acc_layer, "1G");
  text_layer_set_text(sas_layer, "SAS");
  text_layer_set_text_alignment(acc_layer, GTextAlignmentRight);
  text_layer_set_text_alignment(sas_layer, GTextAlignmentRight);

  init_rcs();
  init_battery(size);

#ifdef ACC_ENABLED
  uint32_t num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
#endif
}

void deinit_navball(){
#ifdef ACC_ENABLED
  accel_data_service_unsubscribe();
#endif
  deinit_rcs();
  deinit_battery();

  text_layer_destroy(acc_data_layer);
  text_layer_destroy(acc_layer);
  text_layer_destroy(sas_layer);

  bitmap_layer_destroy(s_canvas_layer);
  gbitmap_destroy(navball_bitmap);
}