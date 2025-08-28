/*
 * manage_OLED.c
 *
 *  Created on: Jul 19, 2025
 *      Author: CentraL
 */

#include "manage_OLED.h"
uint16_t x_offset = 0;
char indec_str[10];
uint16_t row = 0;

void decide_Oled() {
	if (Oled_state == 0) {
		SSD1306_Fill(0);
		SSD1306_UpdateScreen();
	}

	if (Oled_state == 4)
		Oled_torn(indec + 1);

	if (Oled_state == 5)
		Oled_Outcircuit();

	if (Oled_state == 123) {  // this state is conncted to dirt1, dirt2 or flaw
		if (blink_oledPre != blink_oled && elapsed == 1) {
			Oled_dirt_flaw(dirt_ind, cnt_dirt, flaw_ind, cnt_flaw);
			elapsed = 0;
		}
		counter_oled++;
		if (counter_oled > 20) {
			counter_oled = 0;
			elapsed = 1;
		}
	}
}

void Oled_Outcircuit() {
	SSD1306_Fill(0);
	SSD1306_GotoXY(0, 15);
	SSD1306_Puts("Out of circuit!", &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

void Oled_torn(uint8_t indec) {
	char indec_str[10]; // Buffer for the number (adjust size if needed)
	sprintf(indec_str, "%d", indec); // Convert index to string
	x_offset = 0;
	SSD1306_Fill(0);
	SSD1306_GotoXY(0, 0);
	SSD1306_Puts("Yarn Num: ", &Font_7x10, 1);
	int x_offset = 9 * 7 + 5; // 63 (text width) + 5 (gap)
	SSD1306_GotoXY(x_offset, 0); // Position next to the first string
	SSD1306_Puts(indec_str, &Font_7x10, 1);
	SSD1306_GotoXY(0, 15);
	SSD1306_Puts("break!", &Font_7x10, 1);
	SSD1306_UpdateScreen();
}

void Oled_dirt_flaw(volatile uint8_t dirt_ind[12], uint8_t cnt_dirt,
		volatile uint8_t dirt_flaw[12], uint8_t cnt_flaw) {

	blink_oledPre = blink_oled;
	x_offset = 0;
	SSD1306_Fill(0);
	SSD1306_GotoXY(0, 0);
	//////////////////////// Display "clean channels:"
	if (cnt_dirt > 0) {
		SSD1306_Puts("Clean channel(s):", &Font_7x10, 1);
		if (blink_oled == 0) {
			for (uint8_t i = 0; i < cnt_dirt; i++) {
				SSD1306_GotoXY(x_offset, 15);
				sprintf(indec_str, "%d", dirt_ind[i]);
				SSD1306_Puts(indec_str, &Font_7x10, 1);
				x_offset = (dirt_ind[i] > 9) ? x_offset + 2 * 7 : x_offset + 7;
				SSD1306_GotoXY(x_offset, 15);
				if (i < cnt_dirt - 1) {
					SSD1306_Puts(",", &Font_7x10, 1);
					x_offset += 7;
				}
				if (x_offset >= 17 * 7) {
					blink_oled = 1;
					i_cnt_dirt = i + 1;
					break;
				}
			}
		} else if (blink_oled == 1) {
			for (uint8_t i = i_cnt_dirt; i < cnt_dirt; i++) {
				SSD1306_GotoXY(x_offset, 15);
				sprintf(indec_str, "%d", dirt_ind[i]);
				SSD1306_Puts(indec_str, &Font_7x10, 1);
				x_offset = (dirt_ind[i] > 9) ? x_offset + 2 * 7 : x_offset + 7;
				SSD1306_GotoXY(x_offset, 15);
				if (i < cnt_dirt - 1) {
					SSD1306_Puts(",", &Font_7x10, 1);
					x_offset += 7;
				}
			}
			blink_oled = 0;
		}

		//////////////////////// Display Technical flaw
	} else if (cnt_flaw > 0) {
		if (blink_oled == 0) {
			SSD1306_GotoXY(x_offset, 0);
			SSD1306_Puts("Technical flaw!", &Font_7x10, 1);
			blink_oled = 1;
		} else if (blink_oled == 1) {
			SSD1306_GotoXY(x_offset, 0);
			SSD1306_Puts("Channel(s) Out:", &Font_7x10, 1);

			for (uint8_t i = 0; i < cnt_flaw; i++) {
				SSD1306_GotoXY(x_offset, 15);
				sprintf(indec_str, "%d", flaw_ind[i]);
				SSD1306_Puts(indec_str, &Font_7x10, 1);
				x_offset = (flaw_ind[i] > 9) ? x_offset + 2 * 7 : x_offset + 7;
				SSD1306_GotoXY(x_offset, 15);
				if (i < cnt_flaw - 1) {
					SSD1306_Puts(",", &Font_7x10, 1);
					x_offset += 7;
				}
				if (x_offset >= 17 * 7) {
					blink_oled = 2;
					i_cnt_flaw = i + 1;
					break;
				} else
					blink_oled = 0;
			}
		} else if (blink_oled == 2) {
			SSD1306_GotoXY(x_offset, 0);
			SSD1306_Puts("Channel(s) Out:", &Font_7x10, 1);
			for (uint8_t i = i_cnt_flaw; i < cnt_flaw; i++) {
				SSD1306_GotoXY(x_offset, 15);
				sprintf(indec_str, "%d", flaw_ind[i]);
				SSD1306_Puts(indec_str, &Font_7x10, 1);
				x_offset = (flaw_ind[i] > 9) ? x_offset + 2 * 7 : x_offset + 7;
				SSD1306_GotoXY(x_offset, 15);
				if (i < cnt_flaw - 1) {
					SSD1306_Puts(",", &Font_7x10, 1);
					x_offset += 7;
				}
			}
			blink_oled = 0;
		}
	}
	SSD1306_UpdateScreen();
}
