#include "BinarySensor.h"
BinarySensor::BinarySensor() {
    debounceTime=50;
}

void BinarySensor:run() {
    if(debouceTimeElapsed())
        processInput();
}

void BinarySensor::processInput() {
    ns=gpio_get_level(pin);
    if(state!=ns) {
        state=ns;
        tLastReading=now;
        publish();
    }
}

inline bool BinarySensor::debouceTimeElapsed() {
    now=(esp_timer_get_time()/1000);
    return((now-tLastReading)>debounceTime);
}

void BinarySensor::publish() {
    char msg[4];
    if(!mqtt) return;
    if(state) strcpy(msg,"ON"); else strcpy(msg,"OFF");
    mqtt.Publish(mqttStateTopic,msg); 
}