#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"
#include "driver/gpio.h"
// https://github.com/UncleRus/esp-idf-lib
// https://esp-idf-lib.readthedocs.io/en/latest/groups/ds18x20.html
#include <ds18x20.h>
#include "otafw.h"
// #include <iostream>

#include "wifi.h"
#include "mqtt.h"
#include "nvsparameters.h"
#include "BinarySensor.h"
#define MAXCMDLEN 200
#define TOTACheck 24
#define WIFI_CONNECTED_BIT BIT0
#define OTA_BIT BIT1
// ds18b20 sensors
#define GPIO_SENS_PANEL GPIO_NUM_21
#define GPIO_SENS_TANK GPIO_NUM_23
#define GPIO_FP_SENS GPIO_NUM_25
// pump actuators
#define GPIO_PUMP GPIO_NUM_18
#define GPIO_FPPUMP GPIO_NUM_32
#define GPIO_TANKPUMP GPIO_NUM_33

// binary sensors
#define GPIO_FLUX GPIO_NUM_34
#define GPIO_BOILERPUMP GPIO_NUM_35

#define GPIO_LED GPIO_NUM_4
#define GPIO_BUTTON GPIO_NUM_2
#define VERSION 10

static const char *TAG = "main";

extern "C"
{
    void app_main(void);
    extern void simple_ota_example_task(void *pvParameter);
}

// global objects
EventGroupHandle_t event_group;
float Tp = 0, Tt = 0, Tf = 0; // store the last temp read
uint8_t ThTFp = 70;           // threshold to activate fp pump
int64_t now;                  // milliseconds from startup
uint8_t Tread = 5;            // interval in seconds between temperature readings
uint8_t Tsendtemps = 1;       // interval in minutes between temperature transmissions to mqtt broker
uint8_t Ton = 40;             // On time in seconds of the pump (usually is the time required to empty the panel)
uint8_t Toff = 40;            // Off time in seconds
uint8_t SecFpPumpOn = 30;     // num of seconds fp pump must stays on
uint8_t DT_TxMqtt = 2;        // if one of the two value of temperature red exceeds this delta, then values are transmitted to mqtt
uint8_t DT_ActPump = 2;       // if Tpanel > Ttank + DT_ActPump, then pump is acted
uint8_t MinTankTempToUseForWaterHeathing = 30;
bool BoilerPump;          // true if boiler pump is on
bool FluxSensor;          // true if water flux is detected
char *MqttTpTopic;        //  mqtt_tptopic SolarThermostat/Tp
char *MqttTtTopic;        //  mqtt_tttopic SolarThermostat/Tt
char *MqttTfTopic;        // mqtt_tttopic SolarThermostat/Tf
char *MqttControlTopic;   // mqtt_cttopic SolarThermostat/control
char *MqttStatusTopic;    // SolarThermostat/status
char *MqttInfoTopic;      // SolarThermostat/status
char *otaurl;             // otaurl https://otaserver:8070/SolarThermostat.bin
bool forcePumpOn = false; // flag to force pump on
bool disableThermostat = false;

Mqtt mqtt;
WiFi wifi;
NvsParameters param;
Otafw otafw;
ds18x20_addr_t panel_sens[1];
ds18x20_addr_t tank_sens[1];
extern const uint8_t ca_crt_start[] asm("_binary_ca_crt_start");

void onNewCommand(char *s);
char *toLower(char *s);
void publishStatus();

void ProcessStdin()
{
    static char cmd[MAXCMDLEN];
    static uint8_t cmdlen = 0;
    int c = fgetc(stdin);
    if (c != EOF)
    {
        printf("%c", c);
        if (c == '\n')
        {
            cmd[cmdlen] = 0;
            onNewCommand(cmd);
            cmdlen = 0;
        }
        else
        {
            cmd[cmdlen++] = c;
            if (cmdlen == MAXCMDLEN - 1)
            {
                cmdlen = 0;
            }
        }
    }
}

void Ota(void *o)
{
    ESP_LOGI(TAG, "Starting OTA task");
    for (;;)
    {
        if (xEventGroupWaitBits(event_group, WIFI_CONNECTED_BIT | OTA_BIT, pdFALSE, pdTRUE, 1000 / portTICK_PERIOD_MS) == (WIFI_CONNECTED_BIT | OTA_BIT))
        {
            ESP_LOGI(TAG, "Starting OTA update");
            otafw.Check();
            xEventGroupClearBits(event_group, OTA_BIT);
        }
    }
}

void app_main(void)
{
    BinarySensor doorSwitch;
    doorSwitch.mqtt = &mqtt;
    param.load("doorStateTopic",&doorSwitch.mqttStateTopic,"door1");
    doorSwitch.pin = DOORSENSOR;

    BinarySensor stairpushbutton;
    doorswitch.mqtt = mqtt;
    doorswitch.pin = STAIRPUSHBUTTON;

    Switch stairlight;
    stairlight.mqtt = mqtt;
    stairlight.input = &stairpushbutton;

    // srand((unsigned int)esp_timer_get_time());
    event_group = xEventGroupCreate();

    // setup gpio
    gpioSetup();
    otafw.Init();

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set(TAG, ESP_LOG_DEBUG);

    // configure wifi
    wifiSetup();
    mqttSetup();

    publishStatus();

    while (true)
    {
        now = (esp_timer_get_time() / 1000);
        ProcessStdin();
        stairpushbutton.run();
        stairlight.run();
        doorsensor.run();
        blindsensor.run();
        vTaskDelay(1);
    }
}

char *toLower(char *s)
{
    for (char *p = s; *p; p++)
        *p = tolower(*p);
    return s;
}
