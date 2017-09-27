#include <Thread.h>
#include <ThreadController.h>
#include "led.h"
#include "flasher.h"


void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);

  //Thread setup
  flasher_setup();

  led_setState(0, LED_STATE_ON);
  led_setState(1, LED_STATE_FLASHING);
  led_setState(3, LED_STATE_ON);
  led_setState(5, LED_STATE_ON);
  led_setState(7, LED_STATE_ON);
  led_setState(9, LED_STATE_ON);
  led_setState(11, LED_STATE_ON);
  led_shiftOut();
}

void loop() {
  if (flasherThread.shouldRun()) {
    flasherThread.run();
  }
}






