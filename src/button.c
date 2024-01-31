#include "button.h"

void button_init(uint16_t pin, uint8_t mode, uint16_t PUPD, uint16_t EXTImode){
    gpio_init(pin, GPIO_MODE_INPUT);
    gpio_pupd(pin, PUPD);
    if (mode == BUTTON_INTERUPT) {
        exti_setup(pin, EXTImode);
    }
}

// Need a read button function