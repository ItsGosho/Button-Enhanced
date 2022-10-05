#include <Arduino.h>
#include "ButtonEnhanced.h"

#define BUTTON_PIN 22

ButtonEnhanced buttonEnhanced;

void setup() {
    Serial.begin(9600);

    //Preparing the configuration
    ButtonEnhanced::Configuration configuration = ButtonEnhanced::Configuration();
    configuration.shotThresholdMS = DEFAULT_SHOT_THRESHOLD_MS;
    configuration.holdThresholdMS = DEFAULT_HOLD_THRESHOLD_MS;
    configuration.holdNotificationMS = DEFAULT_HOLD_NOTIFICATION_MS;

    //During initialization. If not provided configuration, the default one is used.
    buttonEnhanced = ButtonEnhanced(BUTTON_PIN, configuration);

    //Dynamically, overwriting the existing one.
    buttonEnhanced.setShotThresholdMs(DEFAULT_HOLD_THRESHOLD_MS);
    buttonEnhanced.setHoldThresholdMs(DEFAULT_HOLD_THRESHOLD_MS);
    buttonEnhanced.setHoldNotificationMs(DEFAULT_HOLD_NOTIFICATION_MS);
}

void loop() {

    if (buttonEnhanced.isShot()) {
        Serial.println("Button press detected!");
    }

    if (buttonEnhanced.isHold()) {
        Serial.println("Button hold detected!");
    }
}