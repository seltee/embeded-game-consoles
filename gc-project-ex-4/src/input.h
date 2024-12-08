#pragma once
#include "config.h"
#include "utils.h"

enum InputButtons
{
    ButtonA = 0x0001,
    ButtonB = 0x0002,
    ButtonX = 0x0004,
    ButtonY = 0x0008,
    ButtonLT = 0x0010,
    BUttonRT = 0x0020,
    ButtonSelect = 0x0040,
    ButtonStart = 0x0080
};

// Stick
void inputInitBaseLine();
int inputGetStickX();
int inputGetStickY();

// Buttons
void inputUpdateButtonsData();
bool inputIsButtonPressed(unsigned short button);
unsigned short inputGetButtonState();
