#include "exti.h"
#include "stepper.h"

void exti_setup(uint16_t pin, uint16_t mode){
    EXTI->IMR |= BIT(PINNum(pin));
    if (mode == EXTI_RT) {
        EXTI->RTSR |= BIT(PINNum(pin));
    } else if (mode == EXTI_FT){
        EXTI->FTSR |= BIT(PINNum(pin));
    } else {
        EXTI->RTSR |= BIT(PINNum(pin));
        EXTI->FTSR |= BIT(PINNum(pin));
    }
    SYSCFG->EXTICR[PINNum(pin)/4] |= (PINBank(pin)<<(4*(PINNum(pin)%4)));
    // This doesnt look the cleanest, not sure how to make it better
    switch (PINNum(pin)){
        case 0:
            NVIC_EnableIRQ(EXTI0_IRQn);
            break;
        case 1:
            NVIC_EnableIRQ(EXTI1_IRQn);
            break;
        case 2:
            NVIC_EnableIRQ(EXTI2_IRQn);
            break;
        case 3:
            NVIC_EnableIRQ(EXTI3_IRQn);
            break;
        case 4:
            NVIC_EnableIRQ(EXTI4_IRQn);
            break;
    }
    if (PINNum(pin) >= 5 && PINNum(pin) <= 9) {
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    }
    if (PINNum(pin) >= 10 && PINNum(pin) <= 15) {
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
}

void EXTI15_10_IRQHandler(){
    // Pin 13 Check (Blue button eStop)
    if (EXTI->PR & (1 << 13)) {
        EXTI->PR |= BIT(13);
        stepper_eStop();
    }
}