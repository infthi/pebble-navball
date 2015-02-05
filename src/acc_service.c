#define __ACCSERVICE__
#include <pebble.h>
#include <acc_service.h>

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z
  );

  //Show the data
//  text_layer_set_text(acc_data_layer, s_buffer);
//  refresh();
}

void init_acc_service(){
  uint32_t num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
}

void deinit_acc_service(){
  accel_data_service_unsubscribe();
}