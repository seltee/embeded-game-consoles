#include "display.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

#include "SPI.h"
#include "config.h"
#include "sdcard.h"

bool isRenderingInProcess = false;
unsigned short int lBuffer1[320], lBuffer2[320];
int screenDMAChannel = -1;
struct R12Context *coreR12Context = NULL;

static inline void setDataSending()
{
    gpio_put(GPIO_SCREEN_DC, 1);
    sleep_us(1);
}

static inline void setCommandSending()
{
    gpio_put(GPIO_SCREEN_DC, 0);
    sleep_us(1);
}

static inline void displaySend8(unsigned char byte)
{
    displaySendDataDMA(&byte, 1);
    dma_channel_wait_for_finish_blocking(screenDMAChannel);
    sleep_us(2);
}

static inline void displaySend16(unsigned short data)
{
    displaySend8(data >> 8);
    displaySend8(data & 0xff);
}

static inline void displaySendCmd(unsigned char command)
{
    setCommandSending();
    displaySend8(command);
}

static inline void displaySendData8(unsigned char byte)
{
    setDataSending();
    displaySend8(byte);
}

static inline void displaySendData16(unsigned short data)
{
    setDataSending();
    displaySend16(data);
}

void displaySetCol(unsigned short startCol, unsigned short endCol)
{
    displaySendCmd(0x2A);
    displaySendData16(startCol);
    displaySendData16(endCol);
}

void displaySetPage(unsigned short startPage, unsigned short endPage)
{
    displaySendCmd(0x2B);
    displaySendData16(startPage);
    displaySendData16(endPage);
}

void displaySetXY(unsigned short positionX, unsigned short positionY)
{
    displaySetCol(positionX, positionX);
    displaySetPage(positionY, positionY);
    displaySendCmd(0x2C);
}

void displayClear(unsigned short color)
{
    SDCardSwitchOffCS();
    gpio_put(GPIO_SCREEN_CS, 0);

    displaySetPage(0, 240);
    displaySetCol(0, 320);
    displaySendCmd(0x2c); // data sending mode

    setDataSending();
    const int size = 320 * 240;
    for (int p = 0; p < size; p++)
    {
        displaySend16(color);
    }
}

