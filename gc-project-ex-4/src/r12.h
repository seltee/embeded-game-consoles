#pragma once
#include <stdio.h>
#include "utils.h"

enum R12ContextFlag
{
    R12ContextFlagClearLine = 0x01 // if enabled each line will cleared with clearColor before drawing
};

enum R12SpriteType
{
    R12Sprite8bit = 0,
    R12Sprite4bit = 1,
    R12Sprite2bit = 2,
    R12SpriteMask = 3,
    R12Matrix = 4
};

enum R12SpriteFlag
{
    R12FlagTransparent = 0x01, // if 0 color shouldn't be drawn
    R12FlagHMirror = 0x02,     // mirrors sprite horizontally
    R12FlagVMirror = 0x04,     // mirrors sprite vertically
};

struct R12SpriteMainData
{
    enum R12SpriteType type;

    unsigned char startY;
    unsigned char endY;
    unsigned short startX;
};

struct R12MatrixData
{
    unsigned short int spritesH; // Amount of sprites horizontally
    unsigned short int spritesV; // Amount of sprites horizontally

    unsigned char spriteWidth;      // Width of each sprite
    unsigned char spriteHeight;     // Height of each sprite
    unsigned char bytesPerSpriteId; // Size of sprite id. 1 - up to 256 sprites, 2 - up to 65k
    unsigned char spriteListInLine; // How many sprites in line in the data sprite bitmap

    unsigned char bitsPerPixel;        // Bits per pixel of sprite in the bitmap
    unsigned char bytesPerSpriteWidth; // How many bytes in width of 1 sprite
    unsigned char ignoreSpriteZero;    // Will sprite 0 will be drawn
    unsigned char reserved2;           // Not used

    int spriteLineDataShift; // Bitmap line datashift (width in bytes of the bitmap)

    const unsigned short int *palette; // Matrix always uses palette even for 1 bit sprites
    const unsigned char *spriteList;   // Pointer to tileset bitmap
    const unsigned char *matrixData;   // Matrix grid, array of sprite IDs
};

struct R12SpritePalette
{
    enum R12SpriteType type;

    unsigned char startY;
    unsigned char endY;
    short startX;

    unsigned short width;
    unsigned char height;
    unsigned char flags;

    unsigned int widthDataShift;

    const unsigned short int *palette;

    const unsigned char *data;
};

struct R12SpriteMask
{
    enum R12SpriteType type;

    unsigned char startY;
    unsigned char endY;
    unsigned short startX;

    unsigned short width;
    unsigned char height;
    unsigned char flags;

    unsigned int widthDataShift;

    unsigned short colorBackground;
    unsigned short colorForeground;

    const unsigned char *data;
};

struct R12Matrix
{
    enum R12SpriteType type;

    unsigned char startY;
    unsigned char endY;
    unsigned short startX;

    unsigned short width;
    unsigned char height;
    unsigned char flags;

    int x;
    int y;

    struct R12MatrixData *matrixData;
};

union R12Sprite
{
    struct R12SpriteMainData spriteMainData;
    struct R12SpritePalette spritePalette;
    struct R12SpriteMask spriteMask;
    struct R12Matrix matrix;
};

struct R12Context
{
    union R12Sprite *spritesBuffer1;
    union R12Sprite *spritesBuffer2;

    unsigned short spritesBufferAmount1;
    unsigned short spritesBufferAmount2;

    union R12Sprite *currentSpritesDrawBuffer;
    union R12Sprite *currentSpritesRenderBuffer;
    unsigned short *pCurrentDrawBufferAmount;
    unsigned short *pCurrentRenderBufferAmount;

    unsigned int flags;

    unsigned short clearColor;
    unsigned short maxSprites;
};

void R12InitializeContext(struct R12Context *context, union R12Sprite *spritesBuffer1, union R12Sprite *spritesBuffer2, unsigned short maxSprites);
void R12SetContextFlags(struct R12Context *context, unsigned int flags);
void R12PrepareNewFrame(struct R12Context *context);
void R12FillLine(struct R12Context *context, int lineNumber, unsigned short int *pOutBuffer);
void R12FillLineSprite8bit(int line, struct R12SpritePalette *sprite, unsigned short *pOutBuffer);
void R12FillLineSprite4bit(int line, struct R12SpritePalette *sprite, unsigned short *pOutBuffer);
void R12FillLineSprite2bit(int line, struct R12SpritePalette *sprite, unsigned short *pOutBuffer);
void R12FillLineSpriteMask(int line, struct R12SpriteMask *sprite, unsigned short *pOutBuffer);
void R12FillLineMatrix(int line, struct R12Matrix *matrix, unsigned short *pOutBuffer);

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
    unsigned char flags);

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
    unsigned char flags);

bool R12AddMatrix(
    struct R12Context *context,
    struct R12MatrixData *matrixData,
    int x,
    int y,
    unsigned char flags);