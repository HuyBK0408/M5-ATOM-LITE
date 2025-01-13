#include "led.h"

#define WS2812_T0H_NS (350)
#define WS2812_T0L_NS (1000)
#define WS2812_T1H_NS (1000)
#define WS2812_T1L_NS (350)

#define LED_RMT_TX_CHANNEL RMT_CHANNEL_0
#define LED_GPIO_PIN       27         
#define LED_COUNT         16   

void ws2812_rmt_init(void)
{
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(LED_GPIO_PIN, LED_RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
}

void ws2812_write_leds(uint8_t *pixels, uint32_t num_pixels)
{
    // Convert RGB values to RMT format
    size_t size = num_pixels * 3 * 8 * sizeof(rmt_item32_t);
    rmt_item32_t* rmt_items = (rmt_item32_t*) malloc(size);
    
    // Convert RGB values to RMT pulses
    int rmtIndex = 0;
    for (uint32_t i = 0; i < num_pixels * 3; i++) {
        uint8_t cur = pixels[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (cur & (1 << (7 - j))) {
                // Logical 1
                rmt_items[rmtIndex].level0 = 1;
                rmt_items[rmtIndex].duration0 = WS2812_T1H_NS / 25; // Convert ns to RMT tick
                rmt_items[rmtIndex].level1 = 0;
                rmt_items[rmtIndex].duration1 = WS2812_T1L_NS / 25;
            } else {
                // Logical 0
                rmt_items[rmtIndex].level0 = 1;
                rmt_items[rmtIndex].duration0 = WS2812_T0H_NS / 25;
                rmt_items[rmtIndex].level1 = 0;
                rmt_items[rmtIndex].duration1 = WS2812_T0L_NS / 25;
            }
            rmtIndex++;
        }
    }

    ESP_ERROR_CHECK(rmt_write_items(LED_RMT_TX_CHANNEL, rmt_items, num_pixels * 3 * 8, true));
    free(rmt_items);
}


void set_led_color(uint32_t hex_color)
{
    uint8_t *pixels = (uint8_t*)malloc(LED_COUNT * 3);
    uint8_t red = (hex_color >> 16) & 0xFF;
    uint8_t green = (hex_color >> 8) & 0xFF;
    uint8_t blue = hex_color & 0xFF;

    for (int i = 0; i < LED_COUNT; i++) {
        pixels[i * 3 + 0] = red;
        pixels[i * 3 + 1] = green;
        pixels[i * 3 + 2] = blue;
    }
    ws2812_write_leds(pixels, LED_COUNT);
    free(pixels);
}
