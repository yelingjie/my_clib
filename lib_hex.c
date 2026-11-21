#include "lib_hex.h"

int lib_hex_digit(char value)
{
    if (value >= '0' && value <= '9')
    {
        return value - '0';
    }
    if (value >= 'a' && value <= 'f')
    {
        return value - 'a' + 10;
    }
    if (value >= 'A' && value <= 'F')
    {
        return value - 'A' + 10;
    }
    return -1;
}

int lib_bytes_to_hex(const uint8_t *data, size_t length, char *out, size_t out_size)
{
    static const char digits[] = "0123456789ABCDEF";
    /* 先用除法比较容量，避免 length * 2 + 1 溢出。 */
    if (out == NULL || out_size == 0u || length > (out_size - 1u) / 2u ||
        (length != 0u && data == NULL))
    {
        return 0;
    }
    for (size_t i = 0; i < length; ++i)
    {
        out[2u * i] = digits[data[i] >> 4];
        out[2u * i + 1u] = digits[data[i] & 0x0fu];
    }
    out[length * 2u] = '\0';
    return 1;
}

int lib_hex_to_bytes(const char *text, size_t text_length, uint8_t *out, size_t out_size)
{
    if ((text_length & 1u) != 0u || out_size < text_length / 2u ||
        (text_length != 0u && (text == NULL || out == NULL)))
    {
        return 0;
    }
    /* 先完整验证，再写入，避免失败时留下部分转换结果。 */
    for (size_t i = 0; i < text_length; ++i)
    {
        if (lib_hex_digit(text[i]) < 0)
        {
            return 0;
        }
    }
    for (size_t i = 0; i < text_length / 2u; ++i)
    {
        out[i] = (uint8_t)(((unsigned int)lib_hex_digit(text[2u * i]) << 4) |
                          (unsigned int)lib_hex_digit(text[2u * i + 1u]));
    }
    return 1;
}
