#include "input.h"
#include "hardware/adc.h"

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