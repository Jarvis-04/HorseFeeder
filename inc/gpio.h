#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include "global.h"

// GPIO Mode
enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG};

// Alternate functions
enum {AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15};

// Pull up modes
enum {NOPUPD, PU, PD};

// TODO: Change this to CMSIS version of this register
struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(x) ((struct gpio*)(0x40020000 + 0x400*(x)))

void gpio_init(uint16_t pin, uint16_t mode);

void gpio_pupd(uint16_t pin, uint16_t mode);

void gpio_set_af(uint16_t pin, uint8_t afMode);

void gpio_set(uint16_t pin, bool state);

bool gpio_read(uint16_t pin);

#endif