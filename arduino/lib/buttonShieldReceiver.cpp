
#include <USBAPI.h>
#include "buttonShieldReceiver.h"
#include "constants.h"

namespace ButtonShield {
    namespace queue {
        //Implementation of a circular queue representing the buttons that were registered by isr
        const static byte QUEUE_LENGTH = 8;
        volatile static unsigned char buttons[QUEUE_LENGTH];
        volatile static byte positionOfFirstItem = 0;
        volatile static byte nextEmptyPosition = 0;
    }

    ButtonShieldButtonPressReceiver buttonShieldButtonPressReceiver = ButtonShieldButtonPressReceiver();

    void isrReadButtonPress() {
        if (buttonShieldButtonPressReceiver.buttonPressListener != nullptr) {
            unsigned char buttonNumber = 0;

            for (unsigned char i = 0; i < 8; i++) {
                while (digitalRead(PIN_BUTTON_SHIELD_CLOCK) == LOW);  //Wait for clock to go high

                buttonNumber += digitalRead(PIN_BUTTON_SHIELD_DATA);  //If value is high add one

                //If not last time in loop, shift bits
                if (i != 7) {
                    buttonNumber = buttonNumber << 1u;
                }

                while (digitalRead(PIN_BUTTON_SHIELD_CLOCK) == HIGH);  //Wait for clock to go low
            }

            if (buttonNumber > 64) {  // Button number starts at 129 when it is a press (rather than a un-press)
                // When there is a new item add it at the end and increase
                queue::buttons[queue::nextEmptyPosition] = buttonNumber - static_cast<unsigned char>(129);
                queue::nextEmptyPosition++;

                //Wrap around to make queue circular
                if (queue::nextEmptyPosition == queue::QUEUE_LENGTH) {
                    queue::nextEmptyPosition = 0;
                }
            }
        }
    }


    ButtonShieldButtonPressReceiver &create() {
        attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_SHIELD_INTERRUPT), isrReadButtonPress,
                        FALLING); //Attach interrupt for 64 button shield
        return buttonShieldButtonPressReceiver;
    }


    void ButtonShieldButtonPressReceiver::onRun() {
        //If nextEmpty and firstItem aren't equal there are items in the queue
        while (queue::positionOfFirstItem != queue::nextEmptyPosition) {
            buttonPressListener->onButtonPressed(queue::buttons[queue::positionOfFirstItem]);
            queue::positionOfFirstItem++;

            //Wrap around to make queue circular
            if (queue::positionOfFirstItem == queue::QUEUE_LENGTH) {
                queue::positionOfFirstItem = 0;
            }
        }
    }
}