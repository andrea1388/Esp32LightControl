    inline void mqttSetup() {
    char *username=NULL;
    password=NULL;
    char *uri=NULL; // mqtturi mqtts://mqttserver:8883
    param.load("mqtt_username",&username);
    param.load("mqtt_password",&password);
    param.load("mqtt_uri",&uri);
    param.load("mqtt_tptopic",&MqttTpTopic,"SolarThermostat/Tp");
    param.load("mqtt_tttopic",&MqttTtTopic,"SolarThermostat/Tt");
    param.load("mqtt_tftopic",&MqttTfTopic,"SolarThermostat/Tf");
    param.load("mqtt_cttopic",&MqttControlTopic,"SolarThermostat/control");
    param.load("mqtt_sttopic",&MqttStatusTopic,"SolarThermostat/status");
    param.load("mqtt_sttopic",&MqttInfoTopic,"SolarThermostat/info");
    
    if(uri) 
    {
        mqtt.Init(username,password,uri,(const char*)ca_crt_start);
        mqtt.onEvent=&MqttEvent;
        ESP_LOGI(TAG,"Mqtt started");
    }
    free(username);
    free(password);
    free(uri);
}


void MqttEvent(Mqtt* mqtt, esp_mqtt_event_handle_t event)
{
    switch (event->event_id) 
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            mqtt->Subscribe(MqttControlTopic);
            if(disableThermostat)
                mqtt->Publish(MqttStatusTopic,"OFF");
            else
                mqtt->Publish(MqttStatusTopic,"ON");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            if(strncmp(event->topic,MqttControlTopic,event->topic_len)==0) {
                char* buf;
                buf=(char *)malloc(event->data_len+1);
                strncpy(buf,event->data,event->data_len);
                buf[event->data_len]=0;
                onNewCommand(buf);
                free(buf);
            }
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_ESP_TLS) {
                ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
                ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
            } else {
                ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}
