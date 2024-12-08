#pragma once
#define SWITCH_BYTES(x) ((x >> 8) | (x << 8))

#ifndef bool
#define bool _Bool
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif