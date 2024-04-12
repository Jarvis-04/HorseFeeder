#include "gpio.h"

// Initialize GPIO Pin in the desired mode
void gpio_init(uint16_t pin, uint16_t mode) {
    // Enable the bus for the GPIO
    RCC->AHB1ENR |= BIT(PINBank(pin));
    // Set GPIO bank pin mode
    GPIO(PINBank(pin))->MODER &= ~(0x3 << (PINNum(pin)*2));
    GPIO(PINBank(pin))->MODER |= (mode << (PINNum(pin)*2));
}

// Set a pull up or pull down mode for the gpio pin
void gpio_pupd(uint16_t pin, uint16_t mode){
    GPIO(PINBank(pin))->PUPDR |= (mode << (PINNum(pin)*2));
}

// Set GPIO alternate function
void gpio_set_af(uint16_t pin, uint8_t afMode) {
    // Set GPIO AF register to the requested mode
    GPIO(PINBank(pin))->AFR[PINNum(pin)/8] &= ~(afMode << (PINNum(pin)*4));
    GPIO(PINBank(pin))->AFR[PINNum(pin)/8] |= (afMode << (PINNum(pin)*4));
}

// Set a GPIO pin to high or low
void gpio_set(uint16_t pin, bool state) {
    int addition = state ? 0 : 16;
    GPIO(PINBank(pin))->BSRR |= (1U << (PINNum(pin)+addition));
}

// Read a GPIO pin
bool gpio_read(uint16_t pin) {
    return ((GPIO(PINBank(pin))->IDR) & (BIT(PINNum(pin))));
}