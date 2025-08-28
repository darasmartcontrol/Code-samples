/*
 * manage_OLED.h
 *
 *  Created on: Jul 19, 2025
 *      Author: CentraL
 */

#ifndef INC_MANAGE_OLED_H_
#define INC_MANAGE_OLED_H_

#include "fonts.h"
#include "ssd1306.h"
#include <stdint.h>
#include <stdbool.h>  //For bool
#include <stdio.h> // For sprintf

extern bool elapsed ;
extern uint8_t indec;
extern uint8_t cnt_flaw;
extern uint8_t cnt_dirt;
extern uint8_t Oled_statePre , Oled_state;
extern volatile uint8_t flaw_ind[12] ;
extern volatile uint8_t dirt_ind[12] ;
extern uint8_t blink_oled;
extern uint8_t blink_oledPre;
extern uint8_t counter_oled;
extern uint8_t i_cnt_dirt;
extern uint8_t i_cnt_flaw;

void Oled_Outcircuit(void);
void Oled_torn(uint8_t indec);
void Oled_dirt_flaw(volatile uint8_t dirt_ind[12],uint8_t cnt_dirt,volatile uint8_t dirt_flaw[12],uint8_t cnt_flaw);
void decide_Oled(void);

#endif /* INC_MANAGE_OLED_H_ */
