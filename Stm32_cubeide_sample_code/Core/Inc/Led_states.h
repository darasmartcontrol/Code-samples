/*
 * Led_states.h
 *
 *  Created on: July 7, 2025
 *      Author: CentraL
 */

#ifndef INC_LED_STATES_H_
#define INC_LED_STATES_H_

#include <stdbool.h>
#define NUM_ledStates 6 // 0: led off, 1: dirt1, 2:dirt2, 3:flaw, 4:torn, 5:outCircuit
#define num_led 12    // number of leds


struct LedStates {
	int red, green, blue;
};
extern struct LedStates LED[NUM_ledStates]; // Declaration
// Function prototypes
void setup_param( struct LedStates *LED, int Red, int Green, int Blue); //   red, green, blue
void fadeNeopixels(void);
#endif /* INC_LED_STATES_H_ */
