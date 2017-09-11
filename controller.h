/*  File containing the thread controller.
 *  
 *  The thread contoller runs all the threads when the controller_run method is called.
 *  
 *  Threads can be added using controller_add
*/

#ifndef CONTROLLER
#define CONTROLLER

#include <Thread.h>
#include <ThreadController.h>


ThreadController controller = ThreadController();

//Add a thread to the controller
void controller_add(Thread thread){
  controller.add(&thread);
}

//Run the controller to run the threads that are enabled
void controller_run(){
  controller.run();
}

#endif
