//
// Created by machs on 2019-01-19.
//

#include <USBAPI.h>
#include "threader.h"

namespace runnablesManager {
    Runnable *runnables[MAXIMUM_NUMBER_OF_RUNNABLES] = {};

    void addRunnable(Runnable *runnable) {
        for (unsigned char index = 0; index < MAXIMUM_NUMBER_OF_RUNNABLES; ++index) {
            if (runnables[index] == nullptr) {
                runnables[index] = runnable;
                runnable->runnableId = index; //Set the runnable's id to the index in the list
                return;
            }
        }

        Serial.println("Could not add runnable. Already too many.");
    }

    void removeRunnable(Runnable *runnable) {
        runnables[runnable->runnableId] = nullptr;
    }

    void execute() {
        for (Runnable *callback : runnables) {
            if (callback != nullptr) {
                callback->onRun();
            }
        }

        delay(10);
    }
}