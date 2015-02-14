#define __SAS__
#include <include/gui.h>
#include <include/sas.h>

AppTimer *update_sas_timer = NULL;
bool on = true;

void update_sas_impl(){
  int i;
  uint8_t filler = on?0xff:0;
  int row_size = navball_bitmap->row_size_bytes;
  for (i=1; i<72; i++){
    uint8_t delta = (row_size*8-navball_bitmap->bounds.size.w);

    //uint8_t pixel_delta = (delta%8);

    uint8_t offset = row_size-delta/8-1;
    uint8_t remaining = circle_144[i];
    while (remaining>0){
      if (remaining>=8){
        bitmap_data[row_size*(i)+offset] = filler;
        remaining -= 8;
        offset--;
      } else {
        if (on){
          bitmap_data[row_size*(i)+offset] |= remaining_right[remaining];
        } else {
          bitmap_data[row_size*(i)+offset] &= ~remaining_right[remaining];
        }
        remaining = 0;
      }
    }
  }
  text_layer_set_text_color(sas_layer, on?GColorBlack:GColorWhite);
  update_sas_timer = NULL;
}

void update_sas(bool enabled){
  if (on==enabled){
    return;
  }
  on = enabled;
  if (update_sas_timer!=NULL){
    app_timer_cancel(update_sas_timer);
  }
  update_sas_timer = app_timer_register(500, (AppTimerCallback) update_sas_impl, NULL);
}

void init_sas(){
  sas_layer = text_layer_configure(GRect(72, -4, 72, 18));
  text_layer_set_text(sas_layer, "SAS");
  text_layer_set_text_alignment(sas_layer, GTextAlignmentRight);
}

void deinit_sas(){
  text_layer_destroy(sas_layer);
}