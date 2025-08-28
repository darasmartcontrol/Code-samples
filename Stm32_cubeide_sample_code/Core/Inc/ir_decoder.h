#ifndef IR_V1_H
#define IR_V1_H

#include "main.h"
#include <stdint.h>

#define BUFFER_SIZE 50 // Adjust based on expected max pulses

// Public function prototypes
void IR_Init(void);
//void IR_PrintBuffer(void);
uint32_t IR_GetIRData(void); // New function to get decoded data
uint8_t IR_GetCommand(void); // New function to get command byte
#define MAX_PULSES 34 // For 34 data bits


#endif /* IR_V1_H */
