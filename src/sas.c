#define __SAS__
#include <navball.h>
#include <sas.h>

void update_sas(bool on){
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
  text_layer_set_text_color(sas_layer, on?GColorBlack:GColorWhite);
  refresh();
}

void init_sas(){
  sas_layer = text_layer_configure(GRect(72, -4, 72, 18));
  text_layer_set_text(sas_layer, "SAS");
  text_layer_set_text_alignment(sas_layer, GTextAlignmentRight);
}

void deinit_sas(){
  text_layer_destroy(sas_layer);
}