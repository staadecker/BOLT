#ifndef CONTROLLER
#define CONTROLLER

#include <Thread.h>
#include <ThreadController.h>

ThreadController controller = ThreadController();

void controller_add(Thread thread){
  controller.add(&thread);
}

void controller_run(){
  controller.run();
}

#endif
