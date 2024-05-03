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
    Stepper_TypeDef *railStepper = stepper_init(STEPPIN, DIRPIN, RAILENPIN, 1);

    // Feeder01 Init
    Stepper_TypeDef *feed01Stepper = stepper_init(STEPPIN, DIRPIN, FEED01ENPIN, 16);

    // Load Cell Init
    Load_Cell_TypeDef *load_cell = load_cell_init(LOADCLK, LOADDT);

    // PID Init
    PID_TypeDef *pid = PID_init(10, 0.001, 0.001, 1, 50, 10);

    // Wait 1 Second and Start
    delay(1000);

    stepper_pid(feed01Stepper, pid, load_cell, 4);


    stepper_destroy(railStepper);
    stepper_destroy(feed01Stepper);
    load_cell_destroy(load_cell);
    PID_destroy(pid);
    return 0;
}