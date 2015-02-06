#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

static const uint8_t circle_144[] = {
72,64,57,53,50,47,44,42,40,38,
36,35,33,31,30,29,27,26,25,24,
23,22,21,20,19,18,17,16,15,15,
14,13,12,12,11,11,10, 9, 9, 8,
 8, 7, 7, 6, 6, 5, 5, 5, 4, 4,
 4, 3, 3, 3, 2, 2, 2, 2, 1, 1,
 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
 0, 0, 0,};


static const uint8_t remaining_right[]  = {0, 128, 192, 224, 240, 248, 252, 254};
static const uint8_t remaining_left[]   = {0,   1,   3,   7,  15,  31,  63, 127};

float invSqrt(float x);

float fsqrt(float x);

#endif