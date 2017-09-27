#include "controller.h"

#include "logger.h"
#include <Thread.h>
#include <ThreadController.h>


ThreadController controller = ThreadController();

//Add a thread to the controller
void controller_add(Thread* thread) {
  
  bool result = controller.add(thread);
  if (result){
    logger(LOGGER_TYPE_DEBUG, "controller", "Added thread : " + String(thread->ThreadName));
  } else{
    logger(LOGGER_TYPE_ERROR, "controller", "Failed to add thread : " + String(thread->ThreadName));
  }
}

//Run the controller to run the threads that are enabled
void controller_run() {
  //logger(LOGGER_TYPE_DEBUG, "controller", "Controller run : " + String(controller.size(false)));
  controller.run();
}
