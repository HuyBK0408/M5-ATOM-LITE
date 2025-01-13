#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdlib.h>

#include "led/led.h"

#define BUTTON_GPIO 39 // Định nghĩa chân nút nhấn

static void button_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    gpio_config(&io_conf);
}

void app_main(void)
{
    // Initialize RMT peripheral for WS2812
    ws2812_rmt_init();
   
    button_init();
    
    // Allocate memory for LED data
    int last_button_state = 1;
  
    while (1)
    {
        // Đọc trạng thái nút nhấn
        int button_state = gpio_get_level(BUTTON_GPIO);

        // Kiểm tra xem nút có được nhấn không (falling edge)
        if (button_state == 0 && last_button_state == 1)
        {
            // Tạo màu ngẫu nhiên bằng cách kết hợp các giá trị R, G, B
            uint32_t random_color = (rand() % 256 << 16) | // Red
                                    (rand() % 256 << 8) |  // Green
                                    (rand() % 256);        // Blue
            set_led_color(random_color);
        }

        last_button_state = button_state;
        vTaskDelay(pdMS_TO_TICKS(20)); // Debounce delay
    }
}
