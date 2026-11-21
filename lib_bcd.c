#include "lib_bcd.h"

int lib_u8_to_bcd(uint8_t value, uint8_t *bcd)
{
    if (bcd == 0 || value > 99u)
    {
        return 0;
    }
    *bcd = (uint8_t)(((value / 10u) << 4) | (value % 10u));
    return 1;
}

int lib_bcd_to_u8(uint8_t bcd, uint8_t *value)
{
    if (value == 0 || (bcd >> 4) > 9u || (bcd & 0x0fu) > 9u)
    {
        return 0;
    }
    *value = (uint8_t)((bcd >> 4) * 10u + (bcd & 0x0fu));
    return 1;
}

int lib_u32_to_bcd(uint32_t value, uint32_t *bcd)
{
    uint32_t result = 0u;
    if (bcd == 0 || value > UINT32_C(99999999))
    {
        return 0;
    }
    for (unsigned int shift = 0; shift < 32u; shift += 4u)
    {
        result |= (value % 10u) << shift;
        value /= 10u;
    }
    *bcd = result;
    return 1;
}

int lib_bcd_to_u32(uint32_t bcd, uint32_t *value)
{
    uint32_t result = 0u;
    if (value == 0)
    {
        return 0;
    }
    for (unsigned int shift = 28u;; shift -= 4u)
    {
        uint32_t digit = (bcd >> shift) & 0x0fu;
        if (digit > 9u)
        {
            return 0;
        }
        result = result * 10u + digit;
        if (shift == 0u)
        {
            break;
        }
    }
    *value = result;
    return 1;
}
