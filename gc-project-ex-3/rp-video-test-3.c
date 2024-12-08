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

    unsigned int tBase = MSTimerGet();
    int fps = 0;

    while (1)
    {
        displayRenderR12();
        fps++;
        if (MSTimerGet() - tBase > 1000)
        {
            tBase = MSTimerGet();
            printf("FPS %i\n", fps);
            fps = 0;
        }
    }
}
