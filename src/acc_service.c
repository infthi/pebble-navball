#define __ACCSERVICE__
#include <pebble.h>
#include <acc_service.h>
#include <sas.h>

#define NUM_SAMPLES 50
#define THRESHOLD_ALL 400
#define THRESHOLD_AXIS 200

int16_t old_values[NUM_SAMPLES*3];
int current_idx = 0;

int abs(int x){
  if (x<0)
    return -x;
  return x;
}

bool last_moving = true;

void set_sample_mode(bool moving){
  if (moving==last_moving){
    return;
  }
//  APP_LOG(APP_LOG_LEVEL_INFO, "ACC DATA: moving: %d %d", moving, rate);
  update_sas(!moving);
  last_moving = moving;
}

void compare_to_old_values(AccelData data){
  old_values[current_idx*3] = data.x;
  old_values[current_idx*3+1] = data.y;
  old_values[current_idx*3+2] = data.z;

  int check_idx;
  int max_diff = 0;
  int max_axis_diff = 0;
  for (check_idx = 1; check_idx <NUM_SAMPLES; check_idx++){
    int real_idx = current_idx-check_idx;
    if (real_idx<0){
      real_idx = NUM_SAMPLES+real_idx;
    }
    int diff_x = abs(data.x-old_values[real_idx*3]);
    int diff_y = abs(data.y-old_values[real_idx*3+1]);
    int diff_z = abs(data.z-old_values[real_idx*3+2]);
    int diff = diff_x+diff_y+diff_z;
    
    if (diff>max_diff){
      max_diff = diff;
    }

    if (diff_x>max_axis_diff)
      max_axis_diff = diff_x;
    if (diff_y>max_axis_diff)
      max_axis_diff = diff_y;
    if (diff_z>max_axis_diff)
      max_axis_diff = diff_z;
    
  }
  set_sample_mode((max_diff>THRESHOLD_ALL)||(max_axis_diff>THRESHOLD_AXIS));
  
//  APP_LOG(APP_LOG_LEVEL_INFO, "ACC DATA: %d :: %d %d %d", max_diff, data.x, data.y, data.z);

  current_idx++;
  if (current_idx==NUM_SAMPLES){
    current_idx=0;
  }
};

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z
  );


  compare_to_old_values(data[0]);
  //Show the data
//  text_layer_set_text(acc_data_layer, s_buffer);
//  refresh();
}

void init_acc_service(){
  int idx = 0;
  for (idx=0; idx<150; idx++){
    old_values[idx] = 0;
  }
  uint32_t num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
}

void deinit_acc_service(){
  accel_data_service_unsubscribe();
}