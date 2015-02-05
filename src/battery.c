#define __BATTERY__
#include <navball.h>

int last_battery_state = 0;

AppTimer *charge_anim_timer = NULL;

static void battery_state_draw(uint8_t percent){
  int battery_state = percent/10;

  int begin, end;
  bool fill_white;
  if (last_battery_state<battery_state){
    fill_white = true;
    begin = last_battery_state;
    end = battery_state;
  } else if (last_battery_state>battery_state){
    fill_white = false;
    begin = battery_state;
    end = last_battery_state;
  } else {
    return;
  }
  last_battery_state = battery_state;

  begin*=7;
  end*=7;

  int line;
//  APP_LOG(APP_LOG_LEVEL_INFO, "%d - %d : %s", begin, end, fill_white?"white":"black");
  for (line = begin; line<end; line++){
    int real_line = 144-1-line;
    int line_end = remaining_left[real_line];
    int line_begin = remaining_left[real_line]-10;
    int line_offset = navball_bitmap->row_size_bytes*real_line;
    bitmap_data[line_offset] = fill_white?0xff:0x00;
  }
  refresh();
}

uint32_t last_animation_state = 100;
void charge_timer_callback(){
  last_animation_state +=10;
  if (last_animation_state>100){
    last_animation_state = 0;
  }
  battery_state_draw(last_animation_state);
    charge_anim_timer = app_timer_register(500, (AppTimerCallback) charge_timer_callback, NULL);
}

static void charger_setup(bool on){
  if (on){
    last_animation_state = 0;
    charge_timer_callback();
  } else {
    if (charge_anim_timer!=NULL){
      app_timer_cancel(charge_anim_timer);
      charge_anim_timer = NULL;
    }
  }
}

static void battery_state_handler(BatteryChargeState charge){
  // Long lived buffer
  static char s_buffer[128];

  if (charge.is_charging){
    text_layer_set_text(pow_layer, "crg");
    charger_setup(true);
  } else {
    snprintf(s_buffer, sizeof(s_buffer),
      "%d%c", charge.charge_percent, '%');
    //Show the data
    text_layer_set_text(pow_layer, s_buffer);
    battery_state_draw(charge.charge_percent);
    charger_setup(false);
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