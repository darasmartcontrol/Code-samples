#ifndef _LED_DRIVER_WS2812
#define _LED_DRIVER_WS2812
#include "main.h"
#include "Led_states.h"

#define BITS_PER_LED 24             // 24 bits per LED (GRB)
#define RESET_BITS 224              // 224 bits = 280µs (224 * 1.25µs)
#define WR_BUF_LEN (num_led * BITS_PER_LED + RESET_BITS)  // 12*24 + 224


void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
void led_render();
void rainbow(uint8_t wait);
void flush(void);
void fillBuffer(uint8_t led);
void fillBufferAll(void);
#endif
