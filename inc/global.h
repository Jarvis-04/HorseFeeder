#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "stm32f446xx.h"
#include "core_cm4.h"

#include "systick.h"

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#define BIT(x) (1UL << (x))
#define PIN(bank, num) (((bank - 'A') << 8) | (num))
#define PINBank(pin) (pin >> 8)
#define PINNum(pin) (pin & 255)


// uint16_t step = PIN('A', 5);
// uint16_t dir = PIN('A', 9);
// uint16_t eStopButton = PIN('C', 13);
// uint16_t encoderButton = PIN('B', 5);
// uint16_t encoderA = PIN('A', 10);
// uint16_t encoderB = PIN('B', 3);
#define STEPPIN PIN('A', 5)
#define DIRPIN PIN('A', 9)
#define ESTOPPIN PIN('C', 13)
#define ENCODERBUTTONPIN PIN('B', 3)
#define ENCODERAPIN PIN('B', 4)
#define ENCODERBPIN PIN('B', 5)
#define USARTTX PIN('A', 2)
#define USARTRX PIN('A', 3)

#endif