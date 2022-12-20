#include "driver/gpio.h"
#include "mqtt.h"
#include "BinarySensor.h"
#include "Base.h"

class Switch:Base {
    BinarySensor *input;
    uint8_t onTime;
    bool toggleMode;
}

