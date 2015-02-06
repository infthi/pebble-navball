#define __ACC__
#include <navball.h>
#include <acc.h>
#include <geometry.h>

static void acceleration_state_draw(float state){
/*  int battery_state = percent/10;

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
  refresh();*/
}

void acc_handler(int16_t x, int16_t y, int16_t z){
  static char s_buffer[16];

  float acc = fsqrt((x*x+y*y+z*z)/1000000.0f);
  float frac = acc;
  while (frac>1.0f){
    frac-=1.0f;
  }
  int acc_2 = frac*10;
  int acc_1 = acc-frac;

  snprintf(s_buffer, sizeof(s_buffer), "%d.%dG", acc_1, acc_2);
  
  //Show the data
  text_layer_set_text(acc_layer, s_buffer);
  acceleration_state_draw(acc);
}

void init_acc(GSize size){
  acc_layer = text_layer_configure(GRect(72, size.h-18, 72, 18));
  text_layer_set_text(acc_layer, "1G");
  text_layer_set_text_alignment(acc_layer, GTextAlignmentRight);
}


void deinit_acc(){
  text_layer_destroy(acc_layer);
}