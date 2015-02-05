#define __BATTERY__
#include <navball.h>
#include <battery.h>

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

  begin*=6;
  end*=6;

  int line;
//  APP_LOG(APP_LOG_LEVEL_INFO, "%d - %d : %s", begin, end, fill_white?"white":"black");
  for (line = begin; line<end; line++){
    int real_line = 144-line-1;
    int circle_line = line+1;
    int line_end = circle_144[circle_line];
    int line_offset = navball_bitmap->row_size_bytes*real_line;
//  APP_LOG(APP_LOG_LEVEL_INFO, "Line %d: %d - %d : %s", line, line_begin, line_end, fill_white?"white":"black");

//    line_begin = 8;

    int rem_right = line_end%8;
    int rem_left = 7-rem_right;
    if (fill_white){
      bitmap_data[line_offset+(line_end/8)] |= remaining_left[rem_right];
      if (line_end>=8){
        bitmap_data[line_offset+(line_end/8)-1] |= remaining_right[rem_left];
      }

    } else {
      bitmap_data[line_offset+(line_end/8)] &= ~remaining_left[rem_right];
      if (line_end>=8){
        bitmap_data[line_offset+(line_end/8)-1] &= ~remaining_right[rem_left];
      }

      //and now fixing possibly corrupted bar side by redrawing it; code is cipypasted from navball.c
      uint8_t begin = circle_144[144-real_line];
      uint8_t end = circle_144[144-real_line-1];
      int k;
      if (begin==end)
        end++;
      for (k=begin; k<end; k++){
        draw_part(navball_bitmap->row_size_bytes, real_line, k-8);
      }
    }
//    bitmap_data[line_offset] = fill_white?0xff:0x00;
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
  //any way the old timer must be removed
  if (charge_anim_timer!=NULL){
    app_timer_cancel(charge_anim_timer);
    charge_anim_timer = NULL;
  }
  if (on){
    last_animation_state = 0;
    charge_timer_callback();
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