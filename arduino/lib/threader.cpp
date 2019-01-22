//
// Created by machs on 2019-01-19.
//

#include <USBAPI.h>
#include "threader.h"

namespace threadManager {
    Thread *threads[MAX] = {};

    void addThread(Thread *thread) {
        for (unsigned char i = 0; i < MAX; ++i) {
            if (not threads[i]) {
                threads[i] = thread;
                thread->threadId = i;
                return;
            }
        }

        Serial.println("Could not add callback. Maxed out.");
    }

    void removeThread(Thread *thread) {
        threads[thread->threadId] = nullptr;
    }

    void runThreader() {
        for (Thread *callback : threads) {
            if (callback) {
                callback->runThread();
            }
        }
        delay(10);
    }
}