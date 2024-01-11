#include "systick.h"
#include "rcc.h"

static volatile uint32_t s_ticks;

// void SysTick_Handler() {
//     s_ticks++;
// }

// TODO: Allow user to specify configuration when calling init
void systick_init(uint32_t reloadValue) {
    // Set clock source to processor
    // Enable interupts
    // Enable Systick
    SysTick->CSR |= 0x7U;

    // Reset Value Register then set with desired value
    SysTick->RVR &= ~(0xFFFFFFU);
    SysTick->RVR |= ((reloadValue-1) & 0xFFFFFFU);

    SysTick->CVR = 0U;

    // Enable systick clock
    rcc_enableSysTick();
}

bool systick_timer_expired(uint32_t *timer, uint32_t period) {
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

