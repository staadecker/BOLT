#include "controller.h"

#include "logger.h"
#include <Thread.h>
#include <ThreadController.h>

namespace controller {
  namespace {
    ThreadController controller = ThreadController();
  }
  //Add a thread to the controller
  void addThread(Thread* thread) {

    bool result = controller.add(thread);
    
    if (result) {
      logger::logger(logger::TYPE_DEBUG, "controller", "Added thread : " + String(thread->ThreadName));
    } else {
      logger::logger(logger::TYPE_ERROR, "controller", "Failed to add thread : " + String(thread->ThreadName));
    }
  }

  //Run the controller to run the threads that are enabled
  void runController() {
    controller.run();
  }
}
