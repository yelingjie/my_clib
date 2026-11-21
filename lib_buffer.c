#include "lib_buffer.h"

#include <string.h>

void lib_reverse_bytes(void *data, size_t length)
{
    unsigned char *bytes = (unsigned char *)data;
    for (size_t i = 0; i < length / 2u; ++i)
    {
        unsigned char temp = bytes[i];
        bytes[i] = bytes[length - 1u - i];
        bytes[length - 1u - i] = temp;
    }
}

const void *lib_mem_find(const void *data, size_t length, const void *needle, size_t needle_length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    if ((length != 0u && data == NULL) || (needle_length != 0u && needle == NULL) ||
        needle_length > length)
    {
        return NULL;
    }
    if (needle_length == 0u)
    {
        return data;
    }
    for (size_t i = 0; i <= length - needle_length; ++i)
    {
        if (memcmp(bytes + i, needle, needle_length) == 0)
        {
            return bytes + i;
        }
    }
    return NULL;
}

static unsigned char ascii_lower(unsigned char value)
{
    if (value >= 'A' && value <= 'Z')
    {
        return (unsigned char)(value + ('a' - 'A'));
    }
    return value;
}

const char *lib_str_find_nocase(const char *text, const char *needle)
{
    if (*needle == '\0')
    {
        return text;
    }
    for (; *text != '\0'; ++text)
    {
        size_t i = 0u;
        while (text[i] != '\0' && needle[i] != '\0' &&
               ascii_lower((unsigned char)text[i]) == ascii_lower((unsigned char)needle[i]))
        {
            ++i;
        }
        if (needle[i] == '\0')
        {
            return text;
        }
    }
    return NULL;
}

int lib_buffer_remove(void *data, size_t *length, size_t offset, size_t count)
{
    unsigned char *bytes = (unsigned char *)data;
    if (length == NULL || (*length != 0u && data == NULL) || offset > *length || count > *length - offset)
    {
        return 0;
    }
    if (count != 0u)
    {
        memmove(bytes + offset, bytes + offset + count, *length - offset - count);
        *length -= count;
    }
    return 1;
}
