#include "rcc.h"

void rcc_enableGPIO(uint16_t bank) {
    RCC->AHB1ENR |= BIT(bank);
}

void rcc_enableSysTick() {
    RCC->APB2ENR |= BIT(14);
}

void rcc_enableTIM2() {
    RCC->APB1ENR &= ~BIT(0);
    RCC->APB1ENR |= BIT(0);
}