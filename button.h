/* Manages interaction with the buttons */
#ifndef BUTTON
#define BUTTON

#include "const.h"
#include "controller.h"

//Stores the value of the button pressed or depressed
volatile uint8_t button = 0;

//Called when the P_BUTTON_INTERRUPT is falling
void button_ISR(){
  
  uint8_t  val  = 0;
  uint8_t  clk  = 0;

  clk = digitalRead(P_BUTTON_CLOCK);
  while(clk == LOW)
  {
    clk = digitalRead(P_BUTTON_CLOCK);
  }

  for(int i = 0; i<8; i++)
  {   
    val = digitalRead(P_BUTTON_DATA);
    clk = digitalRead(P_BUTTON_CLOCK);
    
    while(clk == HIGH)
    {
      clk = digitalRead(P_BUTTON_CLOCK);
    }

    if(val == HIGH)
    {
      button = button + 1;
    }
    
    if(i != 7)
    {
      button = button << 1;
    }
    else
    {
      break;
    }

    clk = digitalRead(P_BUTTON_CLOCK);
    
    while(clk == LOW)
    {
      clk = digitalRead(P_BUTTON_CLOCK);
    }
  }
}

void button_isPressed(int buttonNumber){
  if(button != buttonNumber + 128){
    button = 0;
    return true;
  }
  return false;
}

//Ends when the button with buttonNumber is pressed
void button_wait(int buttonNumber){
  while (not button_isPressed){
    controller_run();
  }
}


#endif
