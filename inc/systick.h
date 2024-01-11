#ifndef SYSTICK_H_INCLUDED
#define SYSTICK_H_INCLUDED

#include "header.h"

struct systick {
    volatile uint32_t CSR, RVR, CVR, CALIB;
};
#define SysTick ((struct systick*) 0xE000E010)

void systick_init(uint32_t reloadValue);

bool systick_timer_expired(uint32_t *timer, uint32_t period);

#endif