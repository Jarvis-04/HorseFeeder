#include "usart.h"
#include "gpio.h"

// Setup USART to use STLINK to pass messages to PC
void usart_init() {
    // Enable USART 2 RCC
    RCC->APB1ENR |= BIT(17);

    // Set USART Pins to proper modes
    gpio_init(USARTTX, GPIO_MODE_AF);
    gpio_init(USARTRX, GPIO_MODE_AF);
    gpio_set_af(USARTTX, AF7);
    gpio_set_af(USARTRX, AF7);

    // Set GPIO pins to Fast Speed
    GPIOA->OSPEEDR |= (0x2 << 2*2) | (0x2 << 3*2);

    // Set the proper baud rate value for 9600
    uint16_t uartdiv = (SystemCoreClock/2)/9600;
    USART2->BRR = ((uartdiv/16)<<4 |
                    (uartdiv%16));
    // Enable 
    USART2->CR1 |= BIT(13) | BIT(3) | BIT(2);
}

// Read a character for the USART
char usart_readChar() {
    char rxb = '\0';
    // Wait for character to come in
    while(!(USART2->SR & USART_SR_RXNE));
    // Read character
    rxb = USART2->DR;
    return rxb;
}

// Allow printf to work through the USART
int _write (int handle, char* data, int size) {
    int count = size;
    while( count-- ) {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *data++;
    }
    return size;
}