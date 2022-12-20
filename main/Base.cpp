#include "Base.h"

void Base::publish() {
    char msg[4];
    if(!mqtt) return;
    if(state) strcpy(msg,"ON"); else strcpy(msg,"OFF");
    mqtt.Publish(mqttStateTopic,msg); 
}