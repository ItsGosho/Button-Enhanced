#include <Arduino.h>
#include "ButtonEnhanced.cpp"

#define BUTTON_PIN 22

ButtonEnhanced buttonEnhanced;

void setup() {
    Serial.begin(9600);

    buttonEnhanced = ButtonEnhanced(BUTTON_PIN);
}

void loop() {

    buttonEnhanced.checkState();

    /* if (timeMS != 0) {
         Serial.println(timeMS);

         //TODO: I want to inform if it was single click or

         timeMS = 0;
     }*/
}