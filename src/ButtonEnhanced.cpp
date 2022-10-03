#ifndef ITSGOSHO_BUTTON_ENHANCED_H
#define ITSGOSHO_BUTTON_ENHANCED_H

#include <Arduino.h>

class ButtonEnhanced {

    uint8_t buttonPin;

    typedef void (* onShot);        //When a button is pressed and released immediately
    typedef void (* onPress);       //When a button is pressed.
    typedef void (* onHold);        //When a button is kept pressed.
    typedef void (* onRelease);     //When a button is released.

    /**
     * The timing configuration defines what is shot and hold.
     */
    struct TimingConfiguration {
        uint8_t shotTimingMS;
        uint8_t holdTimingMS;
    };

public:

    ButtonEnhanced() {
    }

    ButtonEnhanced(const uint8_t& buttonPin) : buttonPin(buttonPin) {
        pinMode(buttonPin, INPUT);
    }

    /**
     *
     */
    void checkState() {

    }

};

#endif //ITSGOSHO_BUTTON_ENHANCED_H
