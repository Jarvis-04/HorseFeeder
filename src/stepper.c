#include "stepper.h"
#include "gpio.h"
#include "rcc.h"

// TODO: Change default stepper time to be based on timer clock rather than hard set

volatile uint32_t steps = 0;

// Setup for the stepper motor, step pin must be capable of being driven by timer2, timer 2 will be setup
void stepper_init(Stepper_TypeDef *stepper) {
    stepper->stepTime = 84000;
    // Enable outputs
    gpio_init(stepper->stepPin, GPIO_MODE_AF);
    gpio_init(stepper->dirPin, GPIO_MODE_OUTPUT);
    gpio_set(stepper->dirPin, STEPPER_CW);
    gpio_set_af(stepper->stepPin, AF1);

    // Enable timer clock
    rcc_enableTIM2();

    TIM2->DIER |= BIT(0);
    TIM2->CCMR1 |= (0b011 << 4);
    TIM2->CCER |= BIT(0);
    TIM2->ARR = stepper->stepTime;

    // Enable timer2 interupts
    NVIC_EnableIRQ(TIM2_IRQn);
}

void stepper_setDir(Stepper_TypeDef *stepper, bool dir) {
    gpio_set(stepper->dirPin, dir);
}

void stepper_step(Stepper_TypeDef *stepper, uint32_t numSteps) {
    stepper->stepTime = 84000;
    TIM2->ARR = stepper->stepTime;
    steps = numSteps*2;
    TIM2->CR1 |= BIT(0);
    while (steps != 0);
    TIM2->CR1 &= ~BIT(0);
}

// TODO: Setup rundown time so it decelerates as well
void stepper_stepAccel(Stepper_TypeDef *stepper, uint32_t numSteps) {
    stepper->stepTime = 84000;
    steps = numSteps*2;
    uint32_t currentStep = steps;
    TIM2->ARR = stepper->stepTime;
    TIM2->CR1 |= BIT(0);
    while (steps != 0) {
        if (currentStep != steps) {
            stepper->stepTime -= 50;
            if (stepper->stepTime <= 20000) {
                stepper->stepTime = 20000;
            }
            TIM2->ARR = stepper->stepTime;
            currentStep = steps;
        }
    }
    TIM2->CR1 &= ~BIT(0);
}

void TIM2_IRQHandler() {
    TIM2->SR &= ~(0x1);
    steps--;
}