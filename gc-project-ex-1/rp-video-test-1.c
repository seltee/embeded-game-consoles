#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#define GPIO_LED 0
#define GPIO_BUTTON 1

int main()
{
    gpio_init(GPIO_LED);
    gpio_init(GPIO_BUTTON);
    gpio_set_dir(GPIO_LED, GPIO_OUT);
    gpio_set_dir(GPIO_BUTTON, GPIO_IN);
    gpio_pull_up(GPIO_BUTTON);

    bool prevState = false;

    while (1)
    {
        bool newState = !gpio_get(GPIO_BUTTON);
        if (newState != prevState && newState == true)
        {
            gpio_put(GPIO_LED, 1);
            sleep_ms(200);
            gpio_put(GPIO_LED, 0);
        }
        prevState = newState;
    }
}
