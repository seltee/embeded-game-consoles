#include "display.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/spi.h"

#include "SPI.h"
#include "config.h"
#include "sdcard.h"

const unsigned short int colorPallete[10] = {0x000, 0xffff, 0x00f8, 0xe007, 0x1f00, 0xe0ff, 0xff07, 0x1ff8, 0x34a5};
unsigned char buffer[320 * 240];

static inline void setDataSending()
{
    gpio_put(GPIO_SCREEN_DC, 1);
}

static inline void setCommandSending()
{
    gpio_put(GPIO_SCREEN_DC, 0);
}

inline void displaySend8(unsigned char byte)
{
    SPI0Send8(byte);
}

inline void displaySend16(unsigned short data)
{
    SPI0Send8(data >> 8);
    SPI0Send8(data & 0xff);
}

inline void displaySendCmd(unsigned char command)
{
    setCommandSending();
    displaySend8(command);
}

inline void displaySendData8(unsigned char byte)
{
    setDataSending();
    displaySend8(byte);
}

inline void displaySendData16(unsigned short data)
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
        SPI0Send16(color);
    }
}

void displayInit()
{
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

void displayFillBuffer(unsigned char colorNum)
{
    const int bufferSize = 320 * 240;
    for (int i = 0; i < bufferSize; i++)
    {
        buffer[i] = colorNum;
    }
}

void displayFillRect(short int positionX, short int positionY, short int width, short int height, unsigned char colorNum)
{
    // cut by height
    if (positionY < 0)
    {
        height += positionY;
        positionY = 0;
    }

    if (positionY >= 240)
        return;

    if (positionY + height >= 240)
    {
        height = 240 - positionY;
    }

    if (height <= 0)
        return;

    // cut by width
    if (positionX < 0)
    {
        width += positionX;
        positionX = 0;
    }

    if (positionX >= 320)
        return;

    if (positionX + width >= 320)
    {
        width = 320 - positionX;
    }

    if (width >= 320)
        return;

    for (int iy = positionY; iy < positionY + height; iy++)
    {
        const int lineShift = iy * 320;
        for (int ix = positionX; ix < positionX + width; ix++)
        {
            buffer[lineShift + ix] = colorNum;
        }
    }
}

void displaySendBuffer()
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
        SPI0Send16(colorPallete[buffer[p]]);
    }
}

void displaySwitchOffCS()
{
    gpio_put(GPIO_SCREEN_CS, 1);
}