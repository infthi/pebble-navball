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


static const uint8_t remaining_right[] = {
    0b00000000,
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100,
    0b11111110
};

static const uint8_t remaining_left[] = {
    0b00000000,
    0b00000001,
    0b00000011,
    0b00000111,
    0b00001111,
    0b00011111,
    0b00111111,
    0b01111111
};

static const uint8_t pixels[] = {
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001
};

static const uint8_t chess[] = {
    0b10101010,
    0b01010101
};

float invSqrt(float x);

float fsqrt(float x);

#endif