#include "exti.h"
#include "stepper.h"

void exti_setup(){
    EXTI->IMR |= BIT(13);
    // EXTI->RTSR |= BIT(13);
    EXTI->FTSR |= BIT(13);
    SYSCFG->EXTICR[3] |= (0b0010 << 4);
}

void EXTI15_10_IRQHandler(){
    EXTI->PR |= BIT(13);
    stepper_eStop();
}