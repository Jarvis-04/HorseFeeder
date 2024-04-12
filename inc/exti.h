#ifndef EXTI_H_INCLUDED
#define EXTI_H_INCLUDED

#include "global.h"

// EXTI Mode
enum {EXTI_RT, EXTI_FT, EXTI_BOTH};

void exti_setup(uint16_t pin, uint16_t mode);

#endif