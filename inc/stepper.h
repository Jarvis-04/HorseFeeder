#ifndef STEPPER_H_INCLUDED
#define STEPPER_H_INCLUDED

#include "global.h"

#define PULSE_WIDTH 10
// One roation moves the bed about 44mm

typedef struct stepper {
    volatile uint16_t stepPin;
    volatile uint16_t dirPin;
    volatile uint32_t stepTime;
    volatile uint32_t location;
    volatile bool direction;
}Stepper_TypeDef;

enum {STEPPER_CW, STEPPER_CCW};

void stepper_init(Stepper_TypeDef *stepper);

void stepper_setSpeed(Stepper_TypeDef* stepper, int rpm);

void stepper_setDir(Stepper_TypeDef *stepper, bool dir);

void stepper_step(Stepper_TypeDef *stepper, uint32_t numSteps);

void stepper_move_to_location(Stepper_TypeDef* stepper, uint32_t location_steps);

void stepper_home(Stepper_TypeDef *stepper);

void stepper_eStop();

void stepper_resetEStop();

#endif