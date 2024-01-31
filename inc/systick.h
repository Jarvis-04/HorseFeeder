#ifndef SYSTICK_H_INCLUDED
#define SYSTICK_H_INCLUDED

#include "global.h"

void systick_init(uint32_t reloadValue);

bool systick_timer_expired(uint32_t *timer, uint32_t period);

void delay(uint32_t ms);

#endif