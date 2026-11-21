#include "lib_convert.h"

#include <float.h>
#include <limits.h>
#include <string.h>

#if CHAR_BIT != 8 || FLT_RADIX != 2 || FLT_MANT_DIG != 24 || FLT_MAX_EXP != 128 || FLT_MIN_EXP != -125
#error "lib.c requires 8-bit bytes and IEEE 754 binary32 float"
#endif

/* C99 兼容的编译期长度检查。目标平台须采用 IEEE 754 浮点位布局。 */
typedef char lib_float_size_check[(sizeof(float) == sizeof(uint32_t)) ? 1 : -1];

uint32_t lib_float_to_u32(float value)
{
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

float lib_u32_to_float(uint32_t bits)
{
    float value;
    memcpy(&value, &bits, sizeof(value));
    return value;
}
