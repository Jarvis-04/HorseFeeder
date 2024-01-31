#include "gpio.h"

void gpio_init(uint16_t pin, uint16_t mode) {
    RCC->AHB1ENR |= BIT(PINBank(pin));
    GPIO(PINBank(pin))->MODER &= ~(0x3 << (PINNum(pin)*2));
    GPIO(PINBank(pin))->MODER |= (mode << (PINNum(pin)*2));
}

void gpio_pupd(uint16_t pin, uint16_t mode){
    GPIO(PINBank(pin))->PUPDR |= (mode << (PINNum(pin)*2));
}


void gpio_set_af(uint16_t pin, uint8_t afMode) {
    GPIO(PINBank(pin))->AFR[PINNum(pin)/8] &= ~(afMode << (PINNum(pin)*4));
    GPIO(PINBank(pin))->AFR[PINNum(pin)/8] |= (afMode << (PINNum(pin)*4));
}

void gpio_set(uint16_t pin, bool state) {
    int addition = state ? 0 : 16;
    GPIO(PINBank(pin))->BSRR |= (1U << (PINNum(pin)+addition));
}

bool gpio_read(uint16_t pin) {
    return ((GPIO(PINBank(pin))->IDR) & (BIT(PINNum(pin))));
}