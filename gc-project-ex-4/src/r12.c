#include "r12.h"

void R12InitializeContext(struct R12Context *context, union R12Sprite *spritesBuffer1, union R12Sprite *spritesBuffer2, unsigned short maxSprites)
{
    context->spritesBuffer1 = spritesBuffer1;
    context->spritesBuffer2 = spritesBuffer2;
    context->spritesBufferAmount1 = 0;
    context->spritesBufferAmount2 = 0;
    context->maxSprites = maxSprites;
    context->currentSpritesDrawBuffer = spritesBuffer1;
    context->currentSpritesRenderBuffer = spritesBuffer2;
    context->pCurrentDrawBufferAmount = &context->spritesBufferAmount1;
    context->pCurrentRenderBufferAmount = &context->spritesBufferAmount2;
    context->flags = R12ContextFlagClearLine;
    context->clearColor = 0;
}

void R12SetContextFlags(struct R12Context *context, unsigned int flags)
{
    context->flags = flags;
}

void R12PrepareNewFrame(struct R12Context *context)
{
    if (context->currentSpritesDrawBuffer == context->spritesBuffer1)
    {
        context->currentSpritesDrawBuffer = context->spritesBuffer2;
        context->pCurrentDrawBufferAmount = &context->spritesBufferAmount2;
        context->currentSpritesRenderBuffer = context->spritesBuffer1;
        context->pCurrentRenderBufferAmount = &context->spritesBufferAmount1;
    }
    else
    {
        context->currentSpritesDrawBuffer = context->spritesBuffer1;
        context->pCurrentDrawBufferAmount = &context->spritesBufferAmount1;
        context->currentSpritesRenderBuffer = context->spritesBuffer2;
        context->pCurrentRenderBufferAmount = &context->spritesBufferAmount2;
    }
    *context->pCurrentDrawBufferAmount = 0;
}

void R12FillLine(struct R12Context *context, int lineNumber, unsigned short int *pOutBuffer)
{
    int spriteBufferAmount = *context->pCurrentRenderBufferAmount;
    union R12Sprite *spritesBuffer = context->currentSpritesRenderBuffer;

    // Clear the line if context flag is set
    if (context->flags & R12ContextFlagClearLine)
    {
        for (int i = 0; i < 320; i++)
            pOutBuffer[i] = context->clearColor;
    }

    for (int i = 0; i < spriteBufferAmount; i++)
    {
        union R12Sprite *sprite = &spritesBuffer[i];
        if (lineNumber >= sprite->spriteMainData.startY && lineNumber < sprite->spriteMainData.endY)
        {
            switch (sprite->spriteMainData.type)
            {
            case R12Sprite8bit:
                R12FillLineSprite8bit(lineNumber, &sprite->spritePalette, pOutBuffer);
                break;
            case R12Sprite4bit:
                R12FillLineSprite4bit(lineNumber, &sprite->spritePalette, pOutBuffer);
                break;
            case R12Sprite2bit:
                R12FillLineSprite2bit(lineNumber, &sprite->spritePalette, pOutBuffer);
                break;
            case R12SpriteMask:
                R12FillLineSpriteMask(lineNumber, &sprite->spriteMask, pOutBuffer);
                break;
            case R12Matrix:
                R12FillLineMatrix(lineNumber, &sprite->matrix, pOutBuffer);
                break;
            }
        }
    }
}

