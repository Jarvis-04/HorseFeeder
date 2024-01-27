#include "app_main.h"
#include "header.h"
#include "stepper.h"
#include "gpio.h"
#include "exti.h"

int app_main(void) {
    uint16_t step = PIN('A', 5);
    uint16_t dir = PIN('A', 9);
    uint16_t button = PIN('C', 13);

    gpio_init(button, GPIO_MODE_INPUT);
    GPIOC->PUPDR |= (0b01 << 26); // set pull up
    exti_setup();

    Stepper_TypeDef stepper;
    stepper.stepPin = step;
    stepper.dirPin = dir;

    stepper_init(&stepper);
    stepper_setDir(&stepper, STEPPER_CW);

    bool currentDir = false;
    for (int i=0; i<10; i++) {
        stepper_step(&stepper, 200);
        currentDir = !currentDir;
        stepper_setDir(&stepper, currentDir);
    }

    return 0;
}