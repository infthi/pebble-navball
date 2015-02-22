#define __ACCSERVICE__
#include <pebble.h>
#include <include/gui.h>
#include <include/geometry.h>
#include <include/acc_service.h>
#include <include/sas.h>
#include <include/acc.h>
#include <include/navball.h>

#define NUM_SAMPLES 75
#define THRESHOLD_ALL 400
#define THRESHOLD_AXIS 200

#define POWER_SAVE_CALM_STEPS 3
#define XTREME_POWER_SAVING true

int16_t old_values[NUM_SAMPLES*3];
int current_idx = 0;
int samples_to_check = NUM_SAMPLES;
bool last_moving = true;

#if XTREME_POWER_SAVING
int16_t buffer[] = {0, 0, 0};
int8_t steps_to_pass = POWER_SAVE_CALM_STEPS;
#endif

int abs(int x){
  if (x<0)
    return -x;
  return x;
}


void set_sample_mode(bool moving){
  if (moving==last_moving){
    return;
  }
  last_moving = moving;
//  APP_LOG(APP_LOG_LEVEL_INFO, "ACC DATA: moving: %d %d", moving, rate);


  if (!moving){
    accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
    samples_to_check = NUM_SAMPLES*10/25;

#if XTREME_POWER_SAVING
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    steps_to_pass = POWER_SAVE_CALM_STEPS;
    samples_to_check /= POWER_SAVE_CALM_STEPS;
#endif
  } else {
    accel_service_set_sampling_rate(ACCEL_SAMPLING_25HZ);
    samples_to_check = NUM_SAMPLES;
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Accelerometer power-saving: %s", moving?"OFF":"ON");
}

int16_t prev_values[] = {0,0,0,0,0,0};


//Simple data filter: current value=(current_value+old_value/2_old_old_Value/2)/2
void filter(AccelData data){
  int16_t x = data.x;
  int16_t y = data.y;
  int16_t z = data.z;

  data.x = (data.x*2+prev_values[0]+prev_values[3])/4;
  data.y = (data.y*2+prev_values[1]+prev_values[4])/4;
  data.z = (data.z*2+prev_values[2]+prev_values[5])/4;

  int idx;
  for (idx=0; idx<3; idx++){
    prev_values[idx] = prev_values[idx+3];
  }
  prev_values[3] = x;
  prev_values[4] = y;
  prev_values[5] = z;
}

bool compare_to_old_values(AccelData data){
  old_values[current_idx*3] = data.x;
  old_values[current_idx*3+1] = data.y;
  old_values[current_idx*3+2] = data.z;

  int check_idx;
  int max_diff = 0;
  int max_axis_diff = 0;
  for (check_idx = 1; check_idx < samples_to_check; check_idx++){
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
  bool moving = (max_diff>THRESHOLD_ALL)||(max_axis_diff>THRESHOLD_AXIS);
  set_sample_mode(moving);
  
//  APP_LOG(APP_LOG_LEVEL_INFO, "ACC DATA: %d :: %d %d %d", max_diff, data.x, data.y, data.z);

  current_idx++;
  if (current_idx==NUM_SAMPLES){
    current_idx=0;
  }
  return moving;
};

static void data_handler(AccelData *data, uint32_t num_samples) {
#if XTREME_POWER_SAVING
  if (!last_moving){
    //we are in power-saving mode
    buffer[0] += data[0].x/POWER_SAVE_CALM_STEPS;
    buffer[1] += data[0].y/POWER_SAVE_CALM_STEPS;
    buffer[2] += data[0].z/POWER_SAVE_CALM_STEPS;
    if (0 == (--steps_to_pass)){
      data[0].x = buffer[0];
      data[0].y = buffer[1];
      data[0].z = buffer[2];
      steps_to_pass = POWER_SAVE_CALM_STEPS;
      buffer[0] = 0;
      buffer[1] = 0;
      buffer[2] = 0;
    } else {
      return;
    }
  }
#endif
  filter(data[0]);
  bool moving = compare_to_old_values(data[0]);
  int32_t x = data[0].x;
  int32_t y = data[0].y;
  int32_t z = data[0].z;
  float sqr_sum = x*x+y*y+z*z;
  float inv_sqrt = invSqrt(sqr_sum);

  update_sas(!moving);
  acc_handler(data[0].x, data[0].y, data[0].z, inv_sqrt);
  render_navball(data[0].x, data[0].y, data[0].z, inv_sqrt);
  refresh_gui();
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