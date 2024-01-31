#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

// Encoder is no longer used and as such this file is not kept up. Keeping file in case it is used in the future

#include "global.h"

enum {NU, CW, CCW, PB};

typedef struct encoder{
    uint16_t A;
    uint16_t B;
    uint16_t Button;
} Encoder_TypeDef;

void encoder_setup(Encoder_TypeDef* encoder);

void encoder_start(Encoder_TypeDef* encoder);

void encoder_stop(Encoder_TypeDef* encoder);

#endif