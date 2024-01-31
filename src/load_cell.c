#include "load_cell.h"
#include "gpio.h"
#include "systick.h"

void load_cell_init(Load_Cell_TypeDef *load_cell) {
    gpio_init(load_cell->clk, GPIO_MODE_OUTPUT);
    gpio_init(load_cell->dt, GPIO_MODE_INPUT);
    gpio_pupd(load_cell->dt, PU);
    gpio_set(load_cell->clk, 0);

    // Set gain to A 128
    load_cell_wait(load_cell);
    for (int i=0; i<25; i++){
        gpio_set(load_cell->clk, 1);
        delayMicroSecond(1);
        gpio_set(load_cell->clk, 0);
        delayMicroSecond(1);
    }
}

bool load_cell_ready(Load_Cell_TypeDef *load_cell) {
    return (gpio_read(load_cell->dt)==0);
}

void load_cell_wait(Load_Cell_TypeDef *load_cell) {
    while (load_cell_ready(load_cell) == 0) {}
    return;
}

int load_cell_read(Load_Cell_TypeDef *load_cell) {
    int output = 0;
    // Might need to block interupts in future
    load_cell_wait(load_cell);

    uint32_t data = 0;
    for (int i=0; i<24; i++) {
        gpio_set(load_cell->clk, 1);
        delayMicroSecond(1);
        gpio_set(load_cell->clk, 0);
        delayMicroSecond(1);
        data |= gpio_read(load_cell->dt);
        data = data << 1;
    }
    gpio_set(load_cell->clk, 1);
    delayMicroSecond(1);
    gpio_set(load_cell->clk, 0);

    if (data & BIT(24)) {
        // Strip first bit
        data &= 0x7FFFFF;
        // Do 2's Compilment conversion
        data = (data^0x7FFFFF)+1;
        output = data*-1;
    } else {
        // Strip first bit
        data &= 0x7FFFFF;
        output = data;
    }
    printf("%d\r\n", output);
    return 0;
}