#include "encoder.h"
#include "gpio.h"
#include "exti.h"

volatile static int steps;

// Encoder is no longer used and as such this file is not kept up. Keeping file in case it is used in the future
// TODO: Change the hardcoded timer 3 to be included in the encoder typedef
void encoder_setup(Encoder_TypeDef* encoder) {
    steps = 0;

    gpio_init(encoder->A, GPIO_MODE_AF);
    gpio_init(encoder->B, GPIO_MODE_AF);
    gpio_set_af(encoder->A, AF2);
    gpio_set_af(encoder->B, AF2);
    gpio_init(encoder->Button, GPIO_MODE_INPUT);

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->SMCR |= (0b011 << TIM_SMCR_SMS_Pos);
    TIM3->ARR = 0xFFFF;
    TIM3->CNT = 0xFFFF/2;
    // TIM3->ARR = 4;
    // TIM3->DIER |= TIM_DIER_UIE;
    // TIM3->ARR = 0xFFFF;
    // TIM3->DIER |= TIM_DIER_TIE;
    // NVIC_EnableIRQ(TIM3_IRQn);
}

void encoder_start(Encoder_TypeDef* encoder) {
    TIM3->CR1 |= TIM_CR1_CEN;
}

void encoder_stop(Encoder_TypeDef* encoder) {
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

// void TIM3_IRQHandler(){
//     TIM3->SR &= ~(TIM_SR_UIF);
//     // if ((TIM3->CR1 & TIM_CR1_DIR)) {
//     //     steps++;
//     // } else {
//     //     steps--;
//     // }
//     printf("Click Detected: %d\r\n", steps);
// }