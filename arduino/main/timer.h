/*
   Thread that displays a timer, stop watch or the time on the led screen

   Use clock_start to start the clock.
   Use clock_stop to stop the clock.
   Use clock_setTimer to set the timer.

   All units in millis
*/

#ifndef TIMER_H
#define TIMER_H

namespace timer {
void checkUpdateDisplay();
}

#endif
