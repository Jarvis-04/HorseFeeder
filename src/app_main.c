#include "app_main.h"
#include "global.h"
#include "stepper.h"
#include "usart.h"
#include "button.h"
#include "load_cell.h"

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

    Stepper_TypeDef stepper;
    stepper.stepPin = STEPPIN;
    stepper.dirPin = DIRPIN;
    stepper_init(&stepper);
    stepper_setDir(&stepper, STEPPER_CW);

    Load_Cell_TypeDef load_cell;
    load_cell.clk = LOADCLK;
    load_cell.dt = LOADDT;
    load_cell_init(&load_cell);
    delay(1000);
    load_cell_tare(&load_cell);
    load_cell_power_down(&load_cell);

    // stepper_home(&stepper);
    // stepper_setDir(&stepper, STEPPER_CW);
    // stepper_step(&stepper, mmToSteps(100));

    // while(1) {
    //     // bool currentDir = false;
    //     // for (int i=0; i<10; i++) {
    //     //     // stepper_step(&stepper, 400);
    //     //     stepper_stepAccel(&stepper, 400);
    //     //     currentDir = !currentDir;
    //     //     stepper_setDir(&stepper, currentDir);
    //     // }

    //     // printf("Press r to run again\r\n");
    //     // while (usart_readChar() != 'r');
    // }

    return 0;
}