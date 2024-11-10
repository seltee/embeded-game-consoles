#include "io.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/spi.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

void initIO()
{
    // SPI
    spi_init(spi0, SPI0_BAUD_RATE);

    // SPI format
    spi_set_format(spi0, 8, 0, 0, SPI_MSB_FIRST);

    // assign SPI0 pins
    gpio_set_function(GPIO_SPI0_SCK, GPIO_FUNC_SPI);
    gpio_set_function(GPIO_SPI0_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(GPIO_SPI0_MISO, GPIO_FUNC_SPI);

    // Display controls
    gpio_init(GPIO_SCREEN_CS);
    gpio_set_dir(GPIO_SCREEN_CS, GPIO_OUT);
    gpio_put(GPIO_SCREEN_CS, 1);
    gpio_init(GPIO_SCREEN_RESET);
    gpio_set_dir(GPIO_SCREEN_RESET, GPIO_OUT);
    gpio_init(GPIO_SCREEN_DC);
    gpio_set_dir(GPIO_SCREEN_DC, GPIO_OUT);
    gpio_init(GPIO_SCREEN_LED);
    gpio_set_dir(GPIO_SCREEN_LED, GPIO_OUT);
    gpio_put(GPIO_SCREEN_LED, 0);

    // SD Card
    gpio_init(GPIO_SDCARD_CS);
    gpio_set_dir(GPIO_SDCARD_CS, GPIO_OUT);
    gpio_put(GPIO_SDCARD_CS, 1);

    // Input ADC
    adc_init();
    adc_gpio_init(GPIO_INPUT_AXIS_X);
    adc_gpio_init(GPIO_INPUT_AXIS_Y);

    // Sound
    gpio_init(GPIO_SOUND_WS);
    gpio_set_dir(GPIO_SOUND_WS, GPIO_OUT);
    gpio_put(GPIO_SOUND_WS, 0);
    gpio_init(GPIO_SOUND_SCK);
    gpio_set_dir(GPIO_SOUND_SCK, GPIO_OUT);
    gpio_put(GPIO_SOUND_SCK, 0);
    gpio_init(GPIO_SOUND_DATA);
    gpio_set_dir(GPIO_SOUND_DATA, GPIO_OUT);
    gpio_put(GPIO_SOUND_DATA, 0);
}