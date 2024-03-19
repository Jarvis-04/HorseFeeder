#include "stepper.h"
#include "gpio.h"
#include "limit.h"
#include "math.h"

volatile uint32_t steps = 0;
volatile bool eStop = false;
static uint32_t defaultSpeed;

// Setup for the stepper motor, step pin must be capable of being driven by timer2, timer 2 will be setup
void stepper_init(Stepper_TypeDef *stepper) {
    defaultSpeed = SystemCoreClock/500;
    stepper->stepTime = defaultSpeed;
    stepper->location = -1;
    // Enable outputs
    gpio_init(stepper->stepPin, GPIO_MODE_AF);
    gpio_init(stepper->dirPin, GPIO_MODE_OUTPUT);
    gpio_set(stepper->dirPin, STEPPER_CW);
    gpio_set_af(stepper->stepPin, AF1);

    // Enable Homing Switch
    limit_init(LIMIT01);

    // Enable timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Setup timer for PWM mode with pulse width of 10 counts
    TIM2->DIER |= TIM_DIER_CC1IE;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos);
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CCR1 = PULSE_WIDTH;
    TIM2->ARR = stepper->stepTime;

    // Ensure no eStop
    eStop = false;

    // Enable timer2 interupts
    NVIC_EnableIRQ(TIM2_IRQn);
}

void stepper_setSpeed(Stepper_TypeDef* stepper, int rpm) {
    if (rpm == 0) {
        stepper->stepTime = defaultSpeed;
    } else if (rpm < 0) {
        rpm = rpm*-1;
    }
    stepper->stepTime = 60*(SystemCoreClock/(200*rpm));
}

void stepper_setDir(Stepper_TypeDef *stepper, bool dir) {
    stepper->direction = dir;
    gpio_set(stepper->dirPin, dir);
}

void stepper_step(Stepper_TypeDef *stepper, uint32_t numSteps) {
    if (eStop) return;
    TIM2->CNT = 0;
    TIM2->ARR = stepper->stepTime;
    steps = numSteps;
    TIM2->CR1 |= TIM_CR1_CEN;
    while (steps != 0);
    TIM2->CR1 &= ~TIM_CR1_CEN;
    stepper->location += (stepper->direction ? -1*numSteps : numSteps);
}

void stepper_move_to_location(Stepper_TypeDef* stepper, uint32_t location_steps) {
    // Check for estop
    if (eStop) return;
    // Calculate how many steps needed to move to that location (motor should be homed before using this)
    int distance = location_steps - stepper->location;
    if (distance == 0) return;
    stepper_setDir(stepper, distance<0 ? STEPPER_CCW : STEPPER_CW);
    // convert mm distance to the num of steps required
    stepper_step(stepper, distance<0 ? -1*distance : distance);
}

void stepper_home(Stepper_TypeDef *stepper) {
    if (eStop) return;
    stepper_setDir(stepper, STEPPER_CCW);
    steps = pow(2, 32)-1;
    TIM2->ARR = stepper->stepTime;
    TIM2->CR1 |= TIM_CR1_CEN;
    while(steps != 0) {
        if (limit_read(LIMIT01) == LIMIT_CLOSED) {
            break;
        }
    }
    steps = 0;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    stepper->location = 0;
}

void stepper_eStop() {
    steps = 0;
    eStop = true;
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

void stepper_resetEStop(){
    eStop = false;
}

void TIM2_IRQHandler() {
    TIM2->SR &=~ TIM_SR_CC1IF;
    steps--;
}