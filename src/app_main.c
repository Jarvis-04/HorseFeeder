#include "app_main.h"
#include "global.h"
#include "stepper.h"
#include "gpio.h"
#include "exti.h"
#include "usart.h"
#include "encoder.h"

int app_main(void) {
    systick_init(SystemCoreClock/1000);

    usart_setup();
    printf("Usart Setup and Communicating\r\n");
    printf("Press y to Start System\r\n");
    while(1) {
        if (usart_readChar() == 'y') break;
    }
    printf("System Starting\r\n");

    gpio_init(ESTOPPIN, GPIO_MODE_INPUT);
    gpio_pupd(ESTOPPIN, PU);
    exti_setup(ESTOPPIN, EXTI_FT);

    Stepper_TypeDef stepper;
    stepper.stepPin = STEPPIN;
    stepper.dirPin = DIRPIN;
    stepper_init(&stepper);
    stepper_setDir(&stepper, STEPPER_CW);

    // bool currentDir = false;
    // for (int i=0; i<10; i++) {
    //     stepper_step(&stepper, 200);
    //     currentDir = !currentDir;
    //     stepper_setDir(&stepper, currentDir);
    // }

    return 0;
}