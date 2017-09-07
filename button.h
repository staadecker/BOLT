#ifndef BUTTON
#define BUTTON

#include "const.h"

//Stores the value of the button pressed or depressed
volatile uint8_t Button = 0;

//Called when the P_BUTTON_INTERRUPT is falling
void buttonISR(){
  
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
      Button = Button + 1;
    }
    
    if(i != 7)
    {
      Button = Button << 1;
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

//Ends when the button with buttonNumber is pressed
void waitForButtonPress(int buttonNumber){
  while (Button != buttonNumber + 128){
  }
  Button = 0;
}

#endif
