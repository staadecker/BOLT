#include "lib/startState.h"

StartingState startingState; //Must be define globally to still be accessible in loop function

void setup() {
    startingState.setup();
}

void loop() {
    runnablesManager::execute(); //Parts of the code will add themselves to the runnablesManager who will run them.
}