void displayInit()
{
    screenDMAChannel = dma_claim_unused_channel(true);

    SDCardSwitchOffCS();
    gpio_put(GPIO_SCREEN_CS, 0);
    sleep_ms(1);
    gpio_put(GPIO_SCREEN_RESET, 0);
    sleep_ms(1);
    gpio_put(GPIO_SCREEN_RESET, 1);
    sleep_ms(4);

    displaySendCmd(0x01); // software reset
    sleep_ms(4);

    displaySendCmd(0xCB);
    displaySendData8(0x39);
    displaySendData8(0x2C);
    displaySendData8(0x00);
    displaySendData8(0x34);
    displaySendData8(0x02);
    sleep_ms(1);

    displaySendCmd(0xCF);
    displaySendData8(0x00);
    displaySendData8(0xC1);
    displaySendData8(0x30);
    sleep_ms(1);

    displaySendCmd(0xE8);
    displaySendData8(0x85);
    displaySendData8(0x00);
    displaySendData8(0x78);
    sleep_ms(1);

    displaySendCmd(0xEA);
    displaySendData8(0x00);
    displaySendData8(0x00);
    sleep_ms(1);

    displaySendCmd(0xED);
    displaySendData8(0x64);
    displaySendData8(0x03);
    displaySendData8(0x12);
    displaySendData8(0x81);
    sleep_ms(1);

    displaySendCmd(0xF7);
    displaySendData8(0x20);
    sleep_ms(1);

    displaySendCmd(0xC0);
    displaySendData8(0x25);
    sleep_ms(1);

    displaySendCmd(0xC1);
    displaySendData8(0x10);
    sleep_ms(1);

    displaySendCmd(0xC5);
    displaySendData8(0x3e);
    displaySendData8(0x28);
    sleep_ms(1);

    displaySendCmd(0xC7);
    displaySendData8(0x86);
    sleep_ms(1);

    displaySendCmd(0x36);
    displaySendData8(0x48);
    sleep_ms(1);

    displaySendCmd(0x3A);
    displaySendData8(0x55);
    sleep_ms(1);

    displaySendCmd(0xB1);
    displaySendData8(0x00);
    displaySendData8(0x18);
    sleep_ms(1);

    displaySendCmd(0xB6);
    displaySendData8(0x08);
    displaySendData8(0x82);
    displaySendData8(0x27);
    sleep_ms(1);

    displaySendCmd(0x11); // exit sleep
    sleep_ms(1);
    displaySendCmd(0x29); // display on
    sleep_ms(1);

    // disable 3gamma
    displaySendCmd(0xF2);
    displaySendData8(0x00);
    sleep_ms(1);

    // enable gamma curve
    displaySendCmd(0x26);
    displaySendData8(0x01);
    sleep_ms(1);

    // set positive gamma correction
    displaySendCmd(0xE0);
    displaySendData8(0x0F);
    displaySendData8(0x31);
    displaySendData8(0x2B);
    displaySendData8(0x0C);
    displaySendData8(0x0E);
    displaySendData8(0x08);
    displaySendData8(0x4E);
    displaySendData8(0xF1);
    displaySendData8(0x37);
    displaySendData8(0x07);
    displaySendData8(0x10);
    displaySendData8(0x03);
    displaySendData8(0x0E);
    displaySendData8(0x09);
    displaySendData8(0x00);
    sleep_ms(1);

    // set negative gamma correction
    displaySendCmd(0xE1);
    displaySendData8(0x00);
    displaySendData8(0x0E);
    displaySendData8(0x14);
    displaySendData8(0x03);
    displaySendData8(0x11);
    displaySendData8(0x07);
    displaySendData8(0x31);
    displaySendData8(0xC1);
    displaySendData8(0x48);
    displaySendData8(0x08);
    displaySendData8(0x0F);
    displaySendData8(0x0C);
    displaySendData8(0x31);
    displaySendData8(0x36);
    displaySendData8(0x0F);
    sleep_ms(1);

    // orientation
    displaySendCmd(0x36);
    displaySendData8(0xE8);
    sleep_ms(1);

    displayClear(0x0000);
    displayClear(0x0000);
    gpio_put(GPIO_SCREEN_LED, 1);
}

void _displayRenderR12CoreTask()
{
    displaySetPage(0, 240);
    displaySetCol(0, 320);
    displaySendCmd(0x2c); // Data sending mode

    setDataSending();

    R12FillLine(coreR12Context, 0, lBuffer1);
    displaySendDataDMA((unsigned char *)lBuffer1, 640);
    unsigned short *cBuffer = lBuffer1;

    for (int line = 1; line < 240; line++)
    {
        cBuffer = cBuffer == lBuffer1 ? lBuffer2 : lBuffer1; // Switch buffer
        R12FillLine(line, cBuffer);
        displaySendDataDMA((unsigned char *)cBuffer, 640);
    }

    dma_channel_wait_for_finish_blocking(screenDMAChannel);
    isRenderingInProcess = false;
}

void displayRenderR12(struct R12Context *context)
{
    while (isRenderingInProcess)
        sleep_us(1);

    SDCardSwitchOffCS();
    gpio_put(GPIO_SCREEN_CS, 0);
    R12PrepareNewFrame(context);

    coreR12Context = context;
    isRenderingInProcess = true;
    multicore_reset_core1();
    multicore_launch_core1(_displayRenderR12CoreTask);
}

void displaySendDataDMA(unsigned char *data, unsigned int length)
{
    dma_channel_wait_for_finish_blocking(screenDMAChannel);
    spi_get_hw(spi0)->cpsr = 4;

    dma_channel_config txConfig = dma_channel_get_default_config(screenDMAChannel);
    channel_config_set_transfer_data_size(&txConfig, DMA_SIZE_8); // 8 bits
    channel_config_set_read_increment(&txConfig, true);
    channel_config_set_write_increment(&txConfig, false);
    channel_config_set_dreq(&txConfig, DREQ_SPI0_TX);
    dma_channel_configure(screenDMAChannel, &txConfig, &spi_get_hw(spi0)->dr, data, length, false);

    // enable DMA
    dma_start_channel_mask(1u << screenDMAChannel);
}

void displaySwitchOffCS()
{
    gpio_put(GPIO_SCREEN_CS, 1);
}