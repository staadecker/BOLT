#include <TimeLib.h>
#include <Time.h>

#ifndef CLOCK
#define CLOCK

#include <Thread.h>
#include "screen.h"
#include "controller.h"
#include "const.h"

Thread clock_thread;

unsigned long clock_startTime;
unsigned int clock_timer;

void clock_callbackChrono(){
  screen_display(String((millis() - clock_startTime)/10));
}

void clock_callbackTimer(){
  screen_display(String((millis() - clock_startTime + clock_timer)/10));
}

void clock_callbackTime(){
  screen_display(String(hourFormat12()));
}

void clock_start(byte mode){
  switch (mode){
    case CLOCK_MODE_CHRONO:
      clock_startTime = millis();
      clock_thread.onRun(clock_callbackChrono);
      break;
    case CLOCK_MODE_TIMER:
      clock_startTime = millis();
      clock_thread.onRun(clock_callbackTimer);
      break;
    case CLOCK_MODE_TIME:
      clock_thread.onRun(clock_callbackTime);
      break;
  }
  
  clock_thread.enabled = true;
}

void clock_setTimer(int timerValue){
  clock_timer = timerValue;
}

void clock_stop(){
  clock_thread.enabled = false;
}

void clock_setup(){
  clock_thread = Thread();
  clock_thread.setInterval(10);
  clock_thread.enabled = false;
  controller_add(clock_thread);
}

#endif
