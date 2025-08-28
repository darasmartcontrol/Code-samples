/*
 * yarn_states.h
 *
 *  Created on: July 7, 2025
 *      Author: CentraL
 */
#ifndef INC_YARN_STATES_H_
#define INC_YARN_STATES_H_

#define NUM_YARN 12
#include <stdint.h>
#include <stdbool.h>
extern bool outCircuit;
extern int cc;


struct YarnStates {
    bool torn;
    bool dirt1;
    bool dirt2;
    bool flaw;
    int per, count_total, count_yarn, maxper, yarn_chance;
    int maxim;
    int minim;
    int read_sens;
    int read_sensPre;
    int threshold;
    int thresh;
    int ledState;
    int ledStatePre;
};

extern struct YarnStates yarn[NUM_YARN]; // Declaration

// Function prototypes
void calibration(struct YarnStates* yarn);
void resetCalibration(struct YarnStates* yarn);
void comp_thresh_perturb(struct YarnStates* yarn);
void yarn_state(struct YarnStates* yarn);
#endif /* INC_YARN_STATES_H_ */
