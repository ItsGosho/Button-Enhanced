#ifndef ITSGOSHO_BUTTON_ENHANCED_H
#define ITSGOSHO_BUTTON_ENHANCED_H

#include <Arduino.h>
#include <cstdint>

#define PRESSED_READING 0
#define INTERMEDIATE_READING 1
#define RELEASED_READING 2
#define UNKNOWN_READING 3

#define SHOT_ACTION 0
#define HOLD_ACTION 1
#define UNKNOWN_ACTION 2

#define DEFAULT_SHOT_THRESHOLD_MS 15
#define DEFAULT_HOLD_THRESHOLD_MS 150
#define DEFAULT_HOLD_NOTIFICATION_MS 500

class ButtonEnhanced {

    typedef void (* onShot)();        //When a button is pressed and released immediately
    //typedef void (* onPress)();       //When a button is pressed.
    typedef void (* onHold)();        //When a button is kept pressed.
    //typedef void (* onRelease)();     //When a button is released.

    uint8_t buttonPin;
    unsigned long startMS = 0;
    unsigned long timeMS = 0;

    unsigned long shotThresholdMS = 15;
    unsigned long holdThresholdMS = 150;
    unsigned long holdNotificationLastMS = 0;
    unsigned long holdNotificationMS = 500;

    unsigned long totalShots;
    unsigned long totalHolds;

    bool isTotalShotsPaused;
    bool isTotalHoldsPaused;

    onShot onShotCallback;
    onHold onHoldCallback;

    uint8_t actionState;

    void setStartMs(const unsigned long& startMs) {
        this->startMS = startMs;
    }

    void setTimeMs(const unsigned long& timeMs) {
        this->timeMS = timeMs;
    }

    void setHoldNotificationLastMs(const unsigned long& holdNotificationLastMs) {
        this->holdNotificationLastMS = holdNotificationLastMs;
    }

    void setActionState(const uint8_t& actionState) {
        this->actionState = actionState;
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
     * A reading can be in 3 different states.
     * PRESSED_READING: We detected that there is a press of the provided button.
     * INTERMEDIATE_READING: There was no release of the button since the last reading. The button is still kept pressed.
     * RELEASED_READING: The button has been released.
     *
     * If the state can't be determined, then state UNKNOWN_READING: 3 is returned.
     */
    uint8_t getReadingState() {
        bool buttonRead = digitalRead(this->buttonPin);

        if (buttonRead && this->startMS == 0)
            return PRESSED_READING;
        else if (buttonRead && this->startMS > 0)
            return INTERMEDIATE_READING;
        else if (!buttonRead && this->startMS != 0)
            return RELEASED_READING;
        else
            return UNKNOWN_READING;
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
        this->setActionState(UNKNOWN_ACTION);
    }

    void refreshReading() {
        switch (getReadingState()) {

            case PRESSED_READING:
                this->startMS = millis();
                break;

            case INTERMEDIATE_READING:
                this->timeMS = millis() - this->startMS;

                if (this->isEnteredHold() && this->isHoldNotificationTimePassed()) {
                    this->holdNotificationLastMS = millis();

                    if (this->onHoldCallback)
                        this->onHoldCallback();

                    this->setActionState(HOLD_ACTION);
                }

                break;

            case RELEASED_READING:
                this->timeMS = millis() - this->startMS;

                if (this->timeMS >= this->shotThresholdMS && this->timeMS < this->holdThresholdMS) {

                    if (!this->isTotalShotsPaused) {
                        this->totalShots++;
                    }

                    if (this->onShotCallback)
                        this->onShotCallback();

                    this->setActionState(SHOT_ACTION);
                }

                if (this->timeMS >= this->holdThresholdMS && !this->isTotalHoldsPaused) {
                    this->totalHolds++;
                }

                this->startMS = 0;
                this->timeMS = 0;
                break;

            case UNKNOWN_READING:
                break;
        }
    }

    bool isShot() {
        this->refreshReading();
        uint8_t isShot = this->actionState == SHOT_ACTION;

        if (isShot) {
            this->actionState = UNKNOWN_ACTION;
            return true;
        }

        return false;
    }

    bool isHold() {
        this->refreshReading();
        uint8_t isHold = this->actionState == HOLD_ACTION;

        if (isHold) {
            this->actionState = UNKNOWN_ACTION;
            return true;
        }

        return false;
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

    void setOnShotCallback(void (* callback)()) {
        this->onShotCallback = callback;
    }

    void setOnHoldCallback(void (* callback)()) {
        this->onHoldCallback = callback;
    }

    unsigned long getTotalShots() const {
        return this->totalShots;
    }

    unsigned long getTotalHolds() const {
        return this->totalHolds;
    }

    void clearTotalShots() {
        this->totalShots = 0;
    }

    void clearTotalHolds() {
        this->totalHolds = 0;
    }

    void clearTotals() {
        this->clearTotalShots();
        this->clearTotalHolds();
    }

    void pauseTotalShotsCounting() {
        this->isTotalShotsPaused = true;
    }

    void pauseTotalHoldsCounting() {
        this->isTotalHoldsPaused = true;
    }

    void pauseTotals() {
        this->pauseTotalShotsCounting();
        this->pauseTotalHoldsCounting();
    }

    void resumeTotalShotsCounting() {
        this->isTotalShotsPaused = false;
    }

    void resumeTotalHoldsCounting() {
        this->isTotalHoldsPaused = false;
    }

    void resumeTotals() {
        this->resumeTotalShotsCounting();
        this->resumeTotalHoldsCounting();
    }

    bool getIsTotalShotsPaused() const {
        return this->isTotalShotsPaused;
    }

    bool getIsTotalHoldsPaused() const {
        return this->isTotalHoldsPaused;
    }
};

#endif //ITSGOSHO_BUTTON_ENHANCED_H