void R12FillLineSprite8bit(int line, struct R12SpritePalette *sprite, unsigned short *pOutBuffer)
{
    int inLine = line - sprite->startY; // internal sprite line
    const unsigned char *pSprite = sprite->data + inLine * sprite->widthDataShift;
    unsigned short *pDest = &pOutBuffer[sprite->startX];
    const unsigned short int *palette = sprite->palette;
    unsigned char flags = sprite->flags;

    unsigned short width = sprite->width;
    int i, fastC, slowC;

    fastC = width >> 3;
    slowC = width - (fastC << 3);
    if (flags & R12FlagHMirror)
    {
        pDest += width - 1;
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < fastC; i++)
            {
                if (pSprite[0] != 0)
                    pDest[0] = palette[pSprite[0]];
                if (pSprite[1] != 0)
                    pDest[-1] = palette[pSprite[1]];
                if (pSprite[2] != 0)
                    pDest[-2] = palette[pSprite[2]];
                if (pSprite[3] != 0)
                    pDest[-3] = palette[pSprite[3]];
                if (pSprite[4] != 0)
                    pDest[-4] = palette[pSprite[4]];
                if (pSprite[5] != 0)
                    pDest[-5] = palette[pSprite[5]];
                if (pSprite[6] != 0)
                    pDest[-6] = palette[pSprite[6]];
                if (pSprite[7] != 0)
                    pDest[-7] = palette[pSprite[7]];
                pDest -= 8;
                pSprite += 8;
            }
            for (i = 0; i < slowC; i++)
            {
                if (pSprite[0] > 0)
                    pDest[0] = palette[pSprite[0]];
                pDest--;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < fastC; i++)
            {
                pDest[0] = palette[pSprite[0]];
                pDest[-1] = palette[pSprite[1]];
                pDest[-2] = palette[pSprite[2]];
                pDest[-3] = palette[pSprite[3]];
                pDest[-4] = palette[pSprite[4]];
                pDest[-5] = palette[pSprite[5]];
                pDest[-6] = palette[pSprite[6]];
                pDest[-7] = palette[pSprite[7]];
                pDest -= 8;
                pSprite += 8;
            }
            for (i = 0; i < slowC; i++)
            {
                pDest[0] = palette[pSprite[0]];
                pDest--;
                pSprite++;
            }
        }
    }
    else
    {
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < fastC; i++)
            {
                if (pSprite[0] != 0)
                    pDest[0] = palette[pSprite[0]];
                if (pSprite[1] != 0)
                    pDest[1] = palette[pSprite[1]];
                if (pSprite[2] != 0)
                    pDest[2] = palette[pSprite[2]];
                if (pSprite[3] != 0)
                    pDest[3] = palette[pSprite[3]];
                if (pSprite[4] != 0)
                    pDest[4] = palette[pSprite[4]];
                if (pSprite[5] != 0)
                    pDest[5] = palette[pSprite[5]];
                if (pSprite[6] != 0)
                    pDest[6] = palette[pSprite[6]];
                if (pSprite[7] != 0)
                    pDest[7] = palette[pSprite[7]];
                pDest += 8;
                pSprite += 8;
            }
            for (i = 0; i < slowC; i++)
            {
                if (pSprite[0] > 0)
                    pDest[0] = palette[pSprite[0]];
                pDest++;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < fastC; i++)
            {
                pDest[0] = palette[pSprite[0]];
                pDest[1] = palette[pSprite[1]];
                pDest[2] = palette[pSprite[2]];
                pDest[3] = palette[pSprite[3]];
                pDest[4] = palette[pSprite[4]];
                pDest[5] = palette[pSprite[5]];
                pDest[6] = palette[pSprite[6]];
                pDest[7] = palette[pSprite[7]];
                pDest += 8;
                pSprite += 8;
            }
            for (i = 0; i < slowC; i++)
            {
                pDest[0] = palette[pSprite[0]];
                pDest++;
                pSprite++;
            }
        }
    }
}

