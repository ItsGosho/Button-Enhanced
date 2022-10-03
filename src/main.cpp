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

/**
 * When a button is kept hold we don't want to inform the user that fast.
 * Suppose the user decided that he want to receive the notification when a button is pressed each 500 ms.
 * He would want that if he had a game that requires moving a object while a button is pressed.
 * Each 500 ms he receives notification he will move the object with 1 pixel.
 * Changing the notification delay will increase/decrease the smoothness of the movement.
 */
bool isHoldNotificationTimePassed() {
    return (millis() - holdNotificationLastMS) >= holdNotificationMS;
}

/**
 * When a button is activated we do not know if there was a single fast click or it is kept hold.
 * We must define separation between these two phases.
 * The @param holdThresholdMS defines after what activation/pressing time the button is in hold phase.
 */
bool isEnteredHold() {
    return timeMS >= holdThresholdMS;
}

void loop() {

    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState && startMS == 0) { // Pressed
        startMS = millis();
    }

    if (buttonState && startMS > 0) { // Intermediate. Still not released, but pressed.
        timeMS = millis() - startMS;

        if (isEnteredHold() && isHoldNotificationTimePassed()) {
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