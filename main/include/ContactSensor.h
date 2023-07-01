#pragma once

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

class ContactSensor {
public:
    ContactSensor();
    ContactSensor(gpio_num_t gpioNum);

    esp_err_t Init();
    esp_err_t Init(gpio_num_t gpioNum);

    inline gpio_num_t GetGPIONum();
    static void TimerCallback(TimerHandle_t xTimer);

    friend void IRAM_ATTR contact_sensor_isr_handler(void * arg);
private:
    gpio_num_t mGPIONum;
    TimerHandle_t mContactSensorTimer; // FreeRTOS timers used for debouncing rockers
};

inline gpio_num_t ContactSensor::GetGPIONum()
{
    return mGPIONum;
}