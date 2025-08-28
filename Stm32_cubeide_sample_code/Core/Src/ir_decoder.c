
#include "ir_decoder.h"
#include <string.h>
#include "main.h"

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	return ch;
}

volatile uint32_t capture1 = 0, capture2 = 0;
volatile uint8_t captureState = 0; // 0: wait for falling, 1: wait for rising, 2: wait for falling (decoding)
volatile uint16_t highEdgeCount = 0;
volatile uint16_t lowEdgeCount = 0;
volatile uint32_t lastEdgeTime = 0;
volatile uint8_t signalActive = 0;
volatile uint32_t lowPulseBuffer[MAX_PULSES];
volatile uint32_t highPulseBuffer[MAX_PULSES];
volatile uint32_t irData = 0; // Decoded 32-bit IR data
volatile uint8_t irState = 0; // 0: idle, 1: after leader low, 2: decoding
volatile uint8_t irBitCount = 0; // Bit counter for decoding
extern volatile uint8_t command; // Extracted command byte
extern volatile uint8_t newIRDataFlag;

void IR_Init(void) {
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
		uint32_t capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		uint32_t currentTime = capture * 10; // Convert to µs with PSC=1700-1

//        if (currentTime - lastEdgeTime < 1000) { // Debounce threshold
//            TIM3->SR = ~TIM_SR_CC4IF;
//            return;
//        }

		if (captureState == 0) {
			capture1 = capture;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4,
					TIM_ICPOLARITY_RISING);
			lowEdgeCount++;
			captureState = 1;
			if (!signalActive) {
				signalActive = 1;
				irState = 0;
				irBitCount = 0;
				irData = 0;
				highEdgeCount = 0;
				lowEdgeCount = 0;
			}
		} else if (captureState == 1) {
			capture2 = capture;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4,
					TIM_ICPOLARITY_FALLING);
			highEdgeCount++;

			uint32_t pulseWidth;
			if (capture2 >= capture1) {
				pulseWidth = (capture2 - capture1) * 10;
			} else {
				pulseWidth = (0xFFFF - capture1 + capture2 + 1) * 10;
			}

			if (highEdgeCount - 1 < MAX_PULSES && signalActive) {
				highPulseBuffer[highEdgeCount - 1] = pulseWidth; // Store high pulse
			}

			// State transition for leader low and high
			if (irState == 0 && pulseWidth > 8000 && pulseWidth < 10000) {
				irState = 1;
			}
			capture1 = capture2;
			captureState = 2;
		} else { // captureState == 2
			capture2 = capture;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4,
					TIM_ICPOLARITY_RISING);
			lowEdgeCount++;

			uint32_t pulseWidth;
			if (capture2 >= capture1) {
				pulseWidth = (capture2 - capture1) * 10;
			} else {
				pulseWidth = (0xFFFF - capture1 + capture2 + 1) * 10;
			}

			if (lowEdgeCount - 1 < MAX_PULSES && signalActive) {
				lowPulseBuffer[lowEdgeCount - 1] = pulseWidth; // Store low pulse
			}

			if (irState == 1 && pulseWidth > 4000 && pulseWidth < 5000) {
				irState = 2;
			} else if (irState == 2 && irBitCount < 32 && highEdgeCount > 1) {
				if (pulseWidth > 500 && pulseWidth < 800) { // 560µs high (0 bit)
					irData = (irData << 1) | 0;
				} else if (pulseWidth > 1500 && pulseWidth < 2000) { // 1690µs high (1 bit)
					irData = (irData << 1) | 1;
				}
				irBitCount++;
				if (irBitCount == 32) {
					uint8_t address = (irData >> 24) & 0xFF;
					uint8_t invAddress = (irData >> 16) & 0xFF;
					command = (irData >> 8) & 0xFF;
					uint8_t invCommand = irData & 0xFF;
					if (address == (uint8_t) ~invAddress
							&& command == (uint8_t) ~invCommand) {
						newIRDataFlag = 1; // Valid data, trigger print
					}
				}
			} else if (irBitCount == 32 && pulseWidth > 30000) {
				newIRDataFlag = 1; // End of frame after 32 bits
				signalActive = 0;
				irState = 0;
				irBitCount = 0;
				highEdgeCount = 0;
				lowEdgeCount = 0;
			}

			capture1 = capture2;
			captureState = 1;
		}

		lastEdgeTime = currentTime;
		TIM3->SR = ~TIM_SR_CC4IF;
	}
}


uint32_t IR_GetIRData(void) {
	return irData;
}

uint8_t IR_GetCommand(void) {
	return command;
}

void IR_Reset(void) {
	newIRDataFlag = 0;
	irData = 0; // Adjust based on your variable names
	irState = 0;
	irBitCount = 0;
	memset(lowPulseBuffer, 0, sizeof(lowPulseBuffer));
	memset(highPulseBuffer, 0, sizeof(highPulseBuffer));
}
