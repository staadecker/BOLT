//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_THREADER_H
#define BOLT_THREADER_H

namespace threadManager {
    static const unsigned char MAX = 8;
}

class Thread {
public:
    virtual void runThread();

    unsigned char threadId = threadManager::MAX;
};

namespace threadManager {
    void addThread(Thread *thread);

    void removeThread(Thread *thread);

    void runThreader();
};


#endif //BOLT_THREADER_H
