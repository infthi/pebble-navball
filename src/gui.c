#define __GUI__
#include <include/gui.h>
#include <include/rcs.h>
#include <include/sas.h>
#include <include/battery.h>
#include <include/acc.h>
#include <include/navball.h>
#include <include/acc_service.h>


TextLayer *acc_data_layer;

void draw_circle_part(int i, int k) {
  if ((k<0)||(k>=144)){
    return;
  }
  int offset = row_size*(i)+(k/8);
  uint8_t flag = 1<<(k%8);
  bitmap_data[offset] |= flag;
}


void refresh_gui(){
  layer_mark_dirty(bitmap_layer_get_layer(s_canvas_layer));
}

static void draw_interface() {
  int i,k;
  for (i=1; i<72; i++){
    uint8_t begin = circle_144[i];
    uint8_t end = circle_144[i-1];
    if (begin==end)
      end++;
    for (k=begin; k<end; k++){
//top left
      draw_circle_part(i, k);
//top right
      draw_circle_part(i, 143-k);
//bottom right
      draw_circle_part(143-i, 143-k);
//bottom left
      draw_circle_part(143-i, k);
//G bar
      draw_circle_part(143-i, 151-k);
//power bar
      draw_circle_part(143-i, k-8);
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

void init_gui(BitmapLayer *this_layer) {
  GSize size = layer_get_bounds(bitmap_layer_get_layer(this_layer)).size;
  navball_bitmap = gbitmap_create_blank(size);
  bitmap_data = (uint8_t*)navball_bitmap->addr;
  row_size = navball_bitmap->row_size_bytes;
  
  draw_interface();

  bitmap_layer_set_bitmap(this_layer, navball_bitmap);

  acc_data_layer = text_layer_configure(GRect(0, size.h-118, size.w, 50));

  init_rcs();
  init_sas();
  init_battery(size);
  init_acc(size);
  init_navball();

  init_acc_service();
}

void deinit_gui(){
  deinit_acc_service();
  deinit_rcs();
  deinit_sas();
  deinit_battery();
  deinit_acc();

  text_layer_destroy(acc_data_layer);

  bitmap_layer_destroy(s_canvas_layer);
  gbitmap_destroy(navball_bitmap);
}