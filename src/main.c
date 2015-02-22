#define __MAIN__
#include <include/gui.h>
#include <include/clock.h>

#define PRESENTATION_MODE false

Window *window;
Layer *s_clock_layer;

static void window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Loading window: %p", window);
  window_set_background_color(window, GColorBlack);
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GSize size = bounds.size;

  // Create Layer
  s_canvas_layer = bitmap_layer_create(GRect(0, size.h-size.w, size.w, size.w));
  s_clock_layer = layer_create(GRect(0, -7, size.w, 8+size.h-size.w));

  Layer *navball_layer_layer = bitmap_layer_get_layer(s_canvas_layer);

  layer_add_child(window_layer, s_clock_layer);
  layer_add_child(window_layer, navball_layer_layer);

  init_clock(s_clock_layer);
  init_gui(s_canvas_layer);
  APP_LOG(APP_LOG_LEVEL_INFO, "Done loading window: %p", window);
}

static void window_unload(Window *window) {
}

static void init(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Began init");
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  APP_LOG(APP_LOG_LEVEL_INFO, "Done loading window: %p", window);
  #if PRESENTATION_MODE
    light_enable(true);
  #endif
}

static void deinit(void) {
  deinit_gui();
  deinit_clock();
  layer_destroy(s_clock_layer);
  window_destroy(window);
  #if PRESENTATION_MODE
    light_enable(false);
  #endif
}

#include <math.h>
int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_INFO, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
