#ifndef LOAD_CELL_H_INCLUDED
#define LOAD_CELL_H_INCLUDED

#include "global.h"

typedef struct load_cell{
    uint16_t clk;
    uint16_t dt;
} Load_Cell_TypeDef;

void load_cell_init(Load_Cell_TypeDef *load_cell);

bool load_cell_ready(Load_Cell_TypeDef *load_cell);

void load_cell_wait(Load_Cell_TypeDef *load_cell);

int load_cell_read(Load_Cell_TypeDef *load_cell);

#endif