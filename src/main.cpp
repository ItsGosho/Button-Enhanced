#include <Arduino.h>
#include "ButtonEnhanced.cpp"

#define BUTTON_PIN 22

ButtonEnhanced buttonEnhanced;

void setup() {
    Serial.begin(9600);

    buttonEnhanced = ButtonEnhanced(BUTTON_PIN);
}

unsigned long startMS = 0;
unsigned long timeMS = 0;

unsigned long shotThresholdMS = 15;
unsigned long holdThresholdMS = 150;
unsigned long holdNotificationLastMS = 0;
unsigned long holdNotificationMS = 500;

void loop() {

    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState && startMS == 0) { // Pressed
        startMS = millis();
    }

    if (buttonState && startMS > 0) { // Intermediate. Still not released, but pressed.
        timeMS = millis() - startMS;

        if (timeMS >= holdThresholdMS && (millis() - holdNotificationLastMS) >= holdNotificationMS) {
            Serial.println("Hold");
            holdNotificationLastMS = millis();
        }
    }

    if (!buttonState && startMS != 0) { // Released
        timeMS = millis() - startMS;

        if (timeMS >= shotThresholdMS && timeMS < holdThresholdMS) {
            Serial.println("Shot!");
        }

        startMS = 0;
        timeMS = 0;
    }

    /* if (timeMS != 0) {
         Serial.println(timeMS);

         //TODO: I want to inform if it was single click or

         timeMS = 0;
     }*/
}