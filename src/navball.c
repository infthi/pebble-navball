#include <include/gui.h>

uint8_t begin[2], end[2];
uint8_t y_step;

//inline uint8_t abs(uint8_t x){
//    return (x>0)?x:-x;
//}

inline void hide_pixel(uint8_t x, uint8_t y){
  int offset = row_size*(y)+(x/8);
  uint8_t flag = 1<<(x%8);
  bitmap_data[offset] &= ~flag;
}

inline void put_pixel(uint8_t x, uint8_t y){
  int offset = row_size*(y)+(x/8);
  uint8_t flag = 1<<(x%8);
  bitmap_data[offset] |= flag;
}

void draw_line_pure(int16_t ix0, int16_t iy0, int16_t ix1, int16_t iy1, bool on){
  int16_t x0,y0,x1,y1;
  if (ix0<ix1){
    x0 = ix0;
    x1 = ix1;
    y0 = iy0;
    y1 = iy1;
  } else {
    x0 = ix1;
    x1 = ix0;
    y0 = iy1;
    y1 = iy0;
  }
  int16_t dx=x1-x0;
  int16_t dy=y1-y0;

  bool reverse = false;
  if (abs(dy)>abs(dx)){
    reverse = true;
    int16_t tmp;

    if (y0<y1) {
      tmp = x0;
      x0 = y0;
      y0 = tmp;

      tmp = x1;
      x1 = y1;
      y1 = tmp;
      
      tmp = dx;
      dx = dy;
      dy = tmp;
    } else {
      tmp = x0;
      x0 = y1;
      y1 = tmp;
    
      tmp = x1;
      x1 = y0;
      y0 = tmp;

      tmp = dx;
      dx = -dy;
      dy = -tmp;
    }

  }

  int16_t diff_y = (dy>0)?1:-1;
  dy*=diff_y;

  int16_t D = 2*dy - dx;
  if (on){
    reverse?put_pixel(y0,x0):put_pixel(x0,y0);
  } else {
    reverse?hide_pixel(y0,x0):hide_pixel(x0,y0);
  }
  int16_t x, y=y0;
  for (x = x0+1; x<=x1; x++){
    if (D > 0){
      y +=diff_y;
      D = D + (2*dy-2*dx);
    } else {
      D = D + (2*dy);
    }
    if (on){
      reverse?put_pixel(y,x):put_pixel(x,y);
    } else {
      reverse?hide_pixel(y,x):hide_pixel(x,y);
    }
  }
}

inline void draw_line_relative(int16_t ix1, int16_t iy1, int16_t ix2, int16_t iy2, bool on){
  draw_line_pure(ix1+72, iy1+72, ix2+72, iy2+72, on);
}

int16_t last_x = 0;
int16_t last_y = 0;

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
  for (y=71; y<73; y++){
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
    int offset = row_size*(69+y)+8;
    bitmap_data[offset++] |= level[y*2+1];//I still do not understand why they are mirrored. bit order?
    bitmap_data[offset++] |= level[y*2];
  }
}

void render_navball(int16_t x, int16_t y, int16_t z, float inv_sqrt){
//  float nxf = x*71.0f*inv_sqrt;
  int16_t nx = -x*71*inv_sqrt;
  int16_t ny = y*71*inv_sqrt;

//  int crt = inv_sqrt;
//  int rev = 1.0f/inv_sqrt;

//  APP_LOG(APP_LOG_LEVEL_INFO, "%d %d %d %d", nx, x, crt, rev);

  draw_line_relative(0,0,last_x,last_y, false);
  draw_line_relative(0,0,nx,ny, true);
  last_x = nx;
  last_y = ny;
  
  draw_level();
}