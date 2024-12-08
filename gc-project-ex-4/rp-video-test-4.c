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
#include "src/msTimer.h"
#include "src/text.h"
#include "src/spriteHorse.h"
#include "src/spriteOrange.h"
#include "src/spriteTileset.h"

#define MAX_SPRITES 120
union R12Sprite spritesBuffer1[MAX_SPRITES];
union R12Sprite spritesBuffer2[MAX_SPRITES];

int main()
{
    stdio_init_all();
    set_sys_clock_khz(CLOCK_SPEED, true);
    clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, CLOCK_SPEED, CLOCK_SPEED);

    sleep_ms(1000);
    printf("Start up\n");

    initIO();
    if (SDInit())
    {
        printf("SD Card connected\n");
        SDReadSector();
    }

    displayInit();
    inputInitBaseLine();
    MSTimerStart();

    struct R12Context context;
    R12InitializeContext(&context, spritesBuffer1, spritesBuffer2, MAX_SPRITES);
    R12SetContextFlags(&context, 0); // no need to clear each line

    unsigned int tBase = MSTimerGet();
    int fps = 0;
    int iteration = 0;

    const char *str1 = "Hello world";
    const char *str2 = "Hello YouTube";
    const char *str3 = "Thank you for watching";

    struct R12MatrixData dungeonMatrix;
    unsigned char matrixData[] = {
        34, 1, 34, 34, 15, 34,
        34, 14, 34, 34, 35, 34,
        34, 34, 0, 0, 34, 34,
        15, 16, 0, 0, 34, 2,
        34, 1, 34, 34, 15, 34,
        34, 17, 34, 34, 35, 34};

    dungeonMatrix.spritesH = 6;
    dungeonMatrix.spritesV = 6;
    dungeonMatrix.spriteWidth = 16;
    dungeonMatrix.spriteHeight = 16;
    dungeonMatrix.bytesPerSpriteId = 1;
    dungeonMatrix.spriteListInLine = 7;
    dungeonMatrix.spriteLineDataShift = dungeonMatrix.spriteListInLine * dungeonMatrix.spriteWidth / 2;
    dungeonMatrix.bitsPerPixel = 4;
    dungeonMatrix.bytesPerSpriteWidth = dungeonMatrix.spriteWidth / 2; // 8 because 2 pixels in 1 byte
    dungeonMatrix.palette = tilesetPalette;
    dungeonMatrix.spriteList = tilesetSpriteData;
    dungeonMatrix.matrixData = matrixData;
    dungeonMatrix.ignoreSpriteZero = 1;

    struct R12MatrixData textMatrix;
    const unsigned short textMatrixPalette[] = {0, 0x00f8};
    const unsigned char textMatrixData[] = {0, 1, 2, 3, 4, 5, 6, 7};
    textMatrix.spritesH = 8;
    textMatrix.spritesV = 1;
    textMatrix.spriteWidth = 8;
    textMatrix.spriteHeight = 12;
    textMatrix.bytesPerSpriteId = 1;
    textMatrix.spriteListInLine = 1;
    textMatrix.spriteLineDataShift = textMatrix.spriteListInLine;
    textMatrix.bitsPerPixel = 1;
    textMatrix.bytesPerSpriteWidth = 1;
    textMatrix.palette = textMatrixPalette;
    textMatrix.spriteList = getLetter('Z');
    textMatrix.matrixData = textMatrixData;
    textMatrix.ignoreSpriteZero = 0;

    while (1)
    {
        R12AddSprite(&context, spriteHorseBitsPerPixel, 0, 0, 320, 240, 320, spriteHorsePalette, spriteHorseData, 0);

        const int sprites = 10;
        for (int i = 0; i < sprites; i++)
        {
            R12AddSprite(
                &context,
                spriteOrangeBitsPerPixel,
                getSin((360 / sprites) * i + iteration) * 3 / 4 + 160 - 32,
                getCos((360 / sprites) * i + iteration) * 3 / 4 + 120 - 32,
                64,
                64,
                32,
                spriteOrangePalette,
                spriteOrangeData,
                R12FlagTransparent);
        }
        iteration++;

        addString(str1, 10, 10, 0xffff, &context);
        addString(str2, 10, 30, 0xffff, &context);
        addString(str3, 10, 50, 0xffff, &context);

        R12AddMatrix(&context, &dungeonMatrix, 4, 40, R12FlagTransparent);
        R12AddMatrix(&context, &dungeonMatrix, 100, 40, R12FlagTransparent);
        R12AddMatrix(&context, &dungeonMatrix, 196, 40, R12FlagTransparent);
        R12AddMatrix(&context, &dungeonMatrix, 292, 40, R12FlagTransparent);

        R12AddMatrix(&context, &textMatrix, 4, 60, R12FlagTransparent);

        inputUpdateButtonsData();
        printf("0x%08X\n", inputGetButtonState());
        for (int i = 0; i < 8; i++)
        {
            if (inputIsButtonPressed(1 << i))
            {
                R12AddSprite(
                    &context,
                    spriteOrangeBitsPerPixel,
                    24 * i + 10,
                    60,
                    64,
                    64,
                    32,
                    spriteOrangePalette,
                    spriteOrangeData,
                    R12FlagTransparent);
            }
        }

        displayRenderR12(&context);
        fps++;
        if (MSTimerGet() - tBase > 1000)
        {
            tBase = MSTimerGet();
            printf("FPS %i\n", fps);
            fps = 0;
        }
    }
}
