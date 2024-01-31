#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

#define STEPPIN PIN('A', 5)
#define DIRPIN PIN('A', 9)
#define ESTOPPIN PIN('C', 13)
#define USARTTX PIN('A', 2)
#define USARTRX PIN('A', 3)
#define LIMIT01 PIN('A', 8)
#define LOADCLK PIN('B', 5)
#define LOADDT PIN('B', 4)

#endif