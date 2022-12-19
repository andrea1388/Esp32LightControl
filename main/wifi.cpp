inline void wifiSetup() {
    char *ssid=NULL;
    char *password=NULL;
    param.load("wifi_ssid",&ssid,"ssid");
    param.load("wifi_password",&password,"password");
    wifi.onEvent=&WiFiEvent;
    if(ssid) wifi.Start(ssid,password);
    free(ssid);
    free(password);

    }


void WiFiEvent(WiFi* wifi, uint8_t ev)
{
    switch(ev)
    {
        case WIFI_START: // start
            wifi->Connect();
            break;
        case WIFI_DISCONNECT: // disconnected
            xEventGroupClearBits(event_group, WIFI_CONNECTED_BIT);
            wifi->Connect();
            gpio_set_level(GPIO_LED,0);
            //mqtt.Stop();
            break;
        case WIFI_GOT_IP: // connected
            ESP_LOGI(TAG,"GotIP");
            //ESP_LOGI("Connected. ip=%s",wifi->ip);
            xEventGroupSetBits(event_group, WIFI_CONNECTED_BIT);
            gpio_set_level(GPIO_LED,1);
            mqtt.Start();
            break;

    }
}
