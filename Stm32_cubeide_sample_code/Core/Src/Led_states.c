/*
 * Led_states.c
 *
 *  Created on: July 7, 2025
 *      Author: CentraL
 */

#include "Led_states.h"
#include <stdbool.h>
#include "ws2812.h"

extern uint16_t wr_buf[WR_BUF_LEN];
struct LedStates LED[NUM_ledStates] = { 0 };
void setup_param(struct LedStates *LED, int Red, int Green, int Blue) {
	LED->red = Red;
	LED->green = Green;
	LED->blue = Blue;
}

void fadeNeopixels(void) {
	int n = 0;
	for (int i = 0; i < 512; i++) {
		uint8_t brightness;
		if (i >= 256) {
			brightness = i - n - 1;
			n += 2;
		} else {
			brightness = i;
		}

		// Scale RGB values based on brightness (0-255)
		uint8_t red = (200 * brightness) / 255;    // Scale red
		uint8_t green = (200 * brightness) / 255; // Scale green
		uint8_t blue = 0;   // Scale blue

		// Set brightness for strip1 (LEDs 0 and 1)
		led_set_all_RGB(red, green, blue);  // LED 0
		fillBufferAll();
		led_render(); // Update Neopixels

		// Add delay to control fade speed (e.g., 5 ms per step)
		HAL_Delay(5); // Adjust as needed
	}
}
