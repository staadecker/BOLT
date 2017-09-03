#ifndef button.h
#define button.h

const int PIN_INTERRUPT = 2;
const int PIN_DATA_IN = 3;
const int PIN_CLOCK = 4;

volatile uint8_t Button;

volatile uint8_t buttonPressed = 0;

void buttonCallback(){
  Button = 0;
  
  volatile uint8_t  val  = 0;
  volatile uint8_t  clk  = 0;

  clk = digitalRead(PIN_CLOCK);
  while(clk == LOW)
  {
    clk = digitalRead(PIN_CLOCK);
  }

  for(volatile int i =0; i<8;i++)
  {   
    val = digitalRead(PIN_DATA_IN);
    clk = digitalRead(PIN_CLOCK);
    
    while(clk == HIGH)
    {
      clk = digitalRead(PIN_CLOCK);
    }

    if(val == HIGH)
    {
      Button = Button +1;
    }
    
    if(i != 7)
    {
      Button = Button << 1;
    }
    else
    {
      break;
    }

    clk = digitalRead(PIN_CLOCK);
    
    while(clk == LOW)
    {
      clk = digitalRead(PIN_CLOCK);
    }
  }

  if (Button > 128){
    buttonPressed = Button - 128;
    Serial.println(buttonPressed);
  }
  
}

void waitForButtonPress(int buttonNumber){
  while (buttonPressed != buttonNumber){
  }
  buttonPressed = 0;
}

#endif
