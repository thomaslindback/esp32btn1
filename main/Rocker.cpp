/*
*/

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_system.h"
#include "Rocker.h"

static const char * TAG = "Rocker.cpp";

extern Rocker gRocker;

Rocker::Rocker() {}

Rocker::Rocker(gpio_num_t gpioNum) {
    mGPIONum = gpioNum;
}

int32_t Find_Rocker_Via_Pin(gpio_num_t gpioNum)
{
    /*for (int i = 0; i < ROCKER_NUMBER; i++)
    {
        if (gRockers[i].GetGPIONum() == gpioNum)
        {
            return i;
        }
    }*/
    return -1;
}
void IRAM_ATTR rocker_isr_handler(void * arg)
{
    /*uint32_t gpio_num = (uint32_t) arg;
    int32_t idx       = Find_Rocker_Via_Pin((gpio_num_t) gpio_num);
    if (idx == -1)
    {
        return;
    }*/
    BaseType_t taskWoken = pdFALSE;
    xTimerStartFromISR(gRocker.mrockerTimer,
                       &taskWoken); // If the timer had already been started ,restart it will reset its expiry time
}

esp_err_t Rocker::Init() {
    return Init(mGPIONum);
}

esp_err_t Rocker::Init(gpio_num_t gpioNum)
{
    esp_err_t ret = ESP_OK;

    mGPIONum = gpioNum;
    //  zero-initialize the config structure.
    gpio_config_t io_conf = {};
    // interrupt of falling edge
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    // bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = 1ULL << gpioNum;
    // set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    // enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    gpio_config(&io_conf);
    gpio_set_level(gpioNum, 1);
    // hook isr handler for specific gpio pin
    ret = gpio_isr_handler_add(gpioNum, rocker_isr_handler, (void *) gpioNum);
    ESP_RETURN_ON_ERROR(ret, TAG, "gpio_isr_handler_add failed: %s", esp_err_to_name(ret));

    mrockerTimer = xTimerCreate("BtnTmr",               // Just a text name, not used by the RTOS kernel
                                pdMS_TO_TICKS(50),      // timer period
                                false,                  // no timer reload (==one-shot)
                                (void *) (int) gpioNum, // init timer id = gpioNum index
                                TimerCallback           // timer callback handler (all buttons use
                                                        // the same timer cn function)
    );

    return ESP_OK;
}

void Rocker::TimerCallback(TimerHandle_t xTimer)
{
    // Get the button index of the expired timer and call button event Handler.
    uint32_t gpio_num = (uint32_t) pvTimerGetTimerID(xTimer);
    //GetAppTask().ButtonEventHandler(gpio_num, APP_BUTTON_PRESSED);
    ESP_LOGI(TAG, "TimerCallback %lu", gpio_num);
}
