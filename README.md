# pebble-navball-watchface
KSP-style navball watchface for pebble smartwatch


In this watchface for pebble smartwatch I tried to replicate navball from Kerbal Space Program game. It may not look well, because I was limited by rectangular screen.

The watchface consists of 6 parts: digital clock on the top, round navball in the center, RCS indicator in top left, SAS indicator in top right, power indicator in bottom left and acceleration indicator in bottom right.

'RCS indicator' is used to indicate whenever the connection with your smartphone is active or not. Your watch will also notify you with vibration when smartphone connection is lost or reinstanciated.

'SAS indicator' (aka stability indicator) indicates if the watch was moving or not in the last 3 seconds. If the watch is almost not moving, the watchface enters power-saving mode, in whick it queries accelerometer less frequently.

'Power indicator' is just a battery level indicator.

'Acceleration indicator' indicates you current acceleration in Gs. Usually when you do not move, you are accelerated with 1G by Earth's gravity. Pebble API says it's accelerometer cannot handle more than 4G, so that is the limit. Also this indicator displays changes non-linearry, so it;s bar will be more sensitive near 1G and less sensitive in high-G enviroment.

This watchface can be built and installed by using 'make install' in the project directory. This may require you to set up Pebble SDK, to configure pebble app on your smartphone to accept developer connection, and to set up your phone's IP address in the makefile.
