#ifndef LIMIT_H_INCLUDED
#define LIMIT_H_INCLUDED

#include "global.h"

enum {LIMIT_CLOSED, LIMIT_OPEN};

void limit_init(uint16_t pin);

bool limit_read(uint16_t pin);

#endif