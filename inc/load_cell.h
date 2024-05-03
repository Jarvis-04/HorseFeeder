#ifndef LOAD_CELL_H_INCLUDED
#define LOAD_CELL_H_INCLUDED

#include "global.h"

typedef struct loadCellData Load_Cell_TypeDef;

Load_Cell_TypeDef *load_cell_init(uint16_t clkPin, uint16_t dataPin);

void load_cell_destroy(Load_Cell_TypeDef *load_cell);

bool load_cell_ready(Load_Cell_TypeDef *load_cell);

void load_cell_wait(Load_Cell_TypeDef *load_cell);

int32_t load_cell_read(Load_Cell_TypeDef *load_cell);

float load_cell_read_average(Load_Cell_TypeDef *load_cell, uint8_t num);

void load_cell_tare(Load_Cell_TypeDef *load_cell);

float load_cell_get_value(Load_Cell_TypeDef *load_cell, int num);

float load_cell_get_grams(Load_Cell_TypeDef *load_cell, int num);

void load_cell_calibrate(Load_Cell_TypeDef *load_cell);

void load_cell_power_down (Load_Cell_TypeDef *load_cell);

void load_cell_power_up(Load_Cell_TypeDef *load_cell);

#endif