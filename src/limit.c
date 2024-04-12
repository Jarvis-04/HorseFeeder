#include "limit.h"
#include "gpio.h"

// Initialize the limit switch
void limit_init(uint16_t pin){
    gpio_init(pin, GPIO_MODE_INPUT);
    // Set as PU. Switch is closed if input goes to ground
    gpio_pupd(pin, PU);
}

// Read limit switch
bool limit_read(uint16_t pin) {
    return gpio_read(pin);
}
