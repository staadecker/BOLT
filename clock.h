#ifndef CLOCK
#define CLOCK

#include <Thread.h>
#include "screen.h"
#include "controller.h"

Thread clock_thread;
unsigned long clock_startTime;

void clock_callback(){
  screen_display(String((millis() - clock_startTime)/10));
}

//Restarts the clock at zero
void clock_reset(){
  clock_startTime = millis();
}

void clock_start(){
  clock_thread.enabled = true;
}

void clock_stop(){
  clock_thread.enabled = false;
}

void clock_setup(){
  clock_thread = Thread();
  clock_thread.setInterval(10);
  clock_thread.onRun(clock_callback);
  clock_thread.enabled = false;
  controller_add(clock_thread);
}

#endif
