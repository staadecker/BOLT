#ifndef SCREEN
#define SCREEN
#include <Thread.h>

Thread timeThread;
boolean timeThreadRun = false;
unsigned long startClockTime;

void printToLCD(String message){
  Serial.println("Printing to screen : " + message);
}

void updateTimeDisplay(){
  Serial.println(startClockTime);
  while(true){
    delay(10);
    printToLCD(String((millis() - startClockTime)/10));
  }
}

void startClock(){
  startClockTime = millis();
  timeThreadRun = true;
  timeThread.run();
  
}

void endClock(){
  timeThreadRun = false;
}

void setupClockThread(){
  timeThread = Thread();
  timeThread.setInterval(10);
  timeThread.onRun(updateTimeDisplay);
}

#endif
