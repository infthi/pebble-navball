#include <include/gui.h>

uint8_t begin[2], end[2];
int8_t y_step;

void draw_line_pure(uint8_t ix1, uint8_t iy1, uint8_t ix2, uint8_t iy2){
//we draw everything left-to-right
    if (ix1>ix2){
	begin[0] = ix2;
	begin[1] = iy2;
	end[0] = ix1;
	end[1] = iy1;
    } else {
	begin[0] = ix1;
	begin[1] = iy1;
	end[0] = ix2;
	end[1] = iy2;
    }
    y_step = begin[1]<end[1]?1:-1;
}


void render_navball(){
}