#define __BATTERY__
#include <navball.h>

void init_battery(GSize size){
  pow_layer = text_layer_configure(GRect(0, size.h-18, 84, 18));
  text_layer_set_text(pow_layer, "100%");
}


void deinit_battery(){
  text_layer_destroy(pow_layer);
}