#include "lib_checksum.h"

/* 与充电桩 GetModbusCrc16(0xffff, ...) 的 CRC 数值约定一致。 */
uint16_t lib_crc16_modbus(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xffffu;
    for (size_t i = 0; i < length; ++i)
    {
        crc ^= data[i];
        for (unsigned int bit = 0; bit < 8u; ++bit)
        {
            crc = (uint16_t)((crc >> 1) ^ ((crc & 1u) ? 0xa001u : 0u));
        }
    }
    return crc;
}

uint8_t lib_crc8_maxim(const uint8_t *data, size_t length, uint8_t init)
{
    uint8_t crc = init;
    for (size_t i = 0; i < length; ++i)
    {
        crc ^= data[i];
        for (unsigned int bit = 0; bit < 8u; ++bit)
        {
            crc = (uint8_t)((crc >> 1) ^ ((crc & 1u) ? 0x8cu : 0u));
        }
    }
    return crc;
}

uint8_t lib_crc8_smbus(const uint8_t *data, size_t length)
{
    uint8_t crc = 0u;
    for (size_t i = 0; i < length; ++i)
    {
        crc ^= data[i];
        for (unsigned int bit = 0; bit < 8u; ++bit)
        {
            crc = (uint8_t)(((uint32_t)crc << 1) ^ ((crc & 0x80u) ? 0x07u : 0u));
        }
    }
    return crc;
}

uint16_t lib_crc16_ccitt(const uint8_t *data, size_t length, uint16_t init)
{
    uint16_t crc = init;
    for (size_t i = 0; i < length; ++i)
    {
        crc ^= (uint16_t)((uint32_t)data[i] << 8);
        for (unsigned int bit = 0; bit < 8u; ++bit)
        {
            crc = (uint16_t)(((uint32_t)crc << 1) ^ ((crc & 0x8000u) ? 0x1021u : 0u));
        }
    }
    return crc;
}

uint32_t lib_crc32_ieee(const uint8_t *data, size_t length)
{
    uint32_t crc = UINT32_MAX;
    for (size_t i = 0; i < length; ++i)
    {
        crc ^= data[i];
        for (unsigned int bit = 0; bit < 8u; ++bit)
        {
            crc = (crc >> 1) ^ ((crc & 1u) ? UINT32_C(0xedb88320) : 0u);
        }
    }
    return crc ^ UINT32_MAX;
}

uint8_t lib_sum8(const uint8_t *data, size_t length)
{
    uint8_t sum = 0u;
    for (size_t i = 0; i < length; ++i)
    {
        sum = (uint8_t)((unsigned int)sum + data[i]);
    }
    return sum;
}

uint16_t lib_sum16(const uint8_t *data, size_t length)
{
    uint16_t sum = 0u;
    for (size_t i = 0; i < length; ++i)
    {
        sum = (uint16_t)((uint32_t)sum + data[i]);
    }
    return sum;
}

uint8_t lib_xor8(const uint8_t *data, size_t length)
{
    uint8_t result = 0u;
    for (size_t i = 0; i < length; ++i)
    {
        result ^= data[i];
    }
    return result;
}
