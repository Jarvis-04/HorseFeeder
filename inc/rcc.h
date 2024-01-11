#ifndef RCC_H_INCLUDED
#define RCC_H_INCLUDED

#include "header.h"

struct rcc{
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR, RESERVED01, APB1RSTR, APB2RSTR, RESERVED02[2],
                    AHB1ENR, AHB2ENR, AHB3ENR, RESERVED03, APB1ENR, APB2ENR, RESERVED04[2], AHB1LPENR, AHB2LPENR,
                    AHB3LPENR, RESERVED05, APB1LPENR, APB2LPENR, RESERVED06[2], BDCR, CSR, RESERVED07[2], SSCGR,
                    PLLI2SCFGR, PLLSAICFGR, DCKCFGR, CKGATENR, DCKCFGR2;
};
#define RCC ((struct rcc*)(0x40023800))

void rcc_enableGPIO(uint16_t pin);

void rcc_enableSysTick(void);

void rcc_enableTIM2(void);

#endif