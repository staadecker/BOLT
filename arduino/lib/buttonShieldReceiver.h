/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include "buttonReceiver.h"
#include "runnablesManager.h"

namespace ButtonShield {
    class ButtonShieldButtonPressReceiver : public ButtonPressReceiver {
        void onRun() override;
    };

    ButtonShieldButtonPressReceiver &create();
}


#endif
