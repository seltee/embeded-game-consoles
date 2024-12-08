#include "r12.h"

int counter = 0;

void R12PrepareNewFrame()
{
    counter++;
}

void R12FillLine(int lineNumber, unsigned short int *outBuffer)
{
    for (int x = 0; x < 320; x++)
        outBuffer[x] = 0;

    if ((lineNumber % 80) < 40)
    {
        for (int x = 0; x < 40; x++)
        {
            outBuffer[(x + counter) % 320] = 0xffff;
        }
    }
}