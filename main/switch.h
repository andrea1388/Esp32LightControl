#include "mqtt.h"
class Switch {
    mqtt *mqtt;
    BinarySensor *input;
    void run();
}

