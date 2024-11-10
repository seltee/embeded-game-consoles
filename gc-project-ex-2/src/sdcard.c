#include "sdcard.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/spi.h"

#include "SPI.h"
#include "config.h"
#include "display.h"

unsigned char SDHC = 0;

#define CMD_GO_IDLE_STATE 0       // software reboot
#define CMD_SEND_IF_COND 8        // SDC V2 - check voltage range
#define CMD_READ_SINGLE_BLOCK 17  // Read sector
#define CMD_WRITE_SINGLE_BLOCK 24 // Write sector
#define CMD_SD_SEND_OP_COND 41    // Start init process
#define CMD_APP_CMD 55            // Main command from ACMD
#define CMD_READ_OCR 58           // Read OCR register

inline unsigned char SDCardSPIRead()
{
    return SPI0Send8(0xff);
}

inline unsigned char SDCardSPISend(unsigned char byte)
{
    return SPI0Send8(byte);
}

unsigned char SDSendCommand(unsigned char cmd, unsigned int arg)
{
    unsigned char response;
    unsigned short wait = 0;

    // Multiply adress by 512 for none SDHC read and write commands
    if (SDHC == 0 && (cmd == CMD_READ_SINGLE_BLOCK || cmd == CMD_WRITE_SINGLE_BLOCK))
        arg = arg << 9;

    SDCardSPISend(cmd | 0x40);
    SDCardSPISend(arg >> 24);
    SDCardSPISend(arg >> 16);
    SDCardSPISend(arg >> 8);
    SDCardSPISend(arg);

    // Send empty CRC
    SDCardSPISend(cmd == CMD_SEND_IF_COND ? 0x87 : 0x95);

    // Wait for response
    while ((response = SDCardSPIRead()) == 0xff)
        if (wait++ > 0xfffe)
            break;

    // check OCR from READ_OCR command
    if (response == 0x00 && cmd == CMD_READ_OCR)
    {
        // check if SDHC
        unsigned char tmp = SDCardSPIRead();
        SDHC = (tmp & 0x40) ? 1 : 0;

        // Another 3 bytes
        SDCardSPIRead();
        SDCardSPIRead();
        SDCardSPIRead();
    }

    SDCardSPIRead();
    return response;
}

bool SDInit()
{
    displaySwitchOffCS();
    gpio_put(GPIO_SDCARD_CS, 0);
    unsigned int i, SDVersion = 2, resp, retry = 0;

    sleep_ms(1);
    for (i = 0; i < 80; i++)
        SDCardSPIRead(); // Send > 74 bits
    sleep_ms(1);

    // Soft reset CD
    while (true)
    {
        resp = SDSendCommand(CMD_GO_IDLE_STATE, 0);
        if (resp == 0x01)
            break; // done

        if (retry++ > 0x40)
            return false;
        sleep_us(100);
    }

    SDCardSPIRead();
    SDCardSPIRead();

    retry = 0;
    while (SDSendCommand(CMD_SEND_IF_COND, 0x1AA) != 0x01)
    {
        if (retry++ > 0xfe)
        {
            SDVersion = 1;
            break;
        }
    }

    retry = 0;
    do
    {
        resp = SDSendCommand(CMD_APP_CMD, 0);
        resp = SDSendCommand(CMD_SD_SEND_OP_COND, 0x40000000);
        retry++;
        if (retry > 0xfffe)
            return false;
    } while (resp != 0x00);

    retry = 0;
    SDHC = 0;
    if (SDVersion == 2)
    {
        while (SDSendCommand(CMD_READ_OCR, 0) != 0x00)
            if (retry++ > 0xffe)
                break;
    }
    return true;
}

void SDReadSector()
{
    displaySwitchOffCS();
    gpio_put(GPIO_SDCARD_CS, 0);

    unsigned int i, resp, retry = 0;
    unsigned char byte;

    while (retry++ < 8)
    {
        while (SDCardSPIRead() != 0xff)
            ;
        i = 0;

        resp = SDSendCommand(CMD_READ_SINGLE_BLOCK, 0);
        if (resp)
        {
            if (resp == 5)
            {
                return;
            }
            continue;
        }

        // waiting for data
        while (SDCardSPIRead() != 0xfe)
            if (i++ > 0xffffe)
                continue;

        // reading 512 bytes
        for (i = 0; i < 512; i++)
        {
            byte = SDCardSPIRead();
            printf("Byte %i - 0x%02x\n", i, byte);
        }

        // Skip CRC
        SDCardSPIRead();
        SDCardSPIRead();
        break;
    }
}

void SDCardSwitchOffCS()
{
    gpio_put(GPIO_SDCARD_CS, 1);
}