void R12FillLineSprite4bit(int line, struct R12SpritePalette *sprite, unsigned short *pOutBuffer)
{
    int inLine = line - sprite->startY; // internal sprite line
    const unsigned char *pSprite = sprite->data + inLine * sprite->widthDataShift;
    unsigned short *pDest = &pOutBuffer[sprite->startX];
    const unsigned short int *palette = sprite->palette;
    unsigned char flags = sprite->flags;

    unsigned short width = sprite->width;
    unsigned short bytesInLine = width >> 1;
    int i, fastC, slowC, p1, p2;

    fastC = bytesInLine >> 2;
    slowC = bytesInLine - (fastC << 2);
    if (flags & R12FlagHMirror)
    {
        pDest += width - 1;
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < fastC; i++)
            {
                p1 = pSprite[0] >> 4;
                p2 = pSprite[0] & 0x0f;
                if (p1)
                    pDest[0] = palette[p1];
                if (p2)
                    pDest[-1] = palette[p2];
                p1 = pSprite[1] >> 4;
                p2 = pSprite[1] & 0x0f;
                if (p1)
                    pDest[-2] = palette[p1];
                if (p2)
                    pDest[-3] = palette[p2];
                p1 = pSprite[2] >> 4;
                p2 = pSprite[2] & 0x0f;
                if (p1)
                    pDest[-4] = palette[p1];
                if (p2)
                    pDest[-5] = palette[p2];
                p1 = pSprite[3] >> 4;
                p2 = pSprite[3] & 0x0f;
                if (p1)
                    pDest[-6] = palette[p1];
                if (p2)
                    pDest[-7] = palette[p2];
                pDest -= 8;
                pSprite += 4;
            }
            for (i = 0; i < slowC; i++)
            {
                p1 = pSprite[0] >> 4;
                p2 = pSprite[0] & 0x0f;
                if (p1)
                    pDest[0] = palette[p1];
                if (p2)
                    pDest[-1] = palette[p2];
                pDest -= 2;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < fastC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 4];
                pDest[-1] = palette[pSprite[0] & 0x0f];
                pDest[-2] = palette[pSprite[1] >> 4];
                pDest[-3] = palette[pSprite[1] & 0x0f];
                pDest[-4] = palette[pSprite[2] >> 4];
                pDest[-5] = palette[pSprite[2] & 0x0f];
                pDest[-6] = palette[pSprite[3] >> 4];
                pDest[-7] = palette[pSprite[3] & 0x0f];
                pDest -= 8;
                pSprite += 4;
            }
            for (i = 0; i < slowC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 4];
                pDest[-1] = palette[pSprite[0] & 0x0f];
                pDest -= 2;
                pSprite++;
            }
        }
    }
    else
    {
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < fastC; i++)
            {
                p1 = pSprite[0] >> 4;
                p2 = pSprite[0] & 0x0f;
                if (p1)
                    pDest[0] = palette[p1];
                if (p2)
                    pDest[1] = palette[p2];
                p1 = pSprite[1] >> 4;
                p2 = pSprite[1] & 0x0f;
                if (p1)
                    pDest[2] = palette[p1];
                if (p2)
                    pDest[3] = palette[p2];
                p1 = pSprite[2] >> 4;
                p2 = pSprite[2] & 0x0f;
                if (p1)
                    pDest[4] = palette[p1];
                if (p2)
                    pDest[5] = palette[p2];
                p1 = pSprite[3] >> 4;
                p2 = pSprite[3] & 0x0f;
                if (p1)
                    pDest[6] = palette[p1];
                if (p2)
                    pDest[7] = palette[p2];
                pDest += 8;
                pSprite += 4;
            }
            for (i = 0; i < slowC; i++)
            {
                p1 = pSprite[0] >> 4;
                p2 = pSprite[0] & 0x0f;
                if (p1)
                    pDest[0] = palette[p1];
                if (p2)
                    pDest[1] = palette[p2];
                pDest += 2;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < fastC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 4];
                pDest[1] = palette[pSprite[0] & 0x0f];
                pDest[2] = palette[pSprite[1] >> 4];
                pDest[3] = palette[pSprite[1] & 0x0f];
                pDest[4] = palette[pSprite[2] >> 4];
                pDest[5] = palette[pSprite[2] & 0x0f];
                pDest[6] = palette[pSprite[3] >> 4];
                pDest[7] = palette[pSprite[3] & 0x0f];
                pDest += 8;
                pSprite += 4;
            }
            for (i = 0; i < slowC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 4];
                pDest[1] = palette[pSprite[0] & 0x0f];
                pDest += 2;
                pSprite++;
            }
        }
    }
}

