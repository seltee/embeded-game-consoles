#include "msTimer.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

unsigned int msCounter = 0;
struct repeating_timer timer;

bool _MSTimerCallback(struct repeating_timer *t)
{
    msCounter++;
    return true;
}

void MSTimerStart()
{
    add_repeating_timer_ms(1, _MSTimerCallback, NULL, &timer);
}

unsigned int MSTimerGet()
{
    return msCounter;
}