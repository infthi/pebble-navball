#include <pebble.h>
#include <include/geometry.h>

float invSqrt(float x)
{
        float xhalf = 0.5f*x;
        union
        {
            float x;
            int i;
        } u;
        u.x = x;
        u.i = 0x5f3759df - (u.i >> 1);
        /* The next line can be repeated any number of times to increase accuracy */
        u.x = u.x * (1.5f - xhalf * u.x * u.x);
        return u.x;
}

float fsqrt(float x){
  return 1.0f/invSqrt(x);
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
}