#include "lib_math.h"

int lib_lowest_set_bit(uint32_t value)
{
    int offset = 0;
    if (value == 0u)
    {
        return -1;
    }
    while ((value & 1u) == 0u)
    {
        value >>= 1;
        ++offset;
    }
    return offset;
}

unsigned int lib_popcount32(uint32_t value)
{
    unsigned int count = 0u;
    while (value != 0u)
    {
        value &= value - 1u;
        ++count;
    }
    return count;
}

int lib_i32_minmax(const int32_t *data, size_t length, int32_t *minimum, int32_t *maximum)
{
    int32_t low;
    int32_t high;
    if (data == NULL || length == 0u || minimum == NULL || maximum == NULL || minimum == maximum)
    {
        return 0;
    }
    low = data[0];
    high = data[0];
    for (size_t i = 1u; i < length; ++i)
    {
        if (data[i] < low)
        {
            low = data[i];
        }
        if (data[i] > high)
        {
            high = data[i];
        }
    }
    *minimum = low;
    *maximum = high;
    return 1;
}

int32_t lib_clamp_i32(int32_t value, int32_t minimum, int32_t maximum)
{
    if (value < minimum)
    {
        return minimum;
    }
    if (value > maximum)
    {
        return maximum;
    }
    return value;
}
