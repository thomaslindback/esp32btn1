/*
*/

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_system.h"
#include "Pin.h"

extern Pin gPinRelay;

Pin::Pin() {}

Pin::Pin(gpio_num_t gpioNum) {
    mGPIONum = gpioNum;
}

esp_err_t Pin::Init() {
    return Init(mGPIONum);
}

esp_err_t Pin::Init(gpio_num_t gpioNum) {
    mGPIONum = gpioNum;
    //  zero-initialize the config structure.
    gpio_config_t io_conf = {};
    // interrupt of falling edge
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = 1ULL << gpioNum;
    // set as input mode
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT;

    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;

    gpio_config(&io_conf);
    gpio_set_level(gpioNum, 0);
 
    return ESP_OK;
}
