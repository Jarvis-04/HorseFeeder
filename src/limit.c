#include "limit.h"
#include "gpio.h"

void limit_init(uint16_t pin){
    gpio_init(pin, GPIO_MODE_INPUT);
    gpio_pupd(pin, PU);
}

bool limit_read(uint16_t pin) {
    return gpio_read(pin);
}
