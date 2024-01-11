#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "header.h"

struct timer {
    volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2, CCER, CNT, PSC, ARR, RESERVED, CCR1,
                    CCR2, CCR3, CCR4, RESERVED2, DCR, DMAR, OR;
};
#define TIM2 ((struct timer*)(0x40000000))

#endif