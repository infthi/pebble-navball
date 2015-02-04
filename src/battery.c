#define __BATTERY__
#include <navball.h>

static void battery_state_draw(uint8_t percent){
}

static void battery_state_handler(BatteryChargeState charge){
  // Long lived buffer
  static char s_buffer[128];

  if (charge.is_charging){
    text_layer_set_text(pow_layer, "crg");
    battery_state_draw(50);
  } else {
    snprintf(s_buffer, sizeof(s_buffer),
      "%d%c", charge.charge_percent, '%');
    //Show the data
    text_layer_set_text(pow_layer, s_buffer);
    battery_state_draw(charge.charge_percent);
  }
}

void init_battery(GSize size){
  pow_layer = text_layer_configure(GRect(0, size.h-18, 84, 18));
  text_layer_set_text(pow_layer, "100%");
  battery_state_service_subscribe(battery_state_handler);
  battery_state_handler(battery_state_service_peek());
}


void deinit_battery(){
  battery_state_service_unsubscribe();
  text_layer_destroy(pow_layer);
}