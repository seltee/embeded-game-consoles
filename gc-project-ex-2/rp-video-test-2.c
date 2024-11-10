#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "src/config.h"
#include "src/io.h"
#include "src/display.h"
#include "src/input.h"
#include "src/sdcard.h"
#include "src/sound.h"

int main()
{
    stdio_init_all();
    set_sys_clock_khz(CLOCK_SPEED, true);

    sleep_ms(1000);
    printf("Start up\n");

    initIO();
    displayInit();
    inputInitBaseLine();
    soundInit();

    if (SDInit())
    {
        printf("SD Card connected\n");
        SDReadSector();
    }
    else
    {
        printf("SD Card missing\n");
    }

    // Infinite color looping
    /*
    while (1)
    {
        displayClear(0xFF00);
        displayClear(0x00FF);
        displayClear(0x0FF0);
        displayClear(0xF00F);
        displayClear(0x0000);
    }
    */

    // Infinite LED fading through sound DAC
    /*
    int DACVal = 0;
    int DACAdd = 1;
    while (1)
    {
        if (DACVal > 32500)
            DACAdd = -1;
        if (DACVal < -32500)
            DACAdd = 1;
        DACVal += DACAdd;

        setSoundOutput(DACVal, DACVal);
        sleep_us(10);
    }
    */

    short int sOut = 0;
    short int sDir = 1;
    const int sProg[] = {152, 152, 80, 50, 300, 400, 50, 400};
    int sProgCount = 8;
    const unsigned int progLength = 100000;
    unsigned int counter = progLength;
    int sCurrentProg = 0;
    int sBass = 0;
    int sBassOut = 0;

    while (1)
    {
        sOut += sDir * sProg[sCurrentProg];
        if (sOut > 12000)
            sDir = -1;
        if (sOut < -12000)
            sDir = 1;
        counter--;
        if (counter == 0)
        {
            counter = progLength;
            sCurrentProg++;
            if (sCurrentProg >= sProgCount)
                break;
        }
        sBass++;
        if (sBass > 10000)
        {
            sBassOut = ((sBass % 80) < 40) ? 4000 : 0;
            if (sBass > 20000)
                sBass = 0;
        }

        setSoundOutput(sOut + sBassOut, sOut + sBassOut);
        sleep_us(1);
    }

    int dir = 1, pos = 0;
    int quadPosX = 100, quadPosY = 100;

    while (1)
    {
        pos += dir;
        if (pos > 220)
            dir = -1;
        if (pos < 1)
            dir = 1;

        displayFillBuffer(0x0000);

        displayFillRect(0 + pos, 10, 40, 40, 2);
        displayFillRect(50 + pos, 10, 40, 40, 3);
        displayFillRect(0 + pos, 60, 90, 32, 4);
        displayFillRect(25 + pos, 100, 40, 60, 5);

        int xInput = inputGetStickX();
        int yInput = inputGetStickY();
        quadPosX += (xInput / 128) * 2;
        quadPosY += (yInput / 128) * 2;

        displayFillRect(quadPosX, quadPosY, 32, 32, 6);

        displayFillRect(4, 204, 160 + xInput / 16, 10, 1);
        displayFillRect(4, 224, 160 + yInput / 16, 10, 1);

        displaySendBuffer();
    }
}
