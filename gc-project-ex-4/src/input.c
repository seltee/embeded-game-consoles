#include "input.h"
#include "hardware/adc.h"

#define INPUT_WAIT         \
    __asm volatile("nop"); \
    __asm volatile("nop"); \
    __asm volatile("nop"); \
    __asm volatile("nop"); \
    __asm volatile("nop"); \
    __asm volatile("nop"); \
    __asm volatile("nop");

unsigned short inputButtonsState = 0;

int xBaseline = 0;
int yBaseline = 0;

void inputInitBaseLine()
{
    adc_select_input(0);
    xBaseline = 0;
    for (int i = 0; i < 64; i++)
    {
        xBaseline += adc_read();
    }
    xBaseline /= 64;

    adc_select_input(1);
    yBaseline = 0;
    for (int i = 0; i < 64; i++)
    {
        yBaseline += adc_read();
    }
    yBaseline /= 64;
}

int inputGetStickX()
{
    adc_select_input(0);
    int adcValue = 0;
    for (int i = 0; i < 16; i++)
    {
        adcValue += adc_read();
    }
    adcValue /= 16;
    return adcValue - xBaseline;
}

int inputGetStickY()
{
    adc_select_input(1);
    int adcValue = 0;
    for (int i = 0; i < 16; i++)
    {
        adcValue += adc_read();
    }
    adcValue /= 16;
    return adcValue - yBaseline;
}

void inputUpdateButtonsData()
{
    gpio_put(GPIO_INPUT_SCK, 0);
    gpio_put(GPIO_INPUT_LOAD, 0);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_LOAD, 1);
    INPUT_WAIT
    inputButtonsState = gpio_get(GPIO_INPUT_IN); // bit 7

    gpio_put(GPIO_INPUT_SCK, 1);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_SCK, 0);
    INPUT_WAIT
    inputButtonsState = (inputButtonsState << 1) | gpio_get(GPIO_INPUT_IN); // bit 6

    gpio_put(GPIO_INPUT_SCK, 1);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_SCK, 0);
    INPUT_WAIT
    inputButtonsState = (inputButtonsState << 1) | gpio_get(GPIO_INPUT_IN); // bit 5

    gpio_put(GPIO_INPUT_SCK, 1);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_SCK, 0);
    INPUT_WAIT
    inputButtonsState = (inputButtonsState << 1) | gpio_get(GPIO_INPUT_IN); // bit 4

    gpio_put(GPIO_INPUT_SCK, 1);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_SCK, 0);
    INPUT_WAIT
    inputButtonsState = (inputButtonsState << 1) | gpio_get(GPIO_INPUT_IN); // bit 3

    gpio_put(GPIO_INPUT_SCK, 1);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_SCK, 0);
    INPUT_WAIT
    inputButtonsState = (inputButtonsState << 1) | gpio_get(GPIO_INPUT_IN); // bit 2

    gpio_put(GPIO_INPUT_SCK, 1);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_SCK, 0);
    INPUT_WAIT
    inputButtonsState = (inputButtonsState << 1) | gpio_get(GPIO_INPUT_IN); // bit 1

    gpio_put(GPIO_INPUT_SCK, 1);
    INPUT_WAIT
    gpio_put(GPIO_INPUT_SCK, 0);
    INPUT_WAIT
    inputButtonsState = (inputButtonsState << 1) | gpio_get(GPIO_INPUT_IN); // bit 0
}

bool inputIsButtonPressed(unsigned short button)
{
    return (inputButtonsState & button) ? true : false;
}

unsigned short inputGetButtonState()
{
    return inputButtonsState;
}