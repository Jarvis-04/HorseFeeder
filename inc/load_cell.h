#ifndef LOAD_CELL_H_INCLUDED
#define LOAD_CELL_H_INCLUDED

#include "global.h"

typedef struct load_cell{
    uint16_t clk;
    uint16_t dt;
    int32_t offset;
    int32_t calibration;
} Load_Cell_TypeDef;

void load_cell_init(Load_Cell_TypeDef *load_cell);

bool load_cell_ready(Load_Cell_TypeDef *load_cell);

void load_cell_wait(Load_Cell_TypeDef *load_cell);

int32_t load_cell_read(Load_Cell_TypeDef *load_cell);

int32_t load_cell_read_average(Load_Cell_TypeDef *load_cell, uint8_t num);

void load_cell_tare(Load_Cell_TypeDef *load_cell);

int32_t load_cell_get_value(Load_Cell_TypeDef *load_cell);

int32_t load_cell_get_units(Load_Cell_TypeDef *load_cell);

void load_cell_calibrate(Load_Cell_TypeDef *load_cell);

void load_cell_power_down (Load_Cell_TypeDef *load_cell);

void load_cell_power_up(Load_Cell_TypeDef *load_cell);

#endif