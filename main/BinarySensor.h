class BinarySensor {
    mqtt *mqtt;
    char *mqttStateTopic;
    gpio_num_t pin;
    void run();
    uint8_t debounceTime;
    int64_t tLastReading;
}