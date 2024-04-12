#ifndef STEPPER_H_INCLUDED
#define STEPPER_H_INCLUDED

#include "global.h"
#include "pid.h"
#include "load_cell.h"

#define PULSE_WIDTH 10
// One roation moves the bed about 44mm

// Stepper object
typedef struct stepper {
    volatile uint16_t stepPin;
    volatile uint16_t dirPin;
    volatile uint16_t enPin;
    volatile uint32_t stepTime;
    volatile uint32_t location;
    volatile bool direction;
    volatile uint8_t microStep;
}Stepper_TypeDef;

enum {STEPPER_CW, STEPPER_CCW};

void stepper_init(Stepper_TypeDef *stepper);

void stepper_setSpeed(Stepper_TypeDef* stepper, int rpm);

void stepper_setDir(Stepper_TypeDef *stepper, bool dir);

void stepper_step(Stepper_TypeDef *stepper, uint32_t numSteps);

void stepper_start(Stepper_TypeDef *stepper);

void stepper_stop(Stepper_TypeDef *stepper);

void stepper_move_to_location(Stepper_TypeDef* stepper, uint32_t location_steps);

void stepper_pid(Stepper_TypeDef *stepper, PID_TypeDef *pid, Load_Cell_TypeDef *input, float target);

void stepper_purge(Stepper_TypeDef *stepper);

void stepper_home(Stepper_TypeDef *stepper);

void stepper_enable(Stepper_TypeDef *stepper);

void stepper_disable(Stepper_TypeDef *stepper);

void stepper_eStop();

void stepper_resetEStop();

#endif