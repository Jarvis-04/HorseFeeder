#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "global.h"
#include "gpio.h"
#include "exti.h"

// Button mode
enum {BUTTON_POLLING, BUTTON_INTERUPT};

void button_init(uint16_t pin, uint8_t mode, uint16_t PUPD, uint16_t EXTImode);

#endif