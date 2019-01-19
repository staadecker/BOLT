#include "led-manager.h"
#include "constants.h"
#include "logger.h"

LedManager::LedManager() {
    pinMode(P_LED_VCC, OUTPUT);
    pinMode(P_LED_DATA, OUTPUT);
    pinMode(P_LED_CLOCK, OUTPUT);
    pinMode(P_LED_LATCH, OUTPUT);
}

void LedManager::shiftOut() {
    //Latch Low. VCC high
    digitalWrite(P_LED_VCC, HIGH);
    digitalWrite(P_LED_LATCH, LOW);


    //Shift out
    for (uint8_t i = NUMBER_OF_LEDS; i <= NUMBER_OF_LEDS; i--) {
        digitalWrite(P_LED_DATA, states[i]);

        //Clock
        digitalWrite(P_LED_CLOCK, HIGH);
        digitalWrite(P_LED_CLOCK, LOW);
    }

    //If data is left on high drop to low, so that the final output voltage is not altered
    digitalWrite(P_LED_DATA, LOW);

    //Latch high. VCC adjust to two volts
    digitalWrite(P_LED_LATCH, HIGH);
    analogWrite(P_LED_VCC, VCC_PWM);

}

void LedManager::turnOn(uint8_t ledNumber) {
    log(TYPE_DEBUG, "led", "Set led number " + String(ledNumber) + " ON");

    states[ledNumber] = HIGH;
}

void LedManager::turnOff(uint8_t ledNumber) {
    log(TYPE_DEBUG, "led", "Set led number " + String(ledNumber) + " OFF");

    states[ledNumber] = LOW;
}


