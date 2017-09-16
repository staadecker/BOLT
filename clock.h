/*
   Thread that displays a timer, stop watch or the time on the led screen

   Use clock_start to start the clock.
   Use clock_stop to stop the clock.
   Use clock_setTimer to set the timer.

   All units in millis
*/

#ifndef CLOCK
#define CLOCK

const unsigned char CLOCK_MODE_CHRONO = 0;
const unsigned char CLOCK_MODE_TIMER = 1;
const unsigned char CLOCK_MODE_TIME = 2;

void clock_start(unsigned char mode);
void clock_stop();
void clock_setup();


#endif