void R12FillLineSprite2bit(int line, struct R12SpritePalette *sprite, unsigned short *pOutBuffer)
{
    int inLine = line - sprite->startY; // internal sprite line
    const unsigned char *pSprite = sprite->data + inLine * sprite->widthDataShift;
    unsigned short *pDest = &pOutBuffer[sprite->startX];
    const unsigned short int *palette = sprite->palette;
    unsigned char flags = sprite->flags;

    unsigned short width = sprite->width;
    unsigned short bytesInLine = width >> 2;
    int i, fastC, slowC, p1, p2, p3, p4;

    fastC = bytesInLine >> 1;
    slowC = bytesInLine - (fastC << 1);
    if (flags & R12FlagHMirror)
    {
        pDest += width - 1;
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < fastC; i++)
            {
                p1 = pSprite[0] >> 6;
                p2 = (pSprite[0] >> 4) & 0x03;
                p3 = (pSprite[0] >> 2) & 0x03;
                p4 = pSprite[0] & 0x03;
                if (p1)
                    pDest[0] = palette[p1];
                if (p2)
                    pDest[-1] = palette[p2];
                if (p3)
                    pDest[-2] = palette[p1];
                if (p4)
                    pDest[-3] = palette[p2];
                p1 = pSprite[1] >> 6;
                p2 = (pSprite[1] >> 4) & 0x03;
                p3 = (pSprite[1] >> 2) & 0x03;
                p4 = pSprite[1] & 0x03;
                if (p1)
                    pDest[-4] = palette[p1];
                if (p2)
                    pDest[-5] = palette[p2];
                if (p3)
                    pDest[-6] = palette[p1];
                if (p4)
                    pDest[-7] = palette[p2];
                pDest -= 8;
                pSprite += 2;
            }
            for (i = 0; i < slowC; i++)
            {
                p1 = pSprite[0] >> 6;
                p2 = (pSprite[0] >> 4) & 0x03;
                p3 = (pSprite[0] >> 2) & 0x03;
                p4 = pSprite[0] & 0x03;
                if (p1)
                    pDest[0] = palette[p1];
                if (p2)
                    pDest[-1] = palette[p2];
                if (p3)
                    pDest[-2] = palette[p1];
                if (p4)
                    pDest[-3] = palette[p2];
                pDest -= 4;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < fastC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 6];
                pDest[-1] = palette[(pSprite[0] >> 4) & 0x03];
                pDest[-2] = palette[(pSprite[0] >> 2) & 0x03];
                pDest[-3] = palette[pSprite[0] & 0x03];
                pDest[-4] = palette[pSprite[1] >> 6];
                pDest[-5] = palette[(pSprite[1] >> 4) & 0x03];
                pDest[-6] = palette[(pSprite[1] >> 2) & 0x03];
                pDest[-7] = palette[pSprite[1] & 0x03];
                pDest -= 8;
                pSprite += 2;
            }
            for (i = 0; i < slowC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 6];
                pDest[-1] = palette[(pSprite[0] >> 4) & 0x03];
                pDest[-2] = palette[(pSprite[0] >> 2) & 0x03];
                pDest[-3] = palette[pSprite[0] & 0x03];
                pDest -= 4;
                pSprite++;
            }
        }
    }
    else
    {
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < fastC; i++)
            {
                p1 = pSprite[0] >> 6;
                p2 = (pSprite[0] >> 4) & 0x03;
                p3 = (pSprite[0] >> 2) & 0x03;
                p4 = pSprite[0] & 0x03;
                if (p1)
                    pOutBuffer[0] = palette[p1];
                if (p2)
                    pOutBuffer[1] = palette[p2];
                if (p3)
                    pOutBuffer[2] = palette[p1];
                if (p4)
                    pOutBuffer[3] = palette[p2];
                p1 = pSprite[1] >> 6;
                p2 = (pSprite[1] >> 4) & 0x03;
                p3 = (pSprite[1] >> 2) & 0x03;
                p4 = pSprite[1] & 0x03;
                if (p1)
                    pOutBuffer[4] = palette[p1];
                if (p2)
                    pOutBuffer[5] = palette[p2];
                if (p3)
                    pOutBuffer[6] = palette[p1];
                if (p4)
                    pOutBuffer[7] = palette[p2];
                pDest += 8;
                pSprite += 2;
            }
            for (i = 0; i < slowC; i++)
            {
                p1 = pSprite[0] >> 6;
                p2 = (pSprite[0] >> 4) & 0x03;
                p3 = (pSprite[0] >> 2) & 0x03;
                p4 = pSprite[0] & 0x03;
                if (p1)
                    pOutBuffer[0] = palette[p1];
                if (p2)
                    pOutBuffer[1] = palette[p2];
                if (p3)
                    pOutBuffer[2] = palette[p1];
                if (p4)
                    pOutBuffer[3] = palette[p2];
                pDest += 4;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < fastC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 6];
                pDest[1] = palette[(pSprite[0] >> 4) & 0x03];
                pDest[2] = palette[(pSprite[0] >> 2) & 0x03];
                pDest[3] = palette[pSprite[0] & 0x03];
                pDest[4] = palette[pSprite[1] >> 6];
                pDest[5] = palette[(pSprite[1] >> 4) & 0x03];
                pDest[6] = palette[(pSprite[1] >> 2) & 0x03];
                pDest[7] = palette[pSprite[1] & 0x03];
                pDest += 8;
                pSprite += 2;
            }
            for (i = 0; i < slowC; i++)
            {
                pDest[0] = palette[pSprite[0] >> 6];
                pDest[1] = palette[(pSprite[0] >> 4) & 0x03];
                pDest[2] = palette[(pSprite[0] >> 2) & 0x03];
                pDest[3] = palette[pSprite[0] & 0x03];
                pDest += 4;
                pSprite++;
            }
        }
    }
}

