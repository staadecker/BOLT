#include "controller.h"

#include "logger.h"
#include <Thread.h>
#include <ThreadController.h>


ThreadController controller = ThreadController();

//Add a thread to the controller
void controller_add(Thread thread) {
  controller.add(&thread);
}

//Run the controller to run the threads that are enabled
void controller_run() {
  logger(LOGGER_TYPE_DEBUG, "controller", "Controller run");
  controller.run();
}
