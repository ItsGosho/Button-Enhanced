#include <Arduino.h>
#include "ButtonEnhanced.cpp"

#define BUTTON_PIN 22

ButtonEnhanced buttonEnhanced;

void onButtonShot() {
    Serial.println("Shot!");
    Serial.println(buttonEnhanced.getTotalShots());

    if(buttonEnhanced.getTotalShots() >= 3) {
        buttonEnhanced.pauseTotalShotsCounting();
    }
}

void onButtonHold() {
    Serial.println("Hold!");
    Serial.println(buttonEnhanced.getTotalHolds());

    if(buttonEnhanced.getTotalHolds() >= 2) {
        buttonEnhanced.resumeTotalShotsCounting();
    }
}

void setup() {
    Serial.begin(9600);

    buttonEnhanced = ButtonEnhanced(BUTTON_PIN);
    buttonEnhanced.setOnShotCallback(onButtonShot);
    buttonEnhanced.setOnHoldCallback(onButtonHold);
}

void loop() {

    buttonEnhanced.checkState();
}