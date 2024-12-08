#pragma once
#include "r12.h"

void displaySetCol(unsigned short startCol, unsigned short endCol);
void displaySetPage(unsigned short startPage, unsigned short endPage);
void displaySetXY(unsigned short positionX, unsigned short positionY);
void displayClear(unsigned short color);
void displayInit();

void displayRenderR12(struct R12Context *context);
void displaySendDataDMA(unsigned char *data, unsigned int length);

void displaySwitchOffCS();