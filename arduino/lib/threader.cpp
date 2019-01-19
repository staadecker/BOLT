//
// Created by machs on 2019-01-19.
//

#include "threader.h"
#include "logger.h"

namespace threader {
    static const uint8_t MAX = 8;

    ThreaderCallback *callbacks[MAX] = {};

    uint8_t addCallback(ThreaderCallback *threaderCallback) {
        for (uint8_t i = 0; i < MAX; ++i) {
            if (not callbacks[i]) {
                callbacks[i] = threaderCallback;
                return i;
            }
        }

        log(TYPE_ERROR, "threader", "Could not add callback. Maxed out.");
        return MAX;
    }

    void removeCallback(uint8_t id) {
        callbacks[id] = nullptr;
    }

    void runThreader() {
        for (ThreaderCallback *callback : callbacks) {
            if (callback) {
                callback->call();
            }
        }
    }
}