#include "app_main.h"
#include "header.h"
#include "stepper.h"
#include "gpio.h"

int app_main(void) {
    uint16_t step = PIN('A', 5);
    uint16_t dir = PIN('A', 9);

    Stepper_TypeDef stepper;
    stepper.stepPin = step;
    stepper.dirPin = dir;

    stepper_init(&stepper);
    stepper_setDir(&stepper, STEPPER_CW);

    bool currentDir = false;
    for (int i=0; i<10; i++) {
        stepper_stepAccel(&stepper, 1600);
        currentDir = !currentDir;
        stepper_setDir(&stepper, currentDir);
    }

    return 0;
}