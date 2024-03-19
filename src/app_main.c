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
    load_cell_tare(&load_cell);
    load_cell_power_down(&load_cell);

    delay(1000);

    stepper_setSpeed(&stepper, 200);
    stepper_home(&stepper);
    stepper_move_to_location(&stepper, 200);
    stepper_move_to_location(&stepper, 2000);
    stepper_move_to_location(&stepper, 200);

    return 0;
}