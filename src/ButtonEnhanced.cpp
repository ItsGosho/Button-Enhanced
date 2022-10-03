#ifndef ITSGOSHO_BUTTON_ENHANCED_H
#define ITSGOSHO_BUTTON_ENHANCED_H

#include <Arduino.h>

#define PRESSED 0
#define INTERMEDIATE 1
#define RELEASED 2
#define UNKNOWN 3

#define DEFAULT_SHOT_THRESHOLD_MS 15
#define DEFAULT_HOLD_THRESHOLD_MS 150
#define DEFAULT_HOLD_NOTIFICATION_MS 500

class ButtonEnhanced {

    uint8_t buttonPin;
    unsigned long startMS = 0;
    unsigned long timeMS = 0;

    unsigned long shotThresholdMS = 15;
    unsigned long holdThresholdMS = 150;
    unsigned long holdNotificationLastMS = 0;
    unsigned long holdNotificationMS = 500;

    typedef void (* onShot);        //When a button is pressed and released immediately
    typedef void (* onPress);       //When a button is pressed.
    typedef void (* onHold);        //When a button is kept pressed.
    typedef void (* onRelease);     //When a button is released.

    void setStartMs(const unsigned long& startMs) {
        this->startMS = startMs;
    }

    void setTimeMs(const unsigned long& timeMs) {
        this->timeMS = timeMs;
    }

    void setHoldNotificationLastMs(const unsigned long& holdNotificationLastMs) {
        this->holdNotificationLastMS = holdNotificationLastMs;
    }

public:

    ButtonEnhanced() {
    }

    ButtonEnhanced(const uint8_t& buttonPin) : buttonPin(buttonPin) {
        pinMode(buttonPin, INPUT);
        this->setDefaults();
    }

    void setDefaults() {
        this->setStartMs(0);
        this->setTimeMs(0);
        this->setShotThresholdMs(DEFAULT_SHOT_THRESHOLD_MS);
        this->setHoldThresholdMs(DEFAULT_HOLD_THRESHOLD_MS);
        this->setHoldNotificationLastMs(0);
        this->setHoldNotificationMs(DEFAULT_HOLD_NOTIFICATION_MS);
    }

    /**
    * When a button is kept hold we don't want to inform the user that fast.
    * Suppose the user decided that he want to receive the notification when a button is pressed each 500 ms.
    * He would want that if he had a game that requires moving a object while a button is pressed.
    * Each 500 ms he receives notification he will move the object with 1 pixel.
    * Changing the notification delay will increase/decrease the smoothness of the movement.
    */
    bool isHoldNotificationTimePassed() {
        return (millis() - this->holdNotificationLastMS) >= this->holdNotificationMS;
    }

    /**
     * When a button is activated we do not know if there was a single fast click or it is kept hold.
     * We must define separation between these two phases.
     * The @param holdThresholdMS defines after what activation/pressing time the button is in hold phase.
     */
    bool isEnteredHold() {
        return this->timeMS >= this->holdThresholdMS;
    }

    /**
     * A button can be in 3 different states.
     * PRESSED: We detected that there is a press of the provided button.
     * INTERMEDIATE: There was no release of the button since the last reading. The button is still kept pressed.
     * RELEASED: The button has been released.
     *
     * If the state can't be determined, then state UNKNOWN: 3 is returned.
     */
    uint8_t getState() {
        bool buttonRead = digitalRead(this->buttonPin);

        if (buttonRead && this->startMS == 0)
            return PRESSED;
        else if (buttonRead && this->startMS > 0)
            return INTERMEDIATE;
        else if (!buttonRead && this->startMS != 0)
            return RELEASED;
        else
            return UNKNOWN;
    }

    void checkState() {
        switch (getState()) {

            case PRESSED:
                this->startMS = millis();
                break;

            case INTERMEDIATE:
                this->timeMS = millis() - this->startMS;

                if (this->isEnteredHold() && this->isHoldNotificationTimePassed()) {
                    Serial.println("Hold");
                    this->holdNotificationLastMS = millis();
                }

                break;

            case RELEASED:
                this->timeMS = millis() - this->startMS;

                if (this->timeMS >= this->shotThresholdMS && this->timeMS < this->holdThresholdMS) {
                    Serial.println("Shot!");
                }

                this->startMS = 0;
                this->timeMS = 0;
                break;

            case UNKNOWN:
                break;
        }
    }

    void setShotThresholdMs(const unsigned long& shotThresholdMs) {
        this->shotThresholdMS = shotThresholdMs;
    }

    void setHoldThresholdMs(const unsigned long& holdThresholdMs) {
        this->holdThresholdMS = holdThresholdMs;
    }

    void setHoldNotificationMs(const unsigned long& holdNotificationMs) {
        this->holdNotificationMS = holdNotificationMs;
    }
};

#endif //ITSGOSHO_BUTTON_ENHANCED_H
