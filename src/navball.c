#define __NAVBALL__
#include <navball.h>
#include <geometry.h>

static const GBitmap *navball_bitmap = NULL;
uint8_t *bitmap_data = NULL;

TextLayer *power_layer, *acc_layer, *sas_layer, *rcs_layer;
TextLayer *acc_data_layer;

static uint32_t durations[] = {100, 100, 100, 100, 100};


void draw_part(int row_size, int i, int k) {
  if ((k<0)||(k>=144)){
    return;
  }
  int offset = row_size*(i)+(k/8);
  uint8_t flag = 1<<(k%8);
  bitmap_data[offset] |= flag;
}


void refresh(){
//  layer_mark_dirty(bitmap_layer_get_layer(s_canvas_layer));
}

void update_rcs(bool on){
  int i,k;
  uint8_t filler = on?0xff:0;
  int row_size = navball_bitmap->row_size_bytes;
  for (i=1; i<72; i++){
    uint8_t offset = 0;
    uint8_t remaining = circle_144[i];
    while (remaining>0){
      if (remaining>=8){
	bitmap_data[row_size*(i)+offset] = filler;
        remaining -= 8;
        offset++;
      } else {
        if (on){
          bitmap_data[row_size*(i)+offset] |= remaining_left[remaining];
        } else {
          bitmap_data[row_size*(i)+offset] &= ~remaining_left[remaining];
        }
        remaining = 0;
      }
    }
  }
  text_layer_set_text_color(rcs_layer, on?GColorBlack:GColorWhite);
  layer_mark_dirty(bitmap_layer_get_layer(s_canvas_layer));
  if (on){
    vibes_double_pulse();
  } else {
    vibes_enqueue_custom_pattern((VibePattern){.durations=durations, .num_segments=5});
  }
}

void init_rcs(){
  update_rcs(bluetooth_connection_service_peek());
  bluetooth_connection_service_subscribe(update_rcs);
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
      draw_part(row_size, 144-i, 153-k);
//power bar
      draw_part(row_size, 144-i, k-10);
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

void navball_init(BitmapLayer *this_layer) {
  GSize size = layer_get_bounds(bitmap_layer_get_layer(this_layer)).size;
  navball_bitmap = gbitmap_create_blank(size);
  bitmap_data = (uint8_t*)navball_bitmap->addr;
  
  draw_interface();

  bitmap_layer_set_bitmap(this_layer, navball_bitmap);

  power_layer = text_layer_configure(GRect(0, size.h-18, 84, 18));
  acc_layer = text_layer_configure(GRect(72, size.h-18, 72, 18));
  rcs_layer = text_layer_configure(GRect(0, -4, 72, 18));
  sas_layer = text_layer_configure(GRect(72, -4, 72, 18));

  acc_data_layer = text_layer_configure(GRect(0, size.h-118, size.w, 50));

  text_layer_set_text(power_layer, "100%");
  text_layer_set_text(acc_layer, "1G");
  text_layer_set_text(rcs_layer, "RCS");
  text_layer_set_text(sas_layer, "SAS");
  text_layer_set_text_alignment(acc_layer, GTextAlignmentRight);
  text_layer_set_text_alignment(sas_layer, GTextAlignmentRight);

  init_rcs();

  uint32_t num_samples = 1;
//  accel_data_service_subscribe(num_samples, data_handler);
}