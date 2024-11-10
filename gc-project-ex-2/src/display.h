#pragma once

void displaySendCmd(unsigned char command);
void displaySendData8(unsigned char byte);
void displaySendData16(unsigned short data);
void displaySetCol(unsigned short startCol, unsigned short endCol);
void displaySetPage(unsigned short startPage, unsigned short endPage);
void displaySetXY(unsigned short positionX, unsigned short positionY);
void displayClear(unsigned short color);
void displayInit();

void displayFillBuffer(unsigned char colorNum);
void displayFillRect(short int positionX, short int positionY, short int width, short int height, unsigned char colorNum);
void displaySendBuffer();

void displaySwitchOffCS();