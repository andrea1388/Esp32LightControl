#include "driver/gpio.h"
#include "switch.h"

Switch::Switch() {
    onTime=0;
    toggleMode=true;
    changeState(false);

}

Switch::changeState(bool s) {
    state=s;
    gpio_set_level(pin,state);
    tLastChange=now;
    publish();
}

Switch::run() {
    bool ns=input->state;
    if(ns!= state) {
        changeState(ns);
   
        
    }
    if(state) {
        if(tLastChange>onTime) {
            state=false;
            gpio_set_level(pin,state);
            tLastChange=now;
            publish();
        }
    }

}

void BinarySensor::publish() {
    char msg[4];
    if(!mqtt) return;
    if(state) strcpy(msg,"ON"); else strcpy(msg,"OFF");
    mqtt.Publish(mqttStateTopic,msg); 
}