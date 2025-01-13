#ifndef __LED_H__
#define __LED_H__
#include <stdio.h>
#include "driver/rmt.h"
#include "driver/gpio.h"

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color_t;

void ws2812_rmt_init(void);
void ws2812_write_leds(uint8_t *pixels, uint32_t num_pixels);
void set_led_color(uint32_t hex_color);
#endif // __LED_H__
