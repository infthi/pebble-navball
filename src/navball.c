#include <include/gui.h>
#include <include/geometry.h>

#define REAL_BALL_SIZE 71

uint8_t begin[2], end[2];
uint8_t y_step;

//inline uint8_t abs(uint8_t x){
//    return (x>0)?x:-x;
//}

inline void put_pixel(uint8_t x, uint8_t y){
  int offset = row_size*(y)+(x/8);
  uint8_t flag = 1<<(x%8);
  bitmap_data[offset] |= flag;
}

void draw_line_pure(int16_t ix0, int16_t iy0, int16_t ix1, int16_t iy1){
  int16_t x0,y0,x1;//,y1;
  int16_t dx=ix1-ix0;
  int16_t dy=iy1-iy0;

  bool reverse = false;
  if (abs(dy)>abs(dx)){
    //we must perform calculation along y axis; x and y are swapped for calculation and swapped back for rendering
    reverse = true;

    if (iy0<iy1) {
      x0 = iy0;
      y0 = ix0;

      x1 = iy1;
//      y1 = ix1;

      int16_t tmp = dx;
      dx = dy;
      dy = tmp;
    } else {
      x0 = iy1;
      y0 = ix1;

      x1 = iy0;
//      y1 = ix0;

      int16_t tmp = dx;
      dx = -dy;
      dy = -tmp;
    }
  } else {
    if (ix0<ix1){
      x0 = ix0;
      y0 = iy0;
      x1 = ix1;
//      y1 = iy1;
    } else {
      x0 = ix1;
      y0 = iy1;
      x1 = ix0;
//      y1 = iy0;

      dx = -dx;
      dy = -dy;
    }
  }

  int16_t diff_y = (dy>0)?1:-1;
  dy*=diff_y;

  int16_t D = 2*dy - dx;
  
  reverse?put_pixel(y0,x0):put_pixel(x0,y0);
  
  int16_t x, y=y0;
  for (x = x0+1; x<=x1; x++){
    if (D > 0){
      y +=diff_y;
      D = D + (2*dy-2*dx);
    } else {
      D = D + (2*dy);
    }
    reverse?put_pixel(y,x):put_pixel(x,y);
  }
}

inline void draw_line_relative(int16_t ix1, int16_t iy1, int16_t ix2, int16_t iy2){
  draw_line_pure(ix1+72, iy1+72, ix2+72, iy2+72);
}

uint8_t level[] = {
0b00000001,0b10000000,
0b00000011,0b11000000,
0b10000011,0b11000001,
0b11000001,0b10000011,
0b11100000,0b00000111,
0b01110000,0b00001110,
0b00111000,0b00011100,
0b00011100,0b00111000,
0b00001110,0b01110000,
0b00000111,0b11100000,
0b00000001,0b10000000
};

void draw_level(){
  uint8_t y = 71;
  for (y=72; y<74; y++){
    int offset = row_size*y+5;
    bitmap_data[offset++] = 0xff;//40-
    bitmap_data[offset++] = 0xff;//   48-
    bitmap_data[offset++] = 0xff;//      64
    offset+=2;
    bitmap_data[offset++] = 0xff;//80-
    bitmap_data[offset++] = 0xff;//   96-
    bitmap_data[offset++] = 0xff;//      112
  }
  for (y=0; y<11; y++){
    int offset = row_size*(70+y)+8;
    bitmap_data[offset++] |= level[y*2+1];//I still do not understand why they are mirrored. bit order?
    bitmap_data[offset++] |= level[y*2];
  }
}

void chess_fill(uint8_t line, int16_t begin, int16_t end){
  uint8_t circle_line_idx;
  if (line>72){
    circle_line_idx = 143-line;
  } else {
    circle_line_idx = line;
  }
  if (begin<0)
  {
    begin = circle_144[circle_line_idx];
  }
  if (end<0)
  {
    end = 144-circle_144[circle_line_idx];
  }

  draw_line_pure(begin, line, end, line);
}


//minimal size which looks smooth enough
#define SIDE_SIZE_LARGE 10
#define SIDE_SIZE_FAST 3
int16_t side_points_large[SIDE_SIZE_LARGE*4];
int16_t pivot_koeff_large[SIDE_SIZE_LARGE];
int16_t side_points_fast[SIDE_SIZE_FAST*4];
int16_t pivot_koeff_fast[SIDE_SIZE_FAST];

