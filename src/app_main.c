#include "app_main.h"
#include "global.h"
#include "stepper.h"
#include "usart.h"
#include "button.h"
#include "load_cell.h"
#include "pid.h"

int app_main(void) {
    // Init systick and usart and ask user if they want to begin
    systick_init();

    usart_init();
    printf("Usart Setup and Communicating\r\n");
    printf("Press y to Start System\r\n");
    while(1) {
        if (usart_readChar() == 'y') break;
    }
    printf("System Starting\r\n");

    // ESTOP Init
    button_init(ESTOPPIN, BUTTON_INTERUPT, PU, EXTI_FT);

    // Rail Stepper Init
    Stepper_TypeDef railStepper;
    railStepper.stepPin = STEPPIN;
    railStepper.dirPin = DIRPIN;
    railStepper.enPin = RAILENPIN;
    railStepper.microStep = 1;
    stepper_init(&railStepper);

    // Feeder01 Init
    Stepper_TypeDef feed01Stepper;
    feed01Stepper.stepPin = STEPPIN;
    feed01Stepper.dirPin = DIRPIN;
    feed01Stepper.enPin = FEED01ENPIN;
    feed01Stepper.microStep = 16;
    stepper_init(&feed01Stepper);

    // Load Cell Init
    Load_Cell_TypeDef load_cell;
    load_cell.clk = LOADCLK;
    load_cell.dt = LOADDT;
    load_cell_init(&load_cell);
    load_cell_tare(&load_cell);
    load_cell_power_down(&load_cell);
    load_cell_power_up(&load_cell);

    // PID Init
    PID_TypeDef pid;
    PID_init(&pid, 10, 0.001, 0.001, 1, 50, 10);

    // Wait 1 Second and Start
    delay(1000);

    stepper_pid(&feed01Stepper, &pid, &load_cell, 4);

    return 0;
}