#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stm32f446xx.h"
#include "core_cm4.h"

#include "systick.h"

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#define PI 3.14159
#define mmToSteps(x) (uint32_t)((200/(13.5*PI))*x)

#define BIT(x) (1UL << (x))
#define PIN(bank, num) (((bank - 'A') << 8) | (num))
#define PINBank(pin) (pin >> 8)
#define PINNum(pin) (pin & 255)

// Stepper
#define STEPPIN PIN('A', 5)
#define DIRPIN PIN('A', 9)

// Rail Stepper
#define RAILENPIN PIN('A', 6)

// Feed01
#define FEED01ENPIN PIN('A', 7)

// EStop
#define ESTOPPIN PIN('C', 13)

// USART
#define USARTTX PIN('A', 2)
#define USARTRX PIN('A', 3)

// Limit Switch
#define LIMIT01 PIN('A', 8)

// Load Cell
#define LOADCLK PIN('B', 5)
#define LOADDT PIN('B', 4)

#endif