void render_horizont(int16_t zenith_x, int16_t zenith_y, bool is_zenith_above){
/*
We see a navball's horizont as a half of circle's projection on a navball

The idea: we render a horizont not as a circle projection, but as a simplified polyline.
Two side point os horizont are already known: it's where navballs diameter perpendicular to 'center-zenith' vector crosses the navball border.
Third ('pivot') point is a center of horizont, and can be found on the line connecting center and zenith, PI/2 under the zenith.
No trigonometry; we'll make it one navball radius under the zenith.
*/
  int16_t *side_points, *pivot_koeff;
  int16_t SIDE_SIZE;

  int16_t z_vector_length = zenith_x*zenith_x+zenith_y*zenith_y;
  if (z_vector_length<2) {
    //zenith is almost in the center of navball; horizont invisible
    return;
  }
  int16_t norm_z_vector_length = REAL_BALL_SIZE*REAL_BALL_SIZE;
  float z_vector_koeff = fsqrt((float)norm_z_vector_length/(float)z_vector_length);

  if (z_vector_koeff<1.5f){
    SIDE_SIZE = SIDE_SIZE_FAST;
    side_points = side_points_fast;
    pivot_koeff = pivot_koeff_fast;
  } else {
    SIDE_SIZE = SIDE_SIZE_LARGE;
    side_points = side_points_large;
    pivot_koeff = pivot_koeff_large;
  }

  int16_t z_x_norm = zenith_x*z_vector_koeff;
  int16_t z_y_norm = zenith_y*z_vector_koeff;

  side_points[0] = z_y_norm;
  side_points[1] = -z_x_norm;
  side_points[SIDE_SIZE*4-2] = -z_y_norm;
  side_points[SIDE_SIZE*4-1] = z_x_norm;

  int16_t pivot_x;
  int16_t pivot_y;
  
  if (!is_zenith_above){
    //zenith is above the navball (and is visible), horizont one radius below the zenith
    pivot_x = zenith_x-z_x_norm;
    pivot_y = zenith_y-z_y_norm;
  } else {
    //zenith is below the navball (and is invisible)
//    int16_t remaining_x = z_x_norm;
//    int16_t remaining_y = z_y_norm;
    pivot_x = z_x_norm-zenith_x;
    pivot_y = z_y_norm-zenith_y;
  }

  uint8_t side_point_idx;
  for (side_point_idx=1; side_point_idx<SIDE_SIZE; side_point_idx++){
    int side_koeff = SIDE_SIZE-side_point_idx;
    side_points[side_point_idx*2] = side_points[0]*side_koeff/SIDE_SIZE+pivot_x*pivot_koeff[side_point_idx]/100;
    side_points[side_point_idx*2+1] = side_points[1]*side_koeff/SIDE_SIZE+pivot_y*pivot_koeff[side_point_idx]/100;
    side_points[SIDE_SIZE*4-side_point_idx*2-2] = side_points[SIDE_SIZE*4-2]*side_koeff/SIDE_SIZE+pivot_x*pivot_koeff[side_point_idx]/100;
    side_points[SIDE_SIZE*4-side_point_idx*2-1] = side_points[SIDE_SIZE*4-1]*side_koeff/SIDE_SIZE+pivot_y*pivot_koeff[side_point_idx]/100;
//int16_t test_x = side_points[0]/2+pivot_x*4/5;
//int16_t test_y = side_points[1]/2+pivot_y*4/5;
  }

  int16_t highest_horizont_point = 72, lowest_horizont_point = -72;
  uint8_t point1_offset=0, point2_offset=0, point3_offset=0, point4_offset=0;

  uint8_t idx;
  for (idx=0; idx<SIDE_SIZE-1; idx++){
    point1_offset = idx*2;
    point2_offset = idx*2+2;
    point3_offset = SIDE_SIZE*4-4-idx*2+2;
    point4_offset = SIDE_SIZE*4-4-idx*2;

    draw_line_relative(side_points[point1_offset], side_points[point1_offset+1], side_points[point2_offset], side_points[point2_offset+1]);
    draw_line_relative(side_points[point3_offset], side_points[point3_offset+1], side_points[point4_offset], side_points[point4_offset+1]);

    highest_horizont_point = min(highest_horizont_point, side_points[point1_offset+1]);
    highest_horizont_point = min(highest_horizont_point, side_points[point3_offset+1]);
    lowest_horizont_point = max(lowest_horizont_point, side_points[point1_offset+1]);
    lowest_horizont_point = max(lowest_horizont_point, side_points[point3_offset+1]);
//    draw_line_relative(side_points[0], side_points[1], test_x, test_y, on);
  }
  point1_offset = idx*2+2;
  point3_offset = SIDE_SIZE*4-4-idx*2;
  
  draw_line_relative(side_points[point1_offset], side_points[point1_offset+1], pivot_x, pivot_y);
  draw_line_relative(side_points[point3_offset], side_points[point3_offset+1], pivot_x, pivot_y);
  
  highest_horizont_point = min(highest_horizont_point, side_points[point1_offset+1]);
  highest_horizont_point = min(highest_horizont_point, side_points[point3_offset+1]);
  lowest_horizont_point = max(lowest_horizont_point, side_points[point1_offset+1]);
  lowest_horizont_point = max(lowest_horizont_point, side_points[point3_offset+1]);
  highest_horizont_point = min(highest_horizont_point, pivot_y);
  lowest_horizont_point = max(lowest_horizont_point, pivot_y);

  if (zenith_y<72){
    lowest_horizont_point += 72;
    while(lowest_horizont_point<144){
      chess_fill(lowest_horizont_point, -1, -1);
      lowest_horizont_point++;
    }
  } else {
    highest_horizont_point += 72;
    while(highest_horizont_point>0){
      chess_fill(highest_horizont_point+72, -1, -1);
      highest_horizont_point--;
    }
  }
//  draw_line_relative(pivot_x, pivot_y, side_points[SIDE_SIZE*4-2], side_points[SIDE_SIZE*4-1], on);
}