void R12FillLineSpriteMask(int line, struct R12SpriteMask *sprite, unsigned short *pOutBuffer)
{
    int inLine = line - sprite->startY; // internal sprite line
    const unsigned char *pSprite = sprite->data + inLine * sprite->widthDataShift;
    unsigned short *pDest = &pOutBuffer[sprite->startX];

    unsigned short colorBackground = sprite->colorBackground;
    unsigned short colorForeground = sprite->colorForeground;

    unsigned char flags = sprite->flags;

    unsigned short width = sprite->width;
    unsigned short bytesInLine = width >> 3;
    int i, p1, p2, p3, p4, p5, p6, p7, p8;

    if (flags & R12FlagHMirror)
    {
        pDest += width - 1;
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < bytesInLine; i++)
            {
                p1 = pSprite[0] >> 7;
                p2 = (pSprite[0] >> 6) & 0x01;
                p3 = (pSprite[0] >> 5) & 0x01;
                p4 = (pSprite[0] >> 4) & 0x01;
                p5 = (pSprite[0] >> 3) & 0x01;
                p6 = (pSprite[0] >> 2) & 0x01;
                p7 = (pSprite[0] >> 1) & 0x01;
                p8 = pSprite[0] & 0x01;
                if (p1)
                    pDest[0] = colorForeground;
                if (p2)
                    pDest[-1] = colorForeground;
                if (p3)
                    pDest[-2] = colorForeground;
                if (p4)
                    pDest[-3] = colorForeground;
                if (p5)
                    pDest[-4] = colorForeground;
                if (p6)
                    pDest[-5] = colorForeground;
                if (p7)
                    pDest[-6] = colorForeground;
                if (p8)
                    pDest[-7] = colorForeground;
                pDest -= 8;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < bytesInLine; i++)
            {
                pDest[0] = (&colorForeground)[pSprite[0] >> 7];
                pDest[-1] = (&colorForeground)[(pSprite[0] >> 6) & 0x01];
                pDest[-2] = (&colorForeground)[(pSprite[0] >> 5) & 0x01];
                pDest[-3] = (&colorForeground)[(pSprite[0] >> 4) & 0x01];
                pDest[-4] = (&colorForeground)[(pSprite[0] >> 3) & 0x01];
                pDest[-5] = (&colorForeground)[(pSprite[0] >> 2) & 0x01];
                pDest[-6] = (&colorForeground)[(pSprite[0] >> 1) & 0x01];
                pDest[-7] = (&colorForeground)[pSprite[0] & 0x01];
                pDest -= 8;
                pSprite += 2;
            }
        }
    }
    else
    {
        if (flags & R12FlagTransparent)
        {
            for (int i = 0; i < bytesInLine; i++)
            {
                p1 = pSprite[0] >> 7;
                p2 = (pSprite[0] >> 6) & 0x01;
                p3 = (pSprite[0] >> 5) & 0x01;
                p4 = (pSprite[0] >> 4) & 0x01;
                p5 = (pSprite[0] >> 3) & 0x01;
                p6 = (pSprite[0] >> 2) & 0x01;
                p7 = (pSprite[0] >> 1) & 0x01;
                p8 = pSprite[0] & 0x01;
                if (p1)
                    pDest[0] = colorForeground;
                if (p2)
                    pDest[1] = colorForeground;
                if (p3)
                    pDest[2] = colorForeground;
                if (p4)
                    pDest[3] = colorForeground;
                if (p5)
                    pDest[4] = colorForeground;
                if (p6)
                    pDest[5] = colorForeground;
                if (p7)
                    pDest[6] = colorForeground;
                if (p8)
                    pDest[7] = colorForeground;
                pDest += 8;
                pSprite++;
            }
        }
        else
        {
            for (int i = 0; i < bytesInLine; i++)
            {
                pDest[0] = (&colorForeground)[pSprite[0] >> 7];
                pDest[1] = (&colorForeground)[(pSprite[0] >> 6) & 0x01];
                pDest[2] = (&colorForeground)[(pSprite[0] >> 5) & 0x01];
                pDest[3] = (&colorForeground)[(pSprite[0] >> 4) & 0x01];
                pDest[4] = (&colorForeground)[(pSprite[0] >> 3) & 0x01];
                pDest[5] = (&colorForeground)[(pSprite[0] >> 2) & 0x01];
                pDest[6] = (&colorForeground)[(pSprite[0] >> 1) & 0x01];
                pDest[7] = (&colorForeground)[pSprite[0] & 0x01];
                pDest += 8;
                pSprite += 2;
            }
        }
    }
}

