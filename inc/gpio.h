#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include "header.h"

enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG};

enum {AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15};

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(x) ((struct gpio*)(0x40020000 + 0x400*(x)))

void gpio_init(uint16_t pin, uint16_t mode);

void gpio_set_af(uint16_t pin, uint8_t afMode);

void gpio_set(uint16_t pin, bool state);

// void gpio_reset(uint16_t pin);

#endif