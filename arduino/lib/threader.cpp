//
// Created by machs on 2019-01-19.
//

#include "threader.h"
#include "logger.h"

namespace threadManager {
    static const uint8_t MAX = 8;

    Thread *threads[MAX] = {};

    void addThread(Thread *thread) {
        for (uint8_t i = 0; i < MAX; ++i) {
            if (not threads[i]) {
                threads[i] = thread;
                thread->threadId = i;
                return;
            }
        }

        log(TYPE_ERROR, "threader", "Could not add callback. Maxed out.");
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