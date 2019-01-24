//
// Created by machs on 1/22/2019.
//

#ifndef BOLT_THREADER_H
#define BOLT_THREADER_H

namespace threadManager {
    namespace {
        const unsigned char MAX = 8;
    }
}

class Thread {
public:
    virtual void runThread() = 0;

    unsigned char threadId = threadManager::MAX;
};

namespace threadManager {
    void addThread(Thread *thread);

    void removeThread(Thread *thread);

    void runThreader();
};


#endif //BOLT_THREADER_H
