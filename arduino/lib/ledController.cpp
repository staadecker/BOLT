#include <USBAPI.h>
#include "ledController.h"

LedController::LedController() {
    pinMode(PIN_LED_SHIFT_REG_VCC, OUTPUT);
    pinMode(PIN_LED_SHIFT_REG_DATA, OUTPUT);
    pinMode(PIN_LED_SHIFT_REG_CLOCK, OUTPUT);
    pinMode(PIN_LED_SHIFT_REG_LATCH, OUTPUT);
}

void LedController::shiftOutLEDs() {
#if DEBUG
    Serial.println("Shift OUT leds");
#endif

    //Latch Low. VCC high
    digitalWrite(PIN_LED_SHIFT_REG_VCC, HIGH);
    digitalWrite(PIN_LED_SHIFT_REG_LATCH, LOW);


    //Shift out
    for (unsigned char i = NUMBER_OF_LEDS; i <= NUMBER_OF_LEDS; i--) {
        digitalWrite(PIN_LED_SHIFT_REG_DATA, static_cast<unsigned char>(ledStates[i]));

        //Wait for clock cycle
        digitalWrite(PIN_LED_SHIFT_REG_CLOCK, HIGH);
        digitalWrite(PIN_LED_SHIFT_REG_CLOCK, LOW);
    }

    //If data is left on high drop to low, so that the final output voltage is not altered
    digitalWrite(PIN_LED_SHIFT_REG_DATA, LOW);

    //Latch high. VCC adjust to two volts
    digitalWrite(PIN_LED_SHIFT_REG_LATCH, HIGH);
    analogWrite(PIN_LED_SHIFT_REG_VCC, PWM_VALUE_FOR_VCC_PIN);
}

void LedController::turnOnLed(unsigned char ledNumber) {
#if DEBUG
    Serial.println("Turn ON led :" + String(ledNumber));
#endif
    ledStates[ledNumber] = HIGH;
}

void LedController::turnOffLed(unsigned char ledNumber) {
#if DEBUG
    Serial.println("Turn OFF led :" + String(ledNumber));
#endif
    ledStates[ledNumber] = LOW;
}


