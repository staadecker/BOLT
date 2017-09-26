#include "controller.h"

#include "logger.h"
#include <Thread.h>
#include <ThreadController.h>


ThreadController controller = ThreadController();

//Add a thread to the controller
boolean controller_add(Thread* thread) {
  logger(LOGGER_TYPE_DEBUG, "controller", "Added thread");
  return controller.add(thread);
}

//Run the controller to run the threads that are enabled
void controller_run() {
  //logger(LOGGER_TYPE_DEBUG, "controller", "Controller run : " + String(controller.size(false)));
  controller.run();
}
