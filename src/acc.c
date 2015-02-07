#define __ACC__
#include <include/gui.h>
#include <include/acc.h>
#include <include/geometry.h>

int last_state = 0;

static void acceleration_state_draw(float acc){
  //acceleration can take values in [0..4] since it is what pebble SDK offers to us.
  if (acc > 4.0f){
    acc = 4.0f;
  } else if (acc<0.0f){
    acc = 0.0f;
  }
  
  //non-linear bar responcibility looks better
  acc = fsqrt(acc)*2; //and it is still in [0..4]
  
  int state = acc*70.0f/4.2f;

  int begin, end;
  bool fill_white;
  if (last_state<state){
    fill_white = true;
    begin = last_state;
    end = state;
  } else if (last_state>state){
    fill_white = false;
    begin = state;
    end = last_state;
  } else {
    return;
  }
  last_state = state;

  int line;
  uint8_t row_size = navball_bitmap->row_size_bytes;
  uint8_t delta = (row_size*8-navball_bitmap->bounds.size.w)/8;
//  APP_LOG(APP_LOG_LEVEL_INFO, "%d - %d : %s; %d", begin, end, fill_white?"white":"black", delta);
  
  for (line = begin; line<end; line++){
    int real_line = 144-line-1;
    int circle_line = line+1;
    int line_end = circle_144[circle_line];
    int line_offset = row_size*real_line;
//  APP_LOG(APP_LOG_LEVEL_INFO, "Line %d: %d - %d : %s", line, line_begin, line_end, fill_white?"white":"black");

//    line_begin = 8;

    int rem_right = line_end%8;
    int rem_left = 7-rem_right;
    if (fill_white){
      bitmap_data[line_offset+row_size-delta-(line_end/8)-1] |= remaining_right[rem_right];
      if (line_end>=8){
        bitmap_data[line_offset+row_size-delta-(line_end/8)] |= remaining_left[rem_left];
      }

    } else {
      bitmap_data[line_offset+row_size-delta-(line_end/8)-1] &= ~remaining_right[rem_right];
      if (line_end>=8){
        bitmap_data[line_offset+row_size-delta-(line_end/8)] &= ~remaining_left[rem_left];
      }
/*Fixing bar side is not required since it usually corrupts in lower part, and user will not see it at all*/
/*Code below is incorrect; it redraws battery bar part since this function is copypasted from power manager*/
/*      //and now fixing possibly corrupted bar side by redrawing it; code is cipypasted from navball.c
      uint8_t begin = circle_144[144-real_line];
      uint8_t end = circle_144[144-real_line-1];
      int k;
      if (begin==end)
        end++;
      for (k=begin; k<end; k++){
        draw_part(navball_bitmap->row_size_bytes, real_line, k-8);
      }*/
    }
  }
  refresh_gui();
}

void acc_handler(int16_t x, int16_t y, int16_t z, float inv_sqrt){
  static char s_buffer[16];

  float acc = 1.0f/(960.0f*inv_sqrt); //960 instead of to tune it to show 1G in stand-by mode for me
  float frac = acc;
  while (frac>1.0f){
    frac-=1.0f;
  }
  int acc_2 = frac*10;
  int acc_1 = acc-frac;

  snprintf(s_buffer, sizeof(s_buffer), "%d.%dG", acc_1, acc_2);
  
  //Show the data
  text_layer_set_text(acc_layer, s_buffer);
//  APP_LOG(APP_LOG_LEVEL_INFO, s_buffer);
  acceleration_state_draw(acc);
}

void init_acc(GSize size){
  acc_layer = text_layer_configure(GRect(72, size.h-18, 72, 18));
  text_layer_set_text(acc_layer, "1G");
  text_layer_set_text_alignment(acc_layer, GTextAlignmentRight);
//  acceleration_state_draw(4.2f);
}


void deinit_acc(){
  text_layer_destroy(acc_layer);
}