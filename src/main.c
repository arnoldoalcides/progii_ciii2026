#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led_strip.h"

static const char *TAG = "blink_app";

//  RGB LED GPIO2
#define RGB_GPIO GPIO_NUM_2

static led_strip_handle_t pixel;
static float brightness = 0.2f; // 0.0 to 1.0 

static void pixel_init(void)
{
    // Configure the WS2812B LED hardware parameters
    led_strip_config_t strip_config = {
        .strip_gpio_num = RGB_GPIO,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags.invert_out = false,
    };
    
    // Configure the RMT peripheral
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,   // 10 MHz resolution
        .flags.with_dma = false,             //  ESP32 RMT no DMA support
    };
    
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &pixel));
    led_strip_clear(pixel); // LED starts off
}

static void pixel_set(uint8_t r, uint8_t g, uint8_t b)
{
    //  brightness scaling 
    led_strip_set_pixel(pixel, 0, r * brightness, g * brightness, b * brightness);
    led_strip_refresh(pixel);
}

void app_main(void)
{
    pixel_init();
    ESP_LOGI(TAG, "RGB LED Initialization Complete");

    while (true) {
        // GREEN Light
        pixel_set(0, 255, 0);
        ESP_LOGI(TAG, "Light: GREEN");
        vTaskDelay(pdMS_TO_TICKS(4000)); // Wait 4 seconds

        // YELLOW Light
        pixel_set(255, 150, 0);
        ESP_LOGI(TAG, "Light: YELLOW");
        vTaskDelay(pdMS_TO_TICKS(2000)); // Wait 2 seconds


        // RED Light
        pixel_set(255, 0, 0);
        ESP_LOGI(TAG, "Light: RED");
        vTaskDelay(pdMS_TO_TICKS(4000)); // Wait 4 seconds
    }
}