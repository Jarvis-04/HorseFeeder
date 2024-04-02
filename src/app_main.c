#include "app_main.h"
#include "global.h"
#include "stepper.h"
#include "usart.h"
#include "button.h"
#include "load_cell.h"
#include "pid.h"

int app_main(void) {
    systick_init();

    usart_init();
    printf("Usart Setup and Communicating\r\n");
    printf("Press y to Start System\r\n");
    while(1) {
        if (usart_readChar() == 'y') break;
    }
    printf("System Starting\r\n");

    button_init(ESTOPPIN, BUTTON_INTERUPT, PU, EXTI_FT);

    Stepper_TypeDef railStepper;
    railStepper.stepPin = STEPPIN;
    railStepper.dirPin = DIRPIN;
    railStepper.enPin = RAILENPIN;
    stepper_init(&railStepper);

    Stepper_TypeDef feed01Stepper;
    feed01Stepper.stepPin = STEPPIN;
    feed01Stepper.dirPin = DIRPIN;
    feed01Stepper.enPin = FEED01ENPIN;
    stepper_init(&feed01Stepper);

    Load_Cell_TypeDef load_cell;
    load_cell.clk = LOADCLK;
    load_cell.dt = LOADDT;
    load_cell_init(&load_cell);
    load_cell_tare(&load_cell);
    load_cell_power_down(&load_cell);
    load_cell_power_up(&load_cell);

    delay(1000);

    // while(1) {
    //     printf("Current Weight: %f\r\n", load_cell_get_grams(&load_cell, 1));
    //     delay(1000);
    // }

    // PID_TypeDef pid;
    // PID_init(&pid, 1, 0.5, 0.5);
    // PID_setSetPoint(&pid, 10);

    // while(1) {
    //     PID_compute(&pid, load_cell_get_grams(&load_cell, 1));
    // }

    // stepper_setSpeed(&railStepper, 150);
    // stepper_home(&railStepper);
    // stepper_move_to_location(&railStepper, 1000);
    // stepper_move_to_location(&railStepper, 500);
    // stepper_setDir(&feed01Stepper, STEPPER_CCW);
    // stepper_step(&feed01Stepper, 400);
    // stepper_setDir(&feed01Stepper, STEPPER_CW);
    // stepper_step(&feed01Stepper, 400);

    return 0;
}