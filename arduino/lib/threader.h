//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_THREADER_H
#define BOLT_THREADER_H

#include <stdint-gcc.h>

class ThreaderCallback {
public:
    virtual void call();
};

namespace threader {
    uint8_t addCallback(ThreaderCallback *threaderCallback);

    void removeCallback(uint8_t id);

    void runThreader();
};


#endif //BOLT_THREADER_H
