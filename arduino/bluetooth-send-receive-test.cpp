#include <SoftwareSerial.h>
#include <Arduino.h>

//Enter AT commands in the Serial Monitor and get the results

//Uses the AT-09 Bluetooth chip with custom firmware from here : https://github.com/RedBearLab/CCLoader
//See this tutorial for flashing : https://blog.yavilevich.com/2018/04/should-you-throw-away-your-cc41-hm-10-clones-now-that-android-8-is-here/


//Use Baud Rate 9600 on Serial Monitor
//Use "No line endings" on Serial Monitor

SoftwareSerial BT(9, 10); // RX, TX
// Connect HM10      Arduino Uno
//     Pin 1/TXD          Pin 9
//     Pin 2/RXD          Pin 10

String command;

void setup() {
    Serial.begin(9600);
    BT.begin(9600);

    Serial.println("READY");
}

void loop() {
    if (Serial.available()) {
        command = "";

        while (Serial.available()) {
            command += char(Serial.read());
            delay(10);
        }

        char char_array[command.length() + 1];
        command.toCharArray(char_array, command.length() + 1);

        Serial.print(">> ");
        Serial.println(command);
        BT.write(char_array);
    }

    if (BT.available()) {

        Serial.print("<< ");

        while (BT.available()) {
            Serial.print(char(BT.read()));
            delay(10);
        }

        Serial.println();
    }
}
