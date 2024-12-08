#pragma once
#include "pico/stdlib.h"

unsigned char SDSendCommand(unsigned char cmd, unsigned int arg);

bool SDInit();
void SDReadSector();

void SDCardSwitchOffCS();