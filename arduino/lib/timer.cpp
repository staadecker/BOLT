#include "timer.h"

#include "screen.h"

namespace timer {
    namespace {
        const int INTERVAL = 100;

        unsigned long lastUpdate = 0;

        void updateDisplay() {
            //screen::display(String(game::getRemainingTime()));
            lastUpdate = millis();
        }
    }

    void checkUpdateDisplay() {
        if (millis() > lastUpdate + INTERVAL) {
            updateDisplay();
        }
    }
}
