#include "controller.h"

#include <Thread.h>
#include <ThreadController.h>


ThreadController controller = ThreadController();

//Add a thread to the controller
void controller_add(Thread thread) {
  controller.add(&thread);
}

//Run the controller to run the threads that are enabled
void controller_run() {
  controller.run();
}
