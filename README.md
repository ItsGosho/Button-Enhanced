# Button-Enhanced
[![License](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/ItsGosho/Button-Enhanced/blob/main/LICENSE)
[![Version](https://img.shields.io/github/v/release/ItsGosho/Button-Enhanced?include_prereleases)](https://github.com/ItsGosho/Button-Enhanced/blob/main/LICENSE)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/ItsGosho/library/ButtonEnhanced.svg)](https://registry.platformio.org/libraries/ItsGosho/ButtonEnhanced)

<img src="pics\image-20221004213143380.png" alt="image-20221004213143380" style="zoom:50%;" />

## Functionalities:

- Configure the separation between a single press and hold.
- Manually check for a single press or hold
- Set a callback for single press or hold
- Disable a specific or all callbacks at once for single press or hold
- Define how often to receive hold notification, which works for both checks - manually & callback
- Tracking of total single presses.
- Tracking of total holds.
- Resume or pause the whole button logic.
- Resume or pause the counting logic.
- Resume or pause the callback notifications.
- Change the button pin dynamically



**For examples of a functionality usage, please see the examples.**

## Installation:

##### Standard:

- Download the two source files [ButtonEnhanced.h]( https://github.com/ItsGosho/Button-Enhanced/blob/main/src/ButtonEnhanced.h) & [ButtonEnhanced.cpp](https://github.com/ItsGosho/Button-Enhanced/blob/main/src/ButtonEnhanced.cpp)  from the [latest release]( https://github.com/ItsGosho/Button-Enhanced/releases) 's file **ButtonEnhanced.zip** and place them in your project.

##### PlatformIO:

- TODO: Upload the package and link it



## Simple Example:

##### Notification about single press or hold using callbacks:

```c++
#include <Arduino.h>
#include "ButtonEnhanced.h"

#define BUTTON_PIN 22

ButtonEnhanced buttonEnhanced;

void onButtonShot() {
    Serial.println("Button press detected!");
}

void onButtonHold() {
    Serial.println("Button hold detected!");
}

void setup() {
    Serial.begin(9600);

    buttonEnhanced = ButtonEnhanced(BUTTON_PIN);
    buttonEnhanced.setOnShotCallback(onButtonShot);
    buttonEnhanced.setOnHoldCallback(onButtonHold);
}

void loop() {
    buttonEnhanced.refreshReading();
}
```

##### Notification about single press or hold via methods:

```c++
#include <Arduino.h>
#include "ButtonEnhanced.h"

#define BUTTON_PIN 22

ButtonEnhanced buttonEnhanced;

void setup() {
    Serial.begin(9600);

    buttonEnhanced = ButtonEnhanced(BUTTON_PIN);
}

void loop() {

    if(buttonEnhanced.isShot()) {
        Serial.println("Button press detected!");
    }

    if(buttonEnhanced.isHold()) {
        Serial.println("Button hold detected!");
    }
}
```



## Configuration:

- There is a default configuration for the way that Button Enhanced works. That way you don't need to deal with something unclear to you. But if you want to customize how our library works - you can do it.
- There are **3 core parameters** that can be set upon **ButtonEnhanced**  initialization or dynamically later in your program:

```c++
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
```



##### Shot Threshold:

- Set minimal amount of time in milliseconds that the button must be in pressed state to be counted as single press (shot).
- The **default** time is **15 milliseconds**

<img src="pics\image-20221005033245540.png" alt="image-20221005033245540" style="zoom:50%;" />




##### Hold Threshold:

- Set minimal amount of time in milliseconds that the button must be in pressed state to be counted as hold.
- The **default** time is **200 milliseconds**

<img src="pics\image-20221005034011271.png" alt="image-20221005034011271" style="zoom:50%;" />



##### Hold Notification:

- Set minimal amount of interval time in milliseconds that the notification for hold is called
- The **default** time is **500 milliseconds**

<img src="pics\image-20221005034612276.png" alt="image-20221005034612276" style="zoom:50%;" />



## Other Functionalities:







## Additional Information:

- The library was written with the idea to be used in [another project](https://github.com/ItsGosho/Pong-Pong).
- While writing the library, most of the ideas and concepts were drawn in [Excalidraw](https://excalidraw.com/) ,which is a sketching tool. You can find the latest sketches in the [excali](https://github.com/ItsGosho/Button-Enhanced/tree/dev/excali) folder.


<img src="pics\image-20221005040945841.png" alt="image-20221005040945841" style="zoom:50%;" />






