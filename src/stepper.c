#include "stepper.h"
#include "gpio.h"
#include "limit.h"
#include "math.h"

volatile uint32_t steps = 0;
volatile bool eStop = false;
static uint32_t defaultSpeed;

// Setup for the stepper motor, step pin must be capable of being driven by timer2, timer 2 will be setup
void stepper_init(Stepper_TypeDef *stepper) {
    defaultSpeed = (SystemCoreClock/1000)*2;
    stepper->stepTime = defaultSpeed;
    // Enable outputs
    gpio_init(stepper->stepPin, GPIO_MODE_AF);
    gpio_init(stepper->dirPin, GPIO_MODE_OUTPUT);
    gpio_set(stepper->dirPin, STEPPER_CW);
    gpio_set_af(stepper->stepPin, AF1);

    // Enable Homing Switch
    limit_init(LIMIT01);

    // Enable timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CCMR1 |= (0b011 << TIM_CCMR1_OC1M_Pos);
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->ARR = stepper->stepTime;

    // Ensure no eStop
    eStop = false;

    // Enable timer2 interupts
    NVIC_EnableIRQ(TIM2_IRQn);
}

void stepper_setDir(Stepper_TypeDef *stepper, bool dir) {
    gpio_set(stepper->dirPin, dir);
}

void stepper_step(Stepper_TypeDef *stepper, uint32_t numSteps) {
    if (eStop) return;
    stepper->stepTime = defaultSpeed;
    TIM2->ARR = stepper->stepTime;
    steps = numSteps*2;
    TIM2->CR1 |= BIT(0);
    while (steps != 0);
    TIM2->CR1 &= ~BIT(0);
}

// TODO: Setup rundown time so it decelerates as well
void stepper_stepAccel(Stepper_TypeDef *stepper, uint32_t numSteps) {
    if (eStop) return;
    stepper->stepTime = defaultSpeed;
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

void stepper_home(Stepper_TypeDef *stepper) {
    if (eStop) return;
    stepper_setDir(stepper, STEPPER_CCW);
    stepper->stepTime = defaultSpeed;
    steps = pow(2, 32)-1;
    TIM2->ARR = stepper->stepTime;
    TIM2->CR1 |= BIT(0);
    while(steps != 0) {
        if (limit_read(LIMIT01) == LIMIT_CLOSED) {
            break;
        }
    }
    steps = 0;
    TIM2->CR1 &= ~BIT(0);
}

void stepper_eStop() {
    steps = 0;
    eStop = true;
    TIM2->CR1 &= ~BIT(0);
}

void stepper_resetEStop(){
    eStop = false;
}

void TIM2_IRQHandler() {
    TIM2->SR &= ~(0x1);
    steps--;
}