void R12FillLineMatrix(int line, struct R12Matrix *matrix, unsigned short *pOutBuffer)
{
    struct R12MatrixData *matrixData = matrix->matrixData;
    int inMatrixLine = line - matrix->y;
    int matrixSpriteLine = inMatrixLine / matrixData->spriteHeight;
    int inSpriteLine = inMatrixLine % matrixData->spriteHeight;

    int x = matrix->x;
    int y = matrix->y;
    int spriteWidth = matrixData->spriteWidth;
    int spriteHeight = matrixData->spriteHeight;
    const unsigned short int *palette = matrixData->palette;
    unsigned int widthDataShift = matrixData->spriteLineDataShift;
    unsigned char flags = matrix->flags;
    unsigned char bytesPerSpriteId = matrixData->bytesPerSpriteId;
    unsigned char ignoreSpriteZero = matrixData->ignoreSpriteZero;
    unsigned char bitsPerPixel = matrixData->bitsPerPixel;
    unsigned short spritesH = matrixData->spritesH;

    int yDataShift = 0;
    int py = matrix->startY + matrixSpriteLine * spriteHeight;
    int finalHeight = spriteHeight;
    int spriteNumber = 0;
    if (py < 0)
    {
        yDataShift += widthDataShift * -py;
        finalHeight += py;
        py = 0;
        if (flags & R12FlagVMirror)
            yDataShift += (finalHeight - spriteHeight) * widthDataShift;
    }
    if (py >= 240)
        return;
    if (py + finalHeight >= 240)
    {
        finalHeight = 240 - py;
        if (flags & R12FlagVMirror)
            yDataShift += (spriteHeight - finalHeight) * widthDataShift;
    }
    if (finalHeight <= 0)
        return;

    for (int ix = 0; ix < spritesH; ix++)
    {
        int px = x + ix * spriteWidth;
        if (px >= 320)
            return;

        int finalWidth = spriteWidth;

        if (bytesPerSpriteId == 1)
            spriteNumber = matrixData->matrixData[ix + matrixSpriteLine * spritesH];
        else
            spriteNumber = ((const unsigned short *)matrixData->matrixData)[ix + matrixSpriteLine * spritesH];
        if (spriteNumber == 0 && ignoreSpriteZero)
            continue;

        int spriteDataX = spriteNumber % matrixData->spriteListInLine;
        int spriteDataY = spriteNumber / matrixData->spriteListInLine;
        int spriteXShift = spriteDataX * matrixData->bytesPerSpriteWidth;
        int spriteYShift = spriteDataY * matrixData->spriteHeight * widthDataShift;

        const unsigned char *data = &matrixData->spriteList[spriteXShift + spriteYShift] + yDataShift;

        if (bitsPerPixel == 8 && px < 0)
        {
            data += -px;
            finalWidth += px;
            px = 0;
            if (flags & R12FlagHMirror)
                data += (finalWidth - spriteWidth);
        }
        if (bitsPerPixel == 4 && px < 0)
        {
            int c = ((-px) >> 1) << 1;
            data += c >> 1;
            finalWidth -= c;
            px += c;
            if (flags & R12FlagHMirror)
                data += (finalWidth - spriteWidth);
        }
        if (bitsPerPixel == 2 && px < 0)
        {
            int c = ((-px) >> 2) << 2;
            data += c >> 2;
            finalWidth -= c;
            px += c;
            if (flags & R12FlagHMirror)
                data += (finalWidth - spriteWidth);
        }
        if (bitsPerPixel == 1 && px < 0)
        {
            int c = ((-px) >> 3) << 3;
            data += c >> 3;
            finalWidth -= c;
            px += c;
            if (flags & R12FlagHMirror)
                data += (finalWidth - spriteWidth);
        }
        if (px >= 320)
            continue;
        if (px + finalWidth > 320)
        {
            finalWidth = 320 - px;
            if (flags & R12FlagHMirror)
                data += (spriteWidth - finalWidth);
        }
        if (finalWidth <= 0)
            continue;

        // flag correction
        if (flags & R12FlagVMirror)
        {
            data += widthDataShift * finalHeight - widthDataShift;
            widthDataShift = -widthDataShift;
        }

        if (bitsPerPixel >= 2)
        {
            struct R12SpritePalette spritePalette;
            spritePalette.type = 0;
            spritePalette.startX = px;
            spritePalette.startY = 0;
            spritePalette.endY = finalHeight;
            spritePalette.width = finalWidth;
            spritePalette.height = finalHeight;
            spritePalette.widthDataShift = widthDataShift;
            spritePalette.palette = palette;
            spritePalette.data = data;
            spritePalette.flags = flags;

            if (bitsPerPixel == 8)
                R12FillLineSprite8bit(inSpriteLine, &spritePalette, pOutBuffer);
            if (bitsPerPixel == 4)
                R12FillLineSprite4bit(inSpriteLine, &spritePalette, pOutBuffer);
            if (bitsPerPixel == 2)
                R12FillLineSprite2bit(inSpriteLine, &spritePalette, pOutBuffer);
        }
        else
        {
            struct R12SpriteMask spriteMask;
            spriteMask.type = 0;
            spriteMask.startX = px;
            spriteMask.startY = 0;
            spriteMask.endY = finalHeight;
            spriteMask.width = finalWidth;
            spriteMask.height = finalHeight;
            spriteMask.widthDataShift = widthDataShift;
            spriteMask.colorBackground = palette[0];
            spriteMask.colorForeground = palette[1];
            spriteMask.data = data;
            spriteMask.flags = flags;

            R12FillLineSpriteMask(inSpriteLine, &spriteMask, pOutBuffer);
        }
    }
}

