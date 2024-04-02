#include "systick.h"

static volatile uint32_t s_ticks = 0;

void SysTick_Handler() {
    s_ticks++;
}

// TODO: Allow user to specify configuration when calling init
void systick_init() {
    // Microsecond Reload
    uint32_t reloadValue = SystemCoreClock/1000000;
    // Set clock source to processor
    // Enable interupts
    // Enable Systick
    SysTick->CTRL |= 0x7U;

    // Reset Value Register then set with desired value
    SysTick->LOAD &= ~(0xFFFFFFU);
    SysTick->LOAD |= ((reloadValue-1) & 0xFFFFFFU);

    SysTick->VAL = 0U;

    // Enable systick clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

bool systick_timer_expired(uint32_t *timer, uint32_t period) {
    // Convert period to ms
    period = period*1000;
    // Check to make sure timer has not wrapped
    if (s_ticks + period < *timer) *timer = 0;
    // Setup for the first time
    if (*timer == 0) *timer = s_ticks + period;
    // If not expired return
    if (*timer > s_ticks) return false;
    // Set next expiration time. Check if its been longer than a period since being checked and
    // use now instead of timer to catch up
    *timer = (s_ticks - *timer) > period ? s_ticks + period : *timer + period;
    return true;
}

void delay(uint32_t ms) {
    uint32_t until = s_ticks + (ms*1000);
    while (s_ticks < until) (void)0;
}

uint32_t get_millis() {
    return s_ticks/1000;
}

void delayMicroSecond(uint32_t microS) {
    uint32_t until = s_ticks + microS;
    while (s_ticks < until) (void)0;
}


