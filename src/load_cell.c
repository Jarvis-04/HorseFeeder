#include "load_cell.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"

void load_cell_init(Load_Cell_TypeDef *load_cell) {
    gpio_init(load_cell->clk, GPIO_MODE_OUTPUT);
    gpio_init(load_cell->dt, GPIO_MODE_INPUT);
    gpio_pupd(load_cell->dt, PU);
    gpio_set(load_cell->clk, 0);

    // 1485 was good callibration for 1lb load cell
    load_cell->calibration = 1485;
    load_cell->offset = 0;

    // Set gain to A 128
    load_cell_wait(load_cell);
    for (uint8_t i=0; i<25; i++){
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
}

int32_t load_cell_read(Load_Cell_TypeDef *load_cell) {
    int32_t output = 0;
    load_cell_wait(load_cell);

    // Might need to block interupts in future to ensure proper communication
    uint32_t data = 0;
    for (uint8_t i=0; i<24; i++) {
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
        output |= (0xFF << 24) | data;
    } else {
        output |= (0x00 << 24) | data;
    }
    return output;
}

int32_t load_cell_read_average(Load_Cell_TypeDef *load_cell, uint8_t num) {
    int32_t average = 0;
    for (uint8_t i=0; i<num; i++) {
        average += load_cell_read(load_cell);
    }
    return average/num;
}

void load_cell_tare(Load_Cell_TypeDef *load_cell) {
    int32_t offset = load_cell_read_average(load_cell, 10);
    load_cell->offset = offset;
}

int32_t load_cell_get_value(Load_Cell_TypeDef *load_cell) {
    return (load_cell_read_average(load_cell, 10) - load_cell->offset);
}

int32_t load_cell_get_units(Load_Cell_TypeDef *load_cell) {
    return (load_cell_get_value(load_cell)/load_cell->calibration);
}

// This function assumes a uart connection is already established to read from
void load_cell_calibrate(Load_Cell_TypeDef *load_cell) {
    int32_t currentValue = load_cell_get_units(load_cell);
    char input = 'n';
    printf("The current weight will be reported, use a to increase the calibration factor and z to decrease it. d to finish\r\n");
    while (1) {
        printf("The scale is currently reading %ldgrams\r\n", currentValue);
        printf("The current calibration value is %ld\r\n", load_cell->calibration);
        input = usart_readChar();
        if (input == 'a') {
            load_cell->calibration += 1;
        } else if (input == 'z') {
            load_cell->calibration -= 1;
        } else if (input == 'd') {
            break;
        }
        currentValue = load_cell_get_units(load_cell);
    }
    printf("The final calibration value is %ld\r\n", load_cell->calibration);
}

void load_cell_power_down (Load_Cell_TypeDef *load_cell) {
    gpio_set(load_cell->clk, 0);
    gpio_set(load_cell->clk, 1);
}

void load_cell_power_up(Load_Cell_TypeDef *load_cell) {
    gpio_set(load_cell->clk, 0);
}