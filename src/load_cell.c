#include "load_cell.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"

void load_cell_init(Load_Cell_TypeDef *load_cell) {
    gpio_init(load_cell->clk, GPIO_MODE_OUTPUT);
    gpio_init(load_cell->dt, GPIO_MODE_INPUT);
    gpio_pupd(load_cell->dt, PU);
    gpio_set(load_cell->clk, 0);

    // 1485 was good callibration for 1kg load cell
    // -781 was good callibration for 5kg load cell
    load_cell->calibration = -781;
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

float load_cell_read_average(Load_Cell_TypeDef *load_cell, uint8_t num) {
    uint32_t average = 0;
    for (uint8_t i=0; i<num; i++) {
        average += load_cell_read(load_cell);
    }
    return (float)average/num;
}

void load_cell_tare(Load_Cell_TypeDef *load_cell) {
    uint32_t offset = load_cell_read_average(load_cell, 20);
    load_cell->offset = offset;
}

float load_cell_get_value(Load_Cell_TypeDef *load_cell, int num) {
    return (load_cell_read_average(load_cell, num) - load_cell->offset);
}

float load_cell_get_grams(Load_Cell_TypeDef *load_cell, int num) {
    return (load_cell_get_value(load_cell, num)/load_cell->calibration);
}

// This function assumes a uart connection is already established to read from
// Steps to callibrate
// 1: init load cell
// 2: tare to set offset
// Follow function settings to find optimal calibration value
void load_cell_calibrate(Load_Cell_TypeDef *load_cell) {
    float currentValue = load_cell_get_grams(load_cell, 10);
    char input = 'n';
    printf("The current weight will be reported, use a to increase the calibration factor and z to decrease it. d to finish\r\n");
    while (1) {
        printf("The scale is currently reading %fgrams\r\n", currentValue);
        printf("The current calibration value is %f\r\n", load_cell->calibration);
        input = usart_readChar();
        if (input == 'a') {
            load_cell->calibration += 1;
        } else if (input == 'z') {
            load_cell->calibration -= 1;
        } else if (input == 'd') {
            break;
        }
        currentValue = load_cell_get_grams(load_cell, 10);
    }
    printf("The final calibration value is %f\r\n", load_cell->calibration);
}

void load_cell_power_down (Load_Cell_TypeDef *load_cell) {
    gpio_set(load_cell->clk, 0);
    gpio_set(load_cell->clk, 1);
}

void load_cell_power_up(Load_Cell_TypeDef *load_cell) {
    gpio_set(load_cell->clk, 0);
    // get first value to avoid initial spike
    load_cell_get_grams(load_cell, 1);
}