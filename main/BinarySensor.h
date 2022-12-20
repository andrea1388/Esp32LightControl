#include "driver/gpio.h"
#include "Base.h"
class BinarySensor:Base {
    uint8_t debounceTime;
    int64_t tLastReading;
}