int16_t last_x = 0;
int16_t last_y = 0;
bool last_zenith_up = false;

void init_navball(){
  int i;
  for (i=0; i<SIDE_SIZE_LARGE; i++){
    float projection = (float)(SIDE_SIZE_LARGE-i)/(float)SIDE_SIZE_LARGE;
    pivot_koeff_large[i] = 100.0*fsqrt(1.0-projection*projection);
  }
  for (i=0; i<SIDE_SIZE_FAST; i++){
    float projection = (float)(SIDE_SIZE_FAST-i)/(float)SIDE_SIZE_FAST;
    pivot_koeff_fast[i] = 100.0*fsqrt(1.0-projection*projection);
  }
}

void clean_up(){
  int y = 0, remaining, done;
  uint8_t delta = 2;
  uint8_t last_offset_x = circle_144[0];
  uint8_t prev_offset_x = last_offset_x;
  for (y = 1; y<72; y++){
    int offset_top = y*row_size;
    int offset_bottom = (143-y)*row_size;
    //offcet in pixels from circle border to circle middle line
    int offset_x = circle_144[y-1]; //same offset as previous line; we need smaller offset
    if (offset_x==circle_144[y]){
      offset_x++;
    }

    int mod = (offset_x)%8;

    if (mod!=0){
        uint8_t roffset = 8-mod;
	bitmap_data[offset_top+offset_x/8] &= ~remaining_right[roffset];
	bitmap_data[offset_top+row_size-delta-1-offset_x/8] &= ~remaining_left[roffset];
	bitmap_data[offset_bottom+offset_x/8] &= ~remaining_right[roffset];
	bitmap_data[offset_bottom+row_size-delta-1-offset_x/8] &= ~remaining_left[roffset];
	offset_x+=roffset;
    }
    while (offset_x<72) {
	bitmap_data[offset_top+offset_x/8] = 0;
	bitmap_data[offset_top+row_size-delta-1-offset_x/8] = 0;
	bitmap_data[offset_bottom+offset_x/8] = 0;
	bitmap_data[offset_bottom+row_size-delta-1-offset_x/8] = 0;
	offset_x+=8;
    }
/*

    remaining = 72-circle_144[y];
    done = 0;
    bitmap_data[offset_top+]

    while (remaining>=8){
	remaining-=8;
	
	bitmap_data[offset_top+(72-done)/8] = 0xff;
    }*/
  }
}

void render_navball(int16_t x, int16_t y, int16_t z, float inv_sqrt){
  clean_up();
  int16_t zenith_x = -x*REAL_BALL_SIZE*inv_sqrt;
  int16_t zenith_y = y*REAL_BALL_SIZE*inv_sqrt;

  //(re-)draw line connecting center and zenith
  {
//    render_horizont(last_x, last_y, last_zenith_up, false);
//    draw_line_relative(0,0,last_x,last_y, false);
    last_x = zenith_x;
    last_y = zenith_y;
    last_zenith_up = z<0;
    render_horizont(last_x, last_y, last_zenith_up);
//    draw_line_relative(0,0,last_x,last_y, true);
  }
  
  //finally, draw v-level
  draw_level();
}