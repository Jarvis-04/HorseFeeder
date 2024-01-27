#ifndef STEPPER_H_INCLUDED
#define STEPPER_H_INCLUDED

#include "header.h"

typedef struct stepper {
    volatile uint16_t stepPin;
    volatile uint16_t dirPin;
    volatile uint32_t stepTime;
}Stepper_TypeDef;

enum {STEPPER_CW, STEPPER_CCW};

void stepper_init(Stepper_TypeDef *stepper);

void stepper_setDir(Stepper_TypeDef *stepper, bool dir);

void stepper_step(Stepper_TypeDef *stepper, uint32_t numSteps);

void stepper_stepAccel(Stepper_TypeDef *stepper, uint32_t numSteps);

void stepper_eStop();

void stepper_resetEStop();

#endif