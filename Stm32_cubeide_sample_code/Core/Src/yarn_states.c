/*
 * yarn_states.c
 *
 *  Created on: July 7, 2025
 *      Author: CentraL
 */

#include "yarn_states.h"
#include <stdbool.h>
const int res = 12; // Global constant

struct YarnStates yarn[NUM_YARN] = { 0 }; // Definition

// Calibration function
void calibration(struct YarnStates *yarn) {
	yarn->maxim =
			(yarn->read_sens > yarn->maxim) ? yarn->read_sens : yarn->maxim;
	yarn->minim =
			(yarn->read_sens < yarn->minim) ? yarn->read_sens : yarn->minim;
	yarn->per = yarn->maxim - yarn->minim;
	yarn->maxper = (yarn->per > yarn->maxper) ? yarn->per : yarn->maxper;
	yarn->thresh = yarn->maxper;
}

// Reset Calibration function
void resetCalibration(struct YarnStates *yarn) {
	yarn->maxim = 0;
	yarn->minim = (1 << res) - 1; // Reset to max value
	yarn->per = 0;
	yarn->thresh = (yarn->thresh > yarn->maxper) ? yarn->thresh : yarn->maxper;
	yarn->maxper = 0;
}

// Read Threshold Perturb function
void comp_thresh_perturb(struct YarnStates *yarn) {
	if (yarn->read_sens <= 900)
		yarn->threshold = yarn->thresh + 5;
	else if (yarn->read_sens <= 1500)
		yarn->threshold = yarn->thresh + 8;
	else
		yarn->threshold = yarn->thresh + 10;

	yarn->maxim =
			(yarn->read_sens > yarn->maxim) ? yarn->read_sens : yarn->maxim;
	yarn->minim =
			(yarn->read_sens < yarn->minim) ? yarn->read_sens : yarn->minim;
	yarn->per = yarn->maxim - yarn->minim;
	yarn->maxper = (yarn->per > yarn->maxper) ? yarn->per : yarn->maxper;
	yarn->count_total++;
	if (yarn->per > yarn->threshold)
		yarn->count_yarn++; // Count perturbations above threshold
	yarn->read_sensPre = yarn->read_sens;
}

// Yarn State function
void yarn_state(struct YarnStates *yarn) {
	yarn->yarn_chance = (yarn->count_yarn * 100.0) / yarn->count_total;
	yarn->torn = 0;
	yarn->dirt1 = 0;
	yarn->dirt2 = 0;
	yarn->flaw = 0;
	yarn->ledState = 0;
	yarn->ledStatePre = 0;

	if (yarn->read_sens <= 100 || yarn->read_sens == 4095) {
		yarn->flaw = 1;
		yarn->ledState = 3;
	} else if (yarn->read_sens <= 500 && yarn->read_sens > 100) {
		yarn->dirt2 = 1;
		yarn->ledState = 2;
	} else if (yarn->yarn_chance < 10) {
		yarn->torn = 1;
		yarn->ledState = 4;
	} else if (yarn->read_sens <= 1300 && yarn->read_sens > 500) {
		yarn->dirt1 = 1;
		yarn->ledState = 1;
	}
	if (outCircuit == 1) {
		yarn->ledState = 5;
	}
	yarn->maxim = 0;
	yarn->minim = (1 << res) - 1;
	yarn->count_yarn = 0;
	yarn->count_total = 0;
}
