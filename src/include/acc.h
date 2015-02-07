#ifndef __ACC_H__
#define __ACC_H__
#ifndef __ACC__
#define __ACC__ extern
#endif

__ACC__ TextLayer *acc_layer;

__ACC__ void acc_handler(int16_t x, int16_t y, int16_t z);
__ACC__ void init_acc(GSize parent_size);
__ACC__ void deinit_acc();

#endif