#pragma once

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"

static const char * PIN_TAG = "Pin.cpp";

class Pin {
public:
    Pin();
    Pin(gpio_num_t gpioNum);

    esp_err_t Init();
    esp_err_t Init(gpio_num_t gpioNum);

    inline gpio_num_t GetGPIONum();
    inline esp_err_t Set(uint32_t v);
    inline esp_err_t Toggle();
private:
    gpio_num_t mGPIONum;
};

inline gpio_num_t Pin::GetGPIONum()
{
    return mGPIONum;
}

inline esp_err_t Pin::Set(uint32_t v) {
    ESP_LOGI(PIN_TAG, "Set %d", (int)mGPIONum);

    return gpio_set_level(mGPIONum, v);
}

inline esp_err_t Pin::Toggle() {
    ESP_LOGI(PIN_TAG, "Toggle %d", (int)mGPIONum);

    int v = gpio_get_level(mGPIONum);
    if(v == 1) {
        ESP_LOGI(PIN_TAG, "\tSet value = %d", (int)v);
        return gpio_set_level(mGPIONum, 0);
    } else {
        ESP_LOGI(PIN_TAG, "\tSet value = %d", (int)v);
        return gpio_set_level(mGPIONum, 1);
    }
}