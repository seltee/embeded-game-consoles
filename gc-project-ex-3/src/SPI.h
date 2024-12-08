#pragma once
#include "hardware/spi.h"

inline unsigned char SPI0Send8(unsigned char byte)
{
    unsigned char ret = 0;
    spi_write_read_blocking(spi0, &byte, &ret, 1);
    return ret;
}

inline unsigned short SPI0Send16(unsigned short data)
{
    unsigned short ret = 0;
    spi_write_read_blocking(spi0, (unsigned char*)&data, (unsigned char*)&ret, 2);
    return ret;
}