bool R12AddSprite(
    struct R12Context *context,
    unsigned char bitsPerPixel,
    int x,
    int y,
    int width,
    int height,
    unsigned int widthDataShift,
    const unsigned short int *palette,
    const unsigned char *data,
    unsigned char flags)
{
    if (*context->pCurrentDrawBufferAmount >= context->maxSprites)
        return false;
    if (bitsPerPixel != 8 && bitsPerPixel != 4 && bitsPerPixel != 2)
        return false;

    int finalWidth = width;
    int finalHeight = height;

    // Cropping by screen on height
    if (y < 0)
    {
        data += widthDataShift * -y;
        finalHeight += y;
        y = 0;
        if (flags & R12FlagHMirror)
            data += (finalHeight - height) * widthDataShift;
    }
    if (y >= 240)
        return false;
    if (y + finalHeight >= 240)
    {
        finalHeight = 240 - y;
        if (flags & R12FlagVMirror)
            data += (height - finalHeight) * widthDataShift;
    }
    if (finalHeight <= 0)
        return false;

    // Cropping by screen on width
    if (bitsPerPixel == 8 && x < 0)
    {
        data += -x;
        finalWidth += x;
        x = 0;
        if (flags & R12FlagHMirror)
            data += (finalWidth - width);
    }
    if (bitsPerPixel == 4 && x < 0)
    {
        int c = ((-x) >> 1) << 1;
        data += c >> 1;
        finalWidth -= c;
        x += c;
        if (flags & R12FlagHMirror)
            data += (finalWidth - width);
    }
    if (bitsPerPixel == 2 && x < 0)
    {
        int c = ((-x) >> 2) << 2;
        data += c >> 2;
        finalWidth -= c;
        x += c;
        if (flags & R12FlagHMirror)
            data += (finalWidth - width);
    }
    if (x >= 320)
        return false;
    if (x + finalWidth > 320)
    {
        finalWidth = 320 - x;
        if (flags & R12FlagHMirror)
            data += (width - finalWidth);
    }
    if (finalWidth <= 0)
        return false;

    // All checks passed, add to the queue
    int spriteNum = *context->pCurrentDrawBufferAmount;
    *context->pCurrentDrawBufferAmount = *context->pCurrentDrawBufferAmount + 1;

    if (flags & R12FlagVMirror)
    {
        data += widthDataShift * finalHeight - widthDataShift;
        widthDataShift = -widthDataShift;
    }

    if (bitsPerPixel == 8)
        context->currentSpritesDrawBuffer[spriteNum].spritePalette.type = R12Sprite8bit;
    if (bitsPerPixel == 4)
        context->currentSpritesDrawBuffer[spriteNum].spritePalette.type = R12Sprite4bit;
    if (bitsPerPixel == 2)
        context->currentSpritesDrawBuffer[spriteNum].spritePalette.type = R12Sprite2bit;

    context->currentSpritesDrawBuffer[spriteNum].spritePalette.startX = x;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.startY = y;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.endY = y + finalHeight;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.width = finalWidth;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.height = finalHeight;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.widthDataShift = widthDataShift;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.palette = palette;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.data = data;
    context->currentSpritesDrawBuffer[spriteNum].spritePalette.flags = flags;

    return true;
}

