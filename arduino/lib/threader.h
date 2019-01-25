//
// Created by machs on 1/22/2019.
//

#ifndef BOLT_THREADER_H
#define BOLT_THREADER_H

namespace runnablesManager {
    namespace {
        const unsigned char MAXIMUM_NUMBER_OF_RUNNABLES = 8;
    }
}

class Runnable {
public:
    virtual void onRun() = 0;

    unsigned char runnableId = runnablesManager::MAXIMUM_NUMBER_OF_RUNNABLES;
};

namespace runnablesManager {
    void addRunnable(Runnable *runnable);

    void removeRunnable(Runnable *runnable);

    void execute();
};


#endif //BOLT_THREADER_H
