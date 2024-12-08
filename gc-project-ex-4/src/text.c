#include "text.h"

const unsigned char empty[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
};

const unsigned char letterA[] = {
    0b00010000,
    0b00101000,
    0b00101000,
    0b00101000,
    0b01000100,
    0b01000100,
    0b01111100,
    0b10000010,
    0b10000010,
    0b10000010,
    0b00000000,
    0b00000000,
};

const unsigned char letterB[] = {
    0b11111100,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b11111100,
    0b10000010,
    0b10000010,
    0b10000010,
    0b11111100,
    0b00000000,
    0b00000000,
};

const unsigned char letterC[] = {
    0b00111110,
    0b01000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b01000000,
    0b00111110,
    0b00000000,
    0b00000000,
};

const unsigned char letterD[] = {
    0b11111000,
    0b10000100,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000100,
    0b11111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterE[] = {
    0b11111110,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111110,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111110,
    0b00000000,
    0b00000000,
};

const unsigned char letterF[] = {
    0b11111110,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111110,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
};

const unsigned char letterG[] = {
    0b00111110,
    0b01000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10001110,
    0b10000010,
    0b01000010,
    0b00111100,
    0b00000000,
    0b00000000,
};

const unsigned char letterJ[] = {
    0b11111110,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b10000010,
    0b01000100,
    0b00111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterH[] = {
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b11111110,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b00000000,
    0b00000000,
};

const unsigned char letterI[] = {
    0b01111100,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b01111100,
    0b00000000,
    0b00000000,
};

const unsigned char letterK[] = {
    0b10000010,
    0b10000100,
    0b10001000,
    0b10010000,
    0b10100000,
    0b11000000,
    0b10100000,
    0b10010000,
    0b10001000,
    0b10000100,
    0b00000000,
    0b00000000,
};

const unsigned char letterL[] = {
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111110,
    0b00000000,
    0b00000000,
};

const unsigned char letterM[] = {
    0b10000010,
    0b11000110,
    0b10101010,
    0b10101010,
    0b10101010,
    0b10101010,
    0b10010010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b00000000,
    0b00000000,
};

const unsigned char letterN[] = {
    0b10000010,
    0b11000010,
    0b10100010,
    0b10100010,
    0b10010010,
    0b10010010,
    0b10001010,
    0b10001010,
    0b10000110,
    0b10000010,
    0b00000000,
    0b00000000,
};

const unsigned char letterO[] = {
    0b00111000,
    0b01000100,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b01000100,
    0b00111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterP[] = {
    0b11111000,
    0b10000100,
    0b10000010,
    0b10000010,
    0b10000100,
    0b11111000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
};

const unsigned char letterQ[] = {
    0b00111000,
    0b01000100,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b01000100,
    0b00111000,
    0b00001100,
    0b00000010,
    0b00000000,
};

const unsigned char letterR[] = {
    0b11111000,
    0b10000100,
    0b10000010,
    0b10000100,
    0b11111000,
    0b10100000,
    0b10010000,
    0b10001000,
    0b10000100,
    0b10000010,
    0b00000000,
    0b00000000,
};

const unsigned char letterS[] = {
    0b00111110,
    0b01000000,
    0b10000000,
    0b10000000,
    0b01000000,
    0b00111000,
    0b00000100,
    0b00000010,
    0b00000100,
    0b11111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterT[] = {
    0b11111110,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000,
    0b00000000,
};

const unsigned char letterU[] = {
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b01000100,
    0b00111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterV[] = {
    0b10000010,
    0b10000010,
    0b01000100,
    0b01000100,
    0b01000100,
    0b00101000,
    0b00101000,
    0b00101000,
    0b00010000,
    0b00010000,
    0b00000000,
    0b00000000,
};

const unsigned char letterW[] = {
    0b10000010,
    0b10000010,
    0b10000010,
    0b10000010,
    0b10010010,
    0b10010010,
    0b10101010,
    0b10101010,
    0b10101010,
    0b01000100,
    0b00000000,
    0b00000000,
};

const unsigned char letterX[] = {
    0b10000100,
    0b10000100,
    0b01001000,
    0b01001000,
    0b00110000,
    0b00110000,
    0b01001000,
    0b01001000,
    0b10000100,
    0b10000100,
    0b00000000,
    0b00000000,
};

const unsigned char letterY[] = {
    0b10000010,
    0b10000010,
    0b01000100,
    0b01000100,
    0b00101000,
    0b00101000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000,
    0b00000000,
};

const unsigned char letterZ[] = {
    0b11111110,
    0b01000000,
    0b00100000,
    0b00100000,
    0b00010000,
    0b00010000,
    0b00001000,
    0b00001000,
    0b00000100,
    0b11111110,
    0b00000000,
    0b00000000,
};

const unsigned char letterExclamation[] = {
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
};

const unsigned char letterQuestion[] = {
    0b01111000,
    0b10000100,
    0b00000010,
    0b00000010,
    0b00000100,
    0b11111000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b10000000,
    0b00000000,
    0b00000000,
};

const unsigned char letterDot[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b00000000,
    0b00000000,
};

const unsigned char letterComma[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000000,
    0b01000000,
    0b00000000,
};

const unsigned char letterZero[] = {
    0b00111000,
    0b01000100,
    0b10000010,
    0b10100010,
    0b10010010,
    0b10010010,
    0b10001010,
    0b10000010,
    0b01000100,
    0b00111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterOne[] = {
    0b00100000,
    0b01100000,
    0b10100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b11111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterTwo[] = {
    0b00111000,
    0b01000100,
    0b10000010,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b11111110,
    0b00000000,
    0b00000000,
};

const unsigned char letterThree[] = {
    0b01111000,
    0b10000100,
    0b00000010,
    0b00000100,
    0b01111000,
    0b00000100,
    0b00000010,
    0b00000010,
    0b10000100,
    0b01111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterFour[] = {
    0b00000100,
    0b00001100,
    0b00010100,
    0b00100100,
    0b01000100,
    0b11111110,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000100,
    0b00000000,
    0b00000000,
};

const unsigned char letterFive[] = {
    0b11111110,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111000,
    0b00000100,
    0b00000010,
    0b00000010,
    0b00000100,
    0b11111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterSix[] = {
    0b00111110,
    0b01000000,
    0b10000000,
    0b10000000,
    0b11111000,
    0b10000100,
    0b10000010,
    0b10000010,
    0b01000100,
    0b00111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterSeven[] = {
    0b11111110,
    0b00000010,
    0b00000100,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b00000000,
    0b00000000,
};

const unsigned char letterEight[] = {
    0b00111000,
    0b01000100,
    0b10000010,
    0b01000100,
    0b00111000,
    0b01000100,
    0b10000010,
    0b10000010,
    0b10000100,
    0b01111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterNine[] = {
    0b00111000,
    0b01000100,
    0b10000010,
    0b10000010,
    0b01000010,
    0b00111110,
    0b00000010,
    0b00000010,
    0b10000100,
    0b01111000,
    0b00000000,
    0b00000000,
};

const unsigned char letterSlash[] = {
    0b00001000,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b01000000,
    0b01000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
};

const unsigned char *getLetter(char letter)
{
    if (letter >= 'a' && letter <= 'z')
        letter -= 0x20;

    switch (letter)
    {
    case 'A':
        return letterA;
    case 'B':
        return letterB;
    case 'C':
        return letterC;
    case 'D':
        return letterD;
    case 'E':
        return letterE;
    case 'F':
        return letterF;
    case 'G':
        return letterG;
    case 'J':
        return letterJ;
    case 'H':
        return letterH;
    case 'I':
        return letterI;
    case 'K':
        return letterK;
    case 'L':
        return letterL;
    case 'M':
        return letterM;
    case 'N':
        return letterN;
    case 'O':
        return letterO;
    case 'P':
        return letterP;
    case 'Q':
        return letterQ;
    case 'R':
        return letterR;
    case 'S':
        return letterS;
    case 'T':
        return letterT;
    case 'U':
        return letterU;
    case 'V':
        return letterV;
    case 'W':
        return letterW;
    case 'X':
        return letterX;
    case 'Y':
        return letterY;
    case 'Z':
        return letterZ;
    case '!':
        return letterExclamation;
    case '?':
        return letterQuestion;
    case '.':
        return letterDot;
    case ',':
        return letterComma;
    case '0':
        return letterZero;
    case '1':
        return letterOne;
    case '2':
        return letterTwo;
    case '3':
        return letterThree;
    case '4':
        return letterFour;
    case '5':
        return letterFive;
    case '6':
        return letterSix;
    case '7':
        return letterSeven;
    case '8':
        return letterEight;
    case '9':
        return letterNine;
    case '/':
        return letterSlash;

    default:
        return empty;
    }
}

void addString(const char *string, int x, int y, unsigned short int color, struct R12Context *context)
{
    while (*string)
    {
        const unsigned char *data = getLetter(*string);
        R12AddSpriteMask(context, x, y, 8, 12, 1, 0, color, data, R12FlagTransparent);
        x += 8;
        string++;
    }
}