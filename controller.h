/*  File containing the thread controller.

    The thread contoller runs all the threads when the controller_run method is called.

    Threads can be added using controller_add
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Thread.h>

namespace controller {
  void addThread(Thread* thread);
  void runController();
}

#endif
