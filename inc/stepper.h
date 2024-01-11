#ifndef STEPPER_H_INCLUDED
#define STEPPER_H_INCLUDED

#include "header.h"

enum {STEPPER_CW, STEPPER_CCW};

void stepper_init(uint32_t step, uint32_t dir, uint32_t baseStepTime);

void stepper_step(uint32_t step, uint32_t dir, uint32_t numSteps);

void stepper_test(uint32_t step, uint32_t dir);

#endif