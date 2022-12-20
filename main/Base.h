class Base {
    Mqtt *mqtt;
    char *mqttStateTopic;
    gpio_num_t pin;
    void run();
    bool state;
    void publish();
}