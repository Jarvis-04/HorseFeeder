#include "stepper.h"
#include "gpio.h"
#include "limit.h"
#include "math.h"
#include "usart.h"

// TODO: Set ESTOP on all function that move the steppers

// Variables used by all steppers
volatile uint32_t steps = 0;
volatile bool eStop = false;

// Setup for the stepper motor, step pin must be capable of being driven by timer2, timer 2 will be setup
void stepper_init(Stepper_TypeDef *stepper) {
    // Set default speed and location values
    stepper_setSpeed(stepper, 50);
    stepper->location = -1;
    // Enable outputs
    gpio_init(stepper->stepPin, GPIO_MODE_AF);
    gpio_init(stepper->dirPin, GPIO_MODE_OUTPUT);
    gpio_init(stepper->enPin, GPIO_MODE_OUTPUT);
    gpio_set(stepper->dirPin, STEPPER_CW);
    gpio_set(stepper->enPin, true);
    gpio_set_af(stepper->stepPin, AF1);

    // Enable Homing Switch
    limit_init(LIMIT01);

    // Enable timer clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Setup timer for PWM mode with pulse width of PULSE_WIDTH(10) counts
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
    // Should not set rpm to 0
    if (rpm == 0) {
        return;
    } else if (rpm < 0) {
        // Fix rpm value if it happens to be negative
        rpm = rpm*-1;
    }
    // Calculate the proper step time based on SystemCoreClock, RPM, and steppers microstep value
    stepper->stepTime = 60*(SystemCoreClock/((200*stepper->microStep)*rpm));
    // Reset Count to ensure proper speed is set and then set the reload value of the timer
    TIM2->CNT = 0;
    TIM2->ARR = stepper->stepTime;
}

// Set the stepper dir pin to the requested direction
void stepper_setDir(Stepper_TypeDef *stepper, bool dir) {
    stepper->direction = dir;
    gpio_set(stepper->dirPin, stepper->direction);
}

// Step the motor a requested number of times
void stepper_step(Stepper_TypeDef *stepper, uint32_t numSteps) {
    if (eStop) return;
    // Enable motor and set the requested steps
    stepper_enable(stepper);
    steps = numSteps;
    // Enable timer to start the motor, wait until the steps are done and turn the timer off
    TIM2->CR1 |= TIM_CR1_CEN;
    while (steps != 0);
    TIM2->CR1 &= ~TIM_CR1_CEN;
    // Set the current location of the stepper and then disable it
    stepper->location += (stepper->direction ? -1*numSteps : numSteps);
    stepper_disable(stepper);
}

// Start running the stepper in the backgroud
void stepper_start(Stepper_TypeDef *stepper) {
    stepper_enable(stepper);
    steps = pow(2, 32) -1;
    TIM2->CR1 |= TIM_CR1_CEN;
}

// Stop the stepper, should be called after stepper start
void stepper_stop(Stepper_TypeDef *stepper) {
    TIM2->CR1 &= ~TIM_CR1_CEN;
    stepper_disable(stepper);
}

// Move stepper to specific location
void stepper_move_to_location(Stepper_TypeDef* stepper, uint32_t location_steps) {
    // Check for estop
    if (eStop) return;
    // Calculate how many steps needed to move to that location (motor should be homed before using this)
    int distance = location_steps - stepper->location;
    if (distance == 0) return;
    stepper_setDir(stepper, distance<0 ? STEPPER_CCW : STEPPER_CW);
    stepper_step(stepper, distance<0 ? -1*distance : distance);
}

// Run motor on PID loop to dispense desired amount of feed
void stepper_pid(Stepper_TypeDef *stepper, PID_TypeDef *pid, Load_Cell_TypeDef *input, float target) {
    // Initial setup of pid values and motor settings
    bool running = true;
    float output;
    float weight;
    load_cell_tare(input);
    stepper_setDir(stepper, STEPPER_CW);
    stepper_setSpeed(stepper, pid->outMin);
    PID_setSetPoint(pid, target);
    stepper_start(stepper);
    // Start the motor and loop
    while(running){
        // Get the current weight, find the PID output, and set the motor speed accordingly
        weight = load_cell_get_grams(input, 3);
        output = PID_compute(pid, weight);
        stepper_setSpeed(stepper, (int)output);
        // If the desired weight is achieved then stop the motor
        if (weight >= target) {
            running = false;
        }
    }
    stepper_stop(stepper);
    // TODO: Set some kind of retraction to prevent extra spillage
    // stepper_setSpeed(stepper, 10);
    // stepper_setDir(stepper, STEPPER_CCW);
    // stepper_step(stepper, (200*stepper->microStep)/2);
    // printf("Weight: %f\r\n", weight);
}

// Run the motor until the user no longer needs it to run (used to empty the feed augers)
void stepper_purge(Stepper_TypeDef *stepper) {
    stepper_setDir(stepper, STEPPER_CW);
    stepper_setSpeed(stepper, 75);
    printf("Press s when the purge is complete\r\n");
    stepper_start(stepper);
    while(1) {
        if (usart_readChar() == 's') break;
    }
    stepper_stop(stepper);
    printf("Purge Complete\r\n");
}

// Run motor until it contacts the limit switch
void stepper_home(Stepper_TypeDef *stepper) {
    if (eStop) return;
    stepper_enable(stepper);
    steps = pow(2, 32)-1;
    TIM2->CR1 |= TIM_CR1_CEN;
    while(steps != 0) {
        if (limit_read(LIMIT01) == LIMIT_CLOSED) {
            break;
        }
    }
    steps = 0;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    stepper->location = 0;
    stepper_disable(stepper);
}

// Enable the stepper driver
void stepper_enable(Stepper_TypeDef *stepper) {
    gpio_set(stepper->enPin, false);
}

// Disbale the stepper driver
void stepper_disable(Stepper_TypeDef *stepper) {
    gpio_set(stepper->enPin, true);
}

// If ESTOP is called then turn off timer and set eStop to true
void stepper_eStop() {
    steps = 0;
    eStop = true;
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

// Reset the ESTOP
void stepper_resetEStop(){
    eStop = false;
}

// Timer Interupt service routine to count steps
void TIM2_IRQHandler() {
    TIM2->SR &=~ TIM_SR_CC1IF;
    steps--;
}