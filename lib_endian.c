#include "lib_endian.h"
#include "lib_convert.h"

int lib_is_little_endian(void)
{
    const uint16_t value = 1u;
    return *(const unsigned char *)&value == 1u;
}

uint16_t lib_bswap16(uint16_t value)
{
    return (uint16_t)(((uint32_t)value << 8) | ((uint32_t)value >> 8));
}

uint32_t lib_bswap32(uint32_t value)
{
    return ((value & UINT32_C(0x000000ff)) << 24) |
           ((value & UINT32_C(0x0000ff00)) << 8) |
           ((value & UINT32_C(0x00ff0000)) >> 8) |
           ((value & UINT32_C(0xff000000)) >> 24);
}

uint64_t lib_bswap64(uint64_t value)
{
    return ((uint64_t)lib_bswap32((uint32_t)value) << 32) |
           (uint64_t)lib_bswap32((uint32_t)(value >> 32));
}

uint32_t lib_swap_words32(uint32_t value)
{
    return (value << 16) | (value >> 16);
}

uint16_t lib_read_u16_be(const uint8_t *data)
{
    return (uint16_t)(((uint32_t)data[0] << 8) | (uint32_t)data[1]);
}

uint16_t lib_read_u16_le(const uint8_t *data)
{
    return (uint16_t)((uint32_t)data[0] | ((uint32_t)data[1] << 8));
}

uint32_t lib_read_u24_be(const uint8_t *data)
{
    return ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[2];
}

uint32_t lib_read_u24_le(const uint8_t *data)
{
    return (uint32_t)data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16);
}

uint32_t lib_read_u32_be(const uint8_t *data)
{
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) |
           ((uint32_t)data[2] << 8) | (uint32_t)data[3];
}

uint32_t lib_read_u32_le(const uint8_t *data)
{
    return (uint32_t)data[0] | ((uint32_t)data[1] << 8) |
           ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
}

uint64_t lib_read_u64_be(const uint8_t *data)
{
    return ((uint64_t)lib_read_u32_be(data) << 32) |
           (uint64_t)lib_read_u32_be(data + 4);
}

uint64_t lib_read_u64_le(const uint8_t *data)
{
    return (uint64_t)lib_read_u32_le(data) |
           ((uint64_t)lib_read_u32_le(data + 4) << 32);
}

void lib_write_u16_be(uint8_t *data, uint16_t value)
{
    data[0] = (uint8_t)(value >> 8);
    data[1] = (uint8_t)value;
}

void lib_write_u16_le(uint8_t *data, uint16_t value)
{
    data[0] = (uint8_t)value;
    data[1] = (uint8_t)(value >> 8);
}

void lib_write_u24_be(uint8_t *data, uint32_t value)
{
    data[0] = (uint8_t)(value >> 16);
    data[1] = (uint8_t)(value >> 8);
    data[2] = (uint8_t)value;
}

void lib_write_u24_le(uint8_t *data, uint32_t value)
{
    data[0] = (uint8_t)value;
    data[1] = (uint8_t)(value >> 8);
    data[2] = (uint8_t)(value >> 16);
}

void lib_write_u32_be(uint8_t *data, uint32_t value)
{
    data[0] = (uint8_t)(value >> 24);
    data[1] = (uint8_t)(value >> 16);
    data[2] = (uint8_t)(value >> 8);
    data[3] = (uint8_t)value;
}

void lib_write_u32_le(uint8_t *data, uint32_t value)
{
    data[0] = (uint8_t)value;
    data[1] = (uint8_t)(value >> 8);
    data[2] = (uint8_t)(value >> 16);
    data[3] = (uint8_t)(value >> 24);
}

void lib_write_u64_be(uint8_t *data, uint64_t value)
{
    lib_write_u32_be(data, (uint32_t)(value >> 32));
    lib_write_u32_be(data + 4, (uint32_t)value);
}

void lib_write_u64_le(uint8_t *data, uint64_t value)
{
    lib_write_u32_le(data, (uint32_t)value);
    lib_write_u32_le(data + 4, (uint32_t)(value >> 32));
}

float lib_read_float_be(const uint8_t *data)
{
    return lib_u32_to_float(lib_read_u32_be(data));
}

float lib_read_float_le(const uint8_t *data)
{
    return lib_u32_to_float(lib_read_u32_le(data));
}

void lib_write_float_be(uint8_t *data, float value)
{
    lib_write_u32_be(data, lib_float_to_u32(value));
}

void lib_write_float_le(uint8_t *data, float value)
{
    lib_write_u32_le(data, lib_float_to_u32(value));
}