bool R12AddSpriteMask(
    struct R12Context *context,
    int x,
    int y,
    int width,
    int height,
    unsigned int widthDataShift,
    unsigned short colorBackground,
    unsigned short colorForeground,
    const unsigned char *data,
    unsigned char flags)
{
    if (*context->pCurrentDrawBufferAmount >= context->maxSprites)
        return false;

    int finalWidth = width;
    int finalHeight = height;

    // Cropping by screen on height
    if (y < 0)
    {
        data += widthDataShift * -y;
        finalHeight += y;
        y = 0;
        if (flags & R12FlagHMirror)
            data += (finalHeight - height) * widthDataShift;
    }
    if (y >= 240)
        return false;
    if (y + finalHeight >= 240)
    {
        finalHeight = 240 - y;
        if (flags & R12FlagVMirror)
            data += (height - finalHeight) * widthDataShift;
    }
    if (finalHeight <= 0)
        return false;

    // Cropping by screen on width
    if (x < 0)
    {
        int c = ((-x) >> 3) << 3;
        data += c >> 3;
        finalWidth -= c;
        x += c;
        if (flags & R12FlagHMirror)
            data += (finalWidth - width);
    }
    if (x >= 320)
        return false;
    if (x + finalWidth > 320)
    {
        finalWidth = 320 - x;
        if (flags & R12FlagHMirror)
            data += (width - finalWidth);
    }
    if (finalWidth <= 0)
        return false;

    // All checks passed, add to the queue
    int spriteNum = *context->pCurrentDrawBufferAmount;
    *context->pCurrentDrawBufferAmount = *context->pCurrentDrawBufferAmount + 1;

    if (flags & R12FlagVMirror)
    {
        data += widthDataShift * finalHeight - widthDataShift;
        widthDataShift = -widthDataShift;
    }

    context->currentSpritesDrawBuffer[spriteNum].spriteMask.type = R12SpriteMask;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.startX = x;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.startY = y;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.endY = y + finalHeight;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.width = finalWidth;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.height = finalHeight;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.widthDataShift = widthDataShift;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.colorBackground = colorBackground;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.colorForeground = colorForeground;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.data = data;
    context->currentSpritesDrawBuffer[spriteNum].spriteMask.flags = flags;

    return true;
}

bool R12AddMatrix(
    struct R12Context *context,
    struct R12MatrixData *matrixData,
    int x,
    int y,
    unsigned char flags)
{
    if (*context->pCurrentDrawBufferAmount >= context->maxSprites)
        return false;

    int spriteNum = *context->pCurrentDrawBufferAmount;
    *context->pCurrentDrawBufferAmount = *context->pCurrentDrawBufferAmount + 1;

    int height = matrixData->spritesV * matrixData->spriteHeight;
    if (y + height >= 240)
        height = 240 - y;

    context->currentSpritesDrawBuffer[spriteNum].matrix.type = R12Matrix;
    context->currentSpritesDrawBuffer[spriteNum].matrix.startX = 0;
    context->currentSpritesDrawBuffer[spriteNum].matrix.startY = (y < 0) ? 0 : y;
    context->currentSpritesDrawBuffer[spriteNum].matrix.endY = y + height;
    context->currentSpritesDrawBuffer[spriteNum].matrix.x = x;
    context->currentSpritesDrawBuffer[spriteNum].matrix.y = y;
    context->currentSpritesDrawBuffer[spriteNum].matrix.matrixData = matrixData;
    context->currentSpritesDrawBuffer[spriteNum].matrix.flags = flags;

    return true;
}