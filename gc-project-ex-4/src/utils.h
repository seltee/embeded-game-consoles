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

extern const int _sins[];
inline int getSin(int angleDeg)
{
    return _sins[(angleDeg % 360 + 360) % 360];
}

extern const int _coss[];
inline int getCos(int angleDeg)
{
    return _coss[(angleDeg % 360 + 360) % 360];
}
