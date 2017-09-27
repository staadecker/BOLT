#ifndef LED_H
#define LED_H

#include <Arduino.h>

const uint8_t NUMBER_OF_BITS = 16;

//Value to adjust for optimal output voltage
const uint8_t LED_VCC_PWM = 105;

//0 is always default value
const uint8_t LED_STATE_OFF = 0;
const uint8_t LED_STATE_ON = 1;
const uint8_t LED_STATE_FLASHING = 2;

void led_setup();
void led_shiftOut();
void led_setState(uint8_t led, uint8_t state);

#endif
