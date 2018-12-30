# LED Wiring

[Shift register datasheet](http://www.ti.com/lit/ds/symlink/sn74hc595.pdf). [Arduino tutorial for shift registers](https://www.arduino.cc/en/tutorial/ShiftOut).

Clock = SCK. Latch = RCK. Data = SER. 

VCC is connected to an output pin that allows `analogWrite` (PWM).

Check [`bolt-arduino/constants.h`](bolt-arduino/constants.h) to find what pins to use on the Arduino.

# Flashing new firware onto fake HM-10

Follow [this tutorial](https://blog.yavilevich.com/2018/04/should-you-throw-away-your-cc41-hm-10-clones-now-that-android-8-is-here/).

