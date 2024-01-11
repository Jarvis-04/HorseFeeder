#include <inttypes.h>
#include <stdbool.h>

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#define BIT(x) (1UL << (x))
#define PIN(bank, num) (((bank - 'A') << 8) | (num))
#define PINBank(pin) (pin >> 8)
#define PINNum(pin) (pin & 255)

#endif