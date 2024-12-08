#include "sound.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "utils.h"

#include "config.h"
#define TARGET_FREQUENCY (192000 * 4)

unsigned short int outLeft = 0;
unsigned short int outRight = 0;

int clkShift = 0, sent = 0;
unsigned int tmpSnd = 0;

void __isr pwmInterruptHandler()
{
    // Clear interrupt flag
    pwm_clear_irq(pwm_gpio_to_slice_num(GPIO_SOUND_PWM));

    if (sent == 0 && clkShift == 1)
    {
        // Build 32 bit value we will transmit to DAC
        tmpSnd = (((unsigned int)outLeft) << 16) | (unsigned int)outRight;
    }

    // Channel switch
    if (sent == 0 && clkShift == 1)
        gpio_put(GPIO_SOUND_WS, 0);
    if (sent == 16 && clkShift == 1)
        gpio_put(GPIO_SOUND_WS, 1);

    // CLK shift
    if (clkShift == 1)
    {
        clkShift = 0;
        gpio_put(GPIO_SOUND_DATA, (tmpSnd & 0x80000000) ? 1 : 0);
        tmpSnd <<= 1;
        gpio_put(GPIO_SOUND_SCK, 0);
    }
    else
    {
        clkShift = 1;
        gpio_put(GPIO_SOUND_SCK, 1);

        sent++;
        if (sent == 32)
            sent = 0;
    }
}

void soundInit()
{
    // Configure the PWM GPIO
    gpio_set_function(GPIO_SOUND_PWM, GPIO_FUNC_PWM);

    // Calcualte PWM wrap value for 192,000 Hz frequency
    unsigned int sourceHZ = CLOCK_SPEED * 1000;
    unsigned int wrap = (sourceHZ / TARGET_FREQUENCY);

    pwm_set_clkdiv(pwm_gpio_to_slice_num(GPIO_SOUND_PWM), 1);
    pwm_set_wrap(pwm_gpio_to_slice_num(GPIO_SOUND_PWM), wrap);

    // Set up PWM interrupt
    pwm_clear_irq(pwm_gpio_to_slice_num(GPIO_SOUND_PWM));
    pwm_set_irq_enabled(pwm_gpio_to_slice_num(GPIO_SOUND_PWM), true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwmInterruptHandler);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // enable PWM
    pwm_set_enabled(pwm_gpio_to_slice_num(GPIO_SOUND_PWM), true);
}

void setSoundOutput(unsigned short int leftChannel, unsigned short int rightChannel)
{
    outLeft = SWITCH_BYTES(leftChannel);
    outRight = SWITCH_BYTES(rightChannel);
}