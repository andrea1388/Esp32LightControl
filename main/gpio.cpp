inline void gpioSetup() {
    gpio_set_direction(GPIO_PUMP, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_FPPUMP, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_TANKPUMP, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_BUTTON, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_BOILERPUMP, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_FLUX, GPIO_MODE_INPUT);
    

    gpio_pullup_en(GPIO_BUTTON);
    gpio_set_level(GPIO_LED,0);
    gpio_set_level(GPIO_PUMP,0);

}