/*
 * Thread that displays a timer, stop watch or the time on the led screen
 * 
 * Use clock_start to start the clock.
 * Use clock_stop to stop the clock.
 * Use clock_setTimer to set the timer.
 * 
 * All units in millis
 */
#include <TimeLib.h>
#include <Time.h>

#ifndef CLOCK
#define CLOCK

#include <Thread.h>
#include "screen.h"
#include "controller.h"
#include "const.h"

const byte CLOCK_MODE_CHRONO = 0;
const byte CLOCK_MODE_TIMER = 1;
const byte CLOCK_MODE_TIME = 2;

Thread clock_thread;

unsigned long clock_startTime;
unsigned int clock_timer = 30000;


void clock_callbackChrono(){
  screen_display(String((millis() - clock_startTime)/10));
}

void clock_callbackTimer(){
  //TODO do not display negative times.
  screen_display(String((millis() - clock_startTime + clock_timer)/10));
}

void clock_callbackTime(){
  screen_display(String(hourFormat12()));
}

//Start the clock in a specific mode
void clock_start(byte mode){
  switch (mode){
    case CLOCK_MODE_CHRONO:
      clock_thread.onRun(clock_callbackChrono);
      break;
    case CLOCK_MODE_TIMER:
      clock_thread.onRun(clock_callbackTimer);
      break;
    case CLOCK_MODE_TIME:
      clock_thread.onRun(clock_callbackTime);
      break;
  }

  clock_startTime = millis();
  clock_thread.enabled = true;
}

//Stop the clock
void clock_stop(){
  clock_thread.enabled = false;
}

//Set the time in millis seconds for the timer
void clock_setTimer(int timerValue){
  clock_timer = timerValue;
}


//Setup the clock thread, enabled = false, add to controller
void clock_setup(){
  clock_thread = Thread();
  clock_thread.setInterval(10);
  clock_thread.enabled = false;
  controller_add(clock_thread);
}

#endif
