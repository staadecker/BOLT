//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_THREADER_H
#define BOLT_THREADER_H

#include <stdint-gcc.h>

class Thread {
public:
    virtual void runThread();

    uint8_t threadId = 0;
};

namespace threadManager {
    void addThread(Thread *thread);

    void removeThread(Thread *thread);

    void runThreader();
};


#endif //BOLT_THREADER_H
