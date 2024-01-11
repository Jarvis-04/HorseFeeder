#include "app_main.h"
#include "header.h"
#include "stepper.h"
#include "gpio.h"

int app_main(void) {
    uint32_t step = PIN('A', 5);
    uint32_t dir = PIN('A', 9);

    stepper_init(step, dir, 40000);

    // stepper_step(step, dir, 200);
    stepper_test(step, dir);

    return 0;
}