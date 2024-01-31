#include "usart.h"
#include "gpio.h"

// Setup USART to use STLINK to pass messages to PC
void usart_setup() {
    // Enable USART 2 RCC
    RCC->APB1ENR |= BIT(17);

    // uint16_t USARTTX = PIN('A', 2);
    // uint16_t USARTRX = PIN('A', 3);
    gpio_init(USARTTX, GPIO_MODE_AF);
    gpio_init(USARTRX, GPIO_MODE_AF);
    gpio_set_af(USARTTX, AF7);
    gpio_set_af(USARTRX, AF7);

    GPIOA->OSPEEDR |= (0x2 << 2*2) | (0x2 << 3*2);

    uint16_t uartdiv = (SystemCoreClock/2)/9600;
    USART2->BRR = ((uartdiv/16)<<4 |
                    (uartdiv%16));
    USART2->CR1 |= BIT(13) | BIT(3) | BIT(2);
}

char usart_readChar() {
    char rxb = '\0';
    while(!(USART2->SR & USART_SR_RXNE));
    rxb = USART2->DR;
    return rxb;
}

int _write (int handle, char* data, int size) {
    int count = size;
    while( count-- ) {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *data++;
    }
    return size;
}