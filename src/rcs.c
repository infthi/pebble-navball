#define __RCS__
#include <include/gui.h>
#include <include/rcs.h>

static uint32_t durations[] = {100, 100, 100, 100, 100};

bool initial_setup = true;

void update_rcs(bool on){
  int i;
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
  refresh_gui();

  if (initial_setup){
    //to prevent labs on wf loading
    initial_setup = false;
  } else {
    if (on){
      vibes_double_pulse();
    } else {
      vibes_enqueue_custom_pattern((VibePattern){.durations=durations, .num_segments=5});
    }
  }
}

void init_rcs(){
  rcs_layer = text_layer_configure(GRect(0, -4, 72, 18));
  text_layer_set_text(rcs_layer, "RCS");
  update_rcs(bluetooth_connection_service_peek());
  bluetooth_connection_service_subscribe(update_rcs);
}

void deinit_rcs(){
  bluetooth_connection_service_unsubscribe();
  text_layer_destroy(rcs_layer);
}