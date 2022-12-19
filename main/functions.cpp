void publishStatus() {
    char msg[199];
    sprintf(msg,"Tp=%.1f",Tp);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"Tt=%.1f",Tt);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"Tf=%.1f",Tf);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"Version=%u",VERSION);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"Tread=%u",Tread);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"Tsendtemps=%u",Tsendtemps);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"Ton=%u",Ton);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"Toff=%u",Toff);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"DT_TxMqtt=%u",DT_TxMqtt);
    mqtt.Publish(MqttInfoTopic,msg);
    
    sprintf(msg,"DT_ActPump=%u",DT_ActPump);
    mqtt.Publish(MqttInfoTopic,msg); 
    
    sprintf(msg,"DT_TxMqtt=%u",DT_TxMqtt);
    mqtt.Publish(MqttInfoTopic,msg); 

    sprintf(msg,"MinTankTempToUseForWaterHeathing=%u",MinTankTempToUseForWaterHeathing);
    mqtt.Publish(MqttInfoTopic,msg); 

    sprintf(msg,"SecFpPumpOn=%u",SecFpPumpOn);
    mqtt.Publish(MqttInfoTopic,msg); 

    sprintf(msg,"ThTFp=%u",ThTFp);
    mqtt.Publish(MqttInfoTopic,msg); 

}
