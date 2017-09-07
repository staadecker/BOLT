#ifndef CLOCK
#define CLOCK

#include <Thread.h>
#include "screen.h"

Thread clock_thread;
unsigned long clock_startTime;

void clock_callback(){
  screen_display(String((millis() - clock_startTime)/10));
}

//Restarts the clock at zero
void clock_reset(){
  clock_startTime = millis();
}

//Called to run the clock thread
void clock_run(){
  if(clock_thread.shouldRun()){
    clock_thread.run();
  }
}

void clock_setup(){
  clock_thread = Thread();
  clock_thread.setInterval(10);
  clock_thread.onRun(clock_callback);
}

#endif
