#pragma once
#include "r12.h"

// Letters are 8x12 or 12 bytes
const unsigned char *getLetter(char letter);
void addString(const char *string, int x, int y, unsigned short int color, struct R12Context *context);