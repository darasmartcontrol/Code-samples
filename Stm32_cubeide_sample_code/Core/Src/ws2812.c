// Peripheral usage

#include "tim.h"
#include "ws2812.h"

extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_tim4_ch4;

//
#define PWM_HI (2)
#define PWM_LO (1)

// LED color buffer
extern uint8_t rgb_arr[3 * num_led];

// Define the buffers timers
extern uint16_t wr_buf[WR_BUF_LEN];

static inline uint8_t scale8(uint8_t x, uint8_t scale) {
	return ((uint16_t) x * scale) >> 8;
}

// Set a single color (RGB) to index
void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
	rgb_arr[3 * index] = scale8(g, 0xB0);	// g;
	rgb_arr[3 * index + 1] = r;
	rgb_arr[3 * index + 2] = scale8(b, 0xF0); // b;
}
void flush() {
	for (int i = 0; i < 3 * num_led; i++)
		rgb_arr[i] = 0;
	for (int i = 0; i < WR_BUF_LEN; i++)
		wr_buf[i] = 0;
}
// Set all colors to RGB
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
	for (uint_fast8_t i = 0; i < num_led; ++i)
		led_set_RGB(i, r, g, b);
}
void fillBuffer(uint8_t index) {
	uint8_t g = rgb_arr[index * 3];
	uint8_t r = rgb_arr[index * 3 + 1];
	uint8_t b = rgb_arr[index * 3 + 2];

	// Green (bits 0-23: 8 bits G, 8 R, 8 B)

	for (uint8_t i = 0; i < 8; i++) {
		wr_buf[index * 24 + i] = ((g << i) & 0x80) ? PWM_HI : PWM_LO;
	}
	// Red
	for (uint8_t i = 0; i < 8; i++) {
		wr_buf[index * 24 + 8 + i] = ((r << i) & 0x80) ? PWM_HI : PWM_LO;
	}
	// Blue
	for (uint8_t i = 0; i < 8; i++) {
		wr_buf[index * 24 + 16 + i] = ((b << i) & 0x80) ? PWM_HI : PWM_LO;
	}
}

void fillBufferAll() {

	for (uint8_t index = 0; index < num_led; index ++) {
		uint8_t g = rgb_arr[index * 3];
		uint8_t r = rgb_arr[index * 3 + 1];
		uint8_t b = rgb_arr[index * 3 + 2];

		// Green (bits 0-23: 8 bits G, 8 R, 8 B)

		for (uint8_t i = 0; i < 8; i++) {
			wr_buf[index * 24 + i] = ((g << i) & 0x80) ? PWM_HI : PWM_LO;
		}
		// Red
		for (uint8_t i = 0; i < 8; i++) {
			wr_buf[index * 24 + 8 + i] = ((r << i) & 0x80) ? PWM_HI : PWM_LO;
		}
		// Blue
		for (uint8_t i = 0; i < 8; i++) {
			wr_buf[index * 24 + 16 + i] = ((b << i) & 0x80) ? PWM_HI : PWM_LO;
		}
	}

}

void led_render() {

	if (hdma_tim4_ch4.State != HAL_DMA_STATE_READY)
		return;

	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_4, (uint32_t*) wr_buf,
	WR_BUF_LEN);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_4);
}

// Function to create the rainbow effect
void rainbow(uint8_t wait) {
	uint16_t j;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t WheelPos;

	for (j = 0; j < 256; j++) { // Cycle through all colors
		// Remove 'i' from the phase shift
		WheelPos = j & 255;
		if (WheelPos < 85) {
			r = WheelPos * 3;
			g = 255 - WheelPos * 3;
			b = 0;
		} else if (WheelPos < 170) {
			WheelPos -= 85;
			r = 255 - WheelPos * 3;
			g = 0;
			b = WheelPos * 3;
		} else {
			WheelPos -= 170;
			r = 0;
			g = WheelPos * 3;
			b = 255 - WheelPos * 3;
		}
		for (uint16_t i = 0; i < num_led; i++) {
			// Update LEDs
			led_set_RGB(i, r, g, b);
		}
		fillBufferAll();
		led_render();
		HAL_Delay(wait);
	}
}

