#include "clock.h"

//TextLayer *text_date_layer;
TextLayer *text_time_layer;

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
//  static char date_text[] = "Xxxxxxxxx 00";

  char *time_format;

  if (!tick_time) {
    time_t now = time(NULL);
    tick_time = localtime(&now);
  }

  // TODO: Only update the date when it's changed.
//  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
//  text_layer_set_text(text_date_layer, date_text);


  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_time_layer, time_text);
  APP_LOG(APP_LOG_LEVEL_INFO, "Displaying: %s", time_text);
}

void clock_handle_deinit(void) {
  tick_timer_service_unsubscribe();
}

void clock_handle_init(Layer *parent_layer) {
//  text_date_layer = text_layer_create(GRect(8, 68, 144-8, 168-68));
//  text_layer_set_text_color(text_date_layer, GColorWhite);
//  text_layer_set_background_color(text_date_layer, GColorClear);
//  text_layer_set_font(text_date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
//  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  text_time_layer = text_layer_create(GRect(0, 0, 144, layer_get_bounds(parent_layer).size.h));
  text_layer_set_text_color(text_time_layer, GColorClear);
  text_layer_set_background_color(text_time_layer, GColorBlack);
  text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  layer_add_child(parent_layer, text_layer_get_layer(text_time_layer));

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(NULL, MINUTE_UNIT);
}
