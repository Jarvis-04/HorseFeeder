#include "stepper.h"
#include "gpio.h"
#include "rcc.h"
#include "timer.h"

static uint32_t steps = 0;
static uint32_t stepTime;

// Setup for the stepper motor, step pin must be capable of being driven by timer2, timer 2 will be setup
void stepper_init(uint32_t step, uint32_t dir, uint32_t baseStepTime) {
    stepTime = baseStepTime;
    // Enable outputs
    gpio_init(step, GPIO_MODE_AF);
    gpio_init(dir, GPIO_MODE_OUTPUT);
    gpio_set(dir, STEPPER_CW);
    gpio_set_af(step, AF1);

    // Enable timer clock
    rcc_enableTIM2();

    TIM2->DIER |= BIT(0);
    TIM2->CCMR1 |= (0b011 << 4);
    TIM2->CCER |= BIT(0);
    TIM2->ARR = stepTime;

    // Enable timer2 interupts
    volatile uint32_t* NVIC_ISER0 = (uint32_t *)(0xE000E100);
    *NVIC_ISER0 |= BIT(28);
}

void stepper_step(uint32_t step, uint32_t dir, uint32_t numSteps) {
    while (steps < numSteps*2) {
        TIM2->CR1 |= BIT(0);
    }
    TIM2->CR1 &= ~BIT(0);
}

void stepper_test(uint32_t step, uint32_t dir) {
    stepTime = 84000;
    uint32_t currentStep = steps;
    TIM2->ARR = stepTime;
    TIM2->CR1 |= BIT(0);
    while (steps < 4000*2) {
        if (currentStep != steps) {
            stepTime = stepTime - 50;
            if (stepTime <= 20000) {
                stepTime = 20000;
            }
            TIM2->ARR = stepTime;
            currentStep = steps;
        }
    }
    TIM2->CR1 &= ~BIT(0);
    steps = 0;
}

void TIM2_IRQHandler() {
    static volatile uint32_t* TIM2_SR = (uint32_t *)(0x40000000 + 0x10);
    *TIM2_SR &= ~(0x1);
    steps++;
}