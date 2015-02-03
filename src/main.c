#define __MAIN__
#include <navball.h>
#include <clock.h>

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
  s_clock_layer = layer_create(GRect(0, 0, size.w, size.h-size.w));

  Layer *navball_layer_layer = bitmap_layer_get_layer(s_canvas_layer);

  layer_add_child(window_layer, navball_layer_layer);
  layer_add_child(window_layer, s_clock_layer);

  // Set the update_proc
//  layer_set_update_proc(s_canvas_layer, navball_update_proc);
  clock_handle_init(s_clock_layer);
  navball_init(s_canvas_layer);
  APP_LOG(APP_LOG_LEVEL_INFO, "Done loading window: %p", window);
}

static void window_unload(Window *window) {
//  layer_destroy(s_canvas_layer);
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
}

static void deinit(void) {
  window_destroy(window);
  clock_handle_deinit();
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_INFO, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
