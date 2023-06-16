/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_chip_info.h"
#include "freertos/queue.h"
#include "esp_flash.h"
#include "Rocker.h"

static const char * TAG = "hello_world_main.cpp";

#define ROCKER_GPIO_NUM ((gpio_num_t) 27) // Left button on M5Stack
Rocker gRocker;

extern "C" void app_main(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    esp_err_t err;
    // Initialize the buttons.
    err = gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Rocker isr_install failed: %s", esp_err_to_name(err));
    }

    printf("Initialize Rocker on pin %u\n", (int)ROCKER_GPIO_NUM);
    gRocker = Rocker(ROCKER_GPIO_NUM);
    gRocker.Init();

    int cnt = 0;
    while(1) {
        printf("cnt: %d\n", cnt++);
        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}
