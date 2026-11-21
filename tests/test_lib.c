#include "lib.h"

/* 某些编译器的优化构建默认定义 NDEBUG，测试必须始终执行断言。 */
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_float(void)
{
    const uint32_t patterns[] = {0u, UINT32_C(0x80000000), UINT32_C(0x3f800000),
                                 UINT32_C(0xc0200000), 1u, UINT32_C(0x7f7fffff),
                                 UINT32_C(0x7f800000), UINT32_C(0xff800000), UINT32_C(0x7fc12345)};
    assert(lib_float_to_u32(1.0f) == UINT32_C(0x3f800000));
    assert(lib_u32_to_float(UINT32_C(0xc0200000)) == -2.5f);
    for (size_t i = 0; i < sizeof(patterns) / sizeof(patterns[0]); ++i)
    {
        uint8_t data[4];
        float value = lib_u32_to_float(patterns[i]);
        assert(lib_float_to_u32(value) == patterns[i]);
        lib_write_float_be(data, value);
        assert(lib_read_u32_be(data) == patterns[i]);
        assert(lib_float_to_u32(lib_read_float_be(data)) == patterns[i]);
        lib_write_float_le(data, value);
        assert(lib_read_u32_le(data) == patterns[i]);
        assert(lib_float_to_u32(lib_read_float_le(data)) == patterns[i]);
    }
}

static void test_endian(void)
{
    const uint8_t be[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    const uint8_t le[] = {0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01};
    const uint16_t host = 1u;
    uint8_t guarded[10];
    /* 从偏移 1 开始验证非对齐缓冲区和写入边界。 */
    uint8_t *data = guarded + 1;
    memset(guarded, 0xa5, sizeof(guarded));
    assert(lib_is_little_endian() == (*(const unsigned char *)&host == 1u));
    assert(lib_bswap16(0x1234u) == 0x3412u);
    assert(lib_bswap32(UINT32_C(0x12345678)) == UINT32_C(0x78563412));
    assert(lib_bswap64(UINT64_C(0x0123456789abcdef)) == UINT64_C(0xefcdab8967452301));
    assert(lib_swap_words32(UINT32_C(0x12345678)) == UINT32_C(0x56781234));
    assert(lib_read_u16_be(be) == 0x0123u);
    assert(lib_read_u16_le(le) == 0xcdefu);
    assert(lib_read_u24_be(be) == UINT32_C(0x012345));
    assert(lib_read_u24_le(le) == UINT32_C(0xabcdef));
    assert(lib_read_u32_be(be) == UINT32_C(0x01234567));
    assert(lib_read_u32_le(le) == UINT32_C(0x89abcdef));
    assert(lib_read_u64_be(be) == UINT64_C(0x0123456789abcdef));
    assert(lib_read_u64_le(le) == UINT64_C(0x0123456789abcdef));
    lib_write_u16_be(data, 0x0123u);
    assert(memcmp(data, be, 2) == 0 && data[2] == 0xa5);
    lib_write_u16_le(data, 0xcdefu);
    assert(memcmp(data, le, 2) == 0 && data[2] == 0xa5);
    lib_write_u24_be(data, UINT32_C(0xff012345));
    assert(memcmp(data, be, 3) == 0 && data[3] == 0xa5);
    lib_write_u24_le(data, UINT32_C(0xffabcdef));
    assert(memcmp(data, le, 3) == 0 && data[3] == 0xa5);
    lib_write_u32_be(data, UINT32_C(0x01234567));
    assert(memcmp(data, be, 4) == 0 && data[4] == 0xa5);
    lib_write_u32_le(data, UINT32_C(0x89abcdef));
    assert(memcmp(data, le, 4) == 0 && data[4] == 0xa5);
    lib_write_u64_be(data, UINT64_C(0x0123456789abcdef));
    assert(memcmp(data, be, 8) == 0);
    lib_write_u64_le(data, UINT64_C(0x0123456789abcdef));
    assert(memcmp(data, le, 8) == 0);
    assert(guarded[0] == 0xa5 && guarded[9] == 0xa5);
}

static void test_checksums(void)
{
    const uint8_t vector[] = "123456789";
    uint8_t frame[] = {1, 3, 0, 0, 0, 10, 0, 0};
    const uint8_t sums[] = {0xff, 2, 3};
    assert(lib_crc16_modbus(vector, 9) == 0x4b37u);
    assert(lib_crc8_maxim(vector, 9, 0u) == 0xa1u);
    assert(lib_crc8_smbus(vector, 9) == 0xf4u);
    assert(lib_crc16_ccitt(vector, 9, 0u) == 0x31c3u);
    assert(lib_crc16_ccitt(vector, 9, 0xffffu) == 0x29b1u);
    assert(lib_crc32_ieee(vector, 9) == UINT32_C(0xcbf43926));
    assert(lib_crc16_modbus(frame, 6) == 0xcdc5u);
    lib_write_u16_le(frame + 6, lib_crc16_modbus(frame, 6));
    assert(frame[6] == 0xc5 && frame[7] == 0xcd);
    assert(lib_crc16_modbus(frame, sizeof(frame)) == 0u);
    assert(lib_sum8(sums, 3) == 4u && lib_sum16(sums, 3) == 260u);
    assert(lib_xor8(sums, 3) == 0xfeu);
    assert(lib_crc16_modbus(NULL, 0) == 0xffffu);
    assert(lib_crc16_ccitt(NULL, 0, 0x5555u) == 0x5555u);
    assert(lib_crc8_maxim(NULL, 0, 0x55u) == 0x55u);
    assert(lib_crc8_smbus(NULL, 0) == 0u);
    assert(lib_crc32_ieee(NULL, 0) == 0u);
    assert(lib_sum8(NULL, 0) == 0u && lib_sum16(NULL, 0) == 0u && lib_xor8(NULL, 0) == 0u);
}

static void test_bcd_hex(void)
{
    uint8_t byte = 0x55u;
    uint32_t value = 123u;
    uint8_t raw[] = {0x00, 0xa5, 0xff};
    uint8_t output[] = {0x11, 0x22, 0x33};
    char text[7];
    for (unsigned int i = 0u; i < 256u; ++i)
    {
        int valid = (i >> 4) < 10u && (i & 15u) < 10u;
        byte = 0x55u;
        assert(lib_bcd_to_u8((uint8_t)i, &byte) == valid);
        if (valid)
        {
            uint8_t bcd;
            assert(lib_u8_to_bcd(byte, &bcd) && bcd == i);
        }
        else
        {
            assert(byte == 0x55u);
        }
    }
    assert(!lib_u8_to_bcd(100u, &byte));
    assert(!lib_u8_to_bcd(0u, NULL) && !lib_bcd_to_u8(0u, NULL));
    assert(lib_u32_to_bcd(12345678u, &value) && value == UINT32_C(0x12345678));
    assert(lib_bcd_to_u32(value, &value) && value == 12345678u);
    assert(lib_u32_to_bcd(99999999u, &value) && value == UINT32_C(0x99999999));
    assert(!lib_u32_to_bcd(100000000u, &value) && value == UINT32_C(0x99999999));
    assert(!lib_bcd_to_u32(UINT32_C(0x123a5678), &value) && value == UINT32_C(0x99999999));
    assert(!lib_u32_to_bcd(0u, NULL) && !lib_bcd_to_u32(0u, NULL));
    assert(lib_hex_digit('f') == 15 && lib_hex_digit('A') == 10 && lib_hex_digit('g') == -1);
    assert(lib_bytes_to_hex(raw, 3, text, sizeof(text)) && strcmp(text, "00A5FF") == 0);
    assert(!lib_bytes_to_hex(raw, 3, text, 6) && strcmp(text, "00A5FF") == 0);
    assert(!lib_bytes_to_hex(raw, SIZE_MAX, text, sizeof(text)));
    assert(lib_hex_to_bytes("00a5Ff", 6, output, 3) && memcmp(raw, output, 3) == 0);
    assert(!lib_hex_to_bytes("123", 3, output, 3));
    assert(!lib_hex_to_bytes("11ZZ", 4, output, 3) && memcmp(raw, output, 3) == 0);
    assert(!lib_hex_to_bytes("112233", 6, output, 2) && memcmp(raw, output, 3) == 0);
    assert(!lib_hex_to_bytes(NULL, 2, output, 3));
    assert(lib_hex_to_bytes(NULL, 0, NULL, 0));
    assert(lib_bytes_to_hex(NULL, 0, text, 1) && text[0] == '\0');
}

static void test_buffer_math(void)
{
    uint8_t data[] = {1, 0, 3, 4, 5};
    const uint8_t needle[] = {0, 3};
    const char *text = "xxChArGe.yy";
    size_t length = sizeof(data);
    const int32_t numbers[] = {0, INT32_MIN, 123, INT32_MAX, -9};
    int32_t low = 99;
    int32_t high = 99;
    assert(lib_mem_find(data, length, needle, sizeof(needle)) == data + 1);
    assert(lib_mem_find(data, length, "", 0) == data);
    assert(lib_mem_find(data, length, "abcdef", 6) == NULL);
    assert(lib_mem_find(NULL, 1, needle, 2) == NULL);
    assert(lib_str_find_nocase(text, "charge") == text + 2);
    assert(lib_str_find_nocase(text, "charge.yyz") == NULL);
    assert(lib_str_find_nocase(text, "") == text);
    assert(!lib_buffer_remove(data, &length, 4, 2) && length == 5 && data[4] == 5);
    assert(!lib_buffer_remove(data, &length, SIZE_MAX, 1));
    assert(lib_buffer_remove(data, &length, 1, 2) && length == 3);
    assert(data[0] == 1 && data[1] == 4 && data[2] == 5);
    lib_reverse_bytes(data, length);
    assert(data[0] == 5 && data[1] == 4 && data[2] == 1);
    lib_reverse_bytes(NULL, 0);
    assert(lib_i32_minmax(numbers, 5, &low, &high) && low == INT32_MIN && high == INT32_MAX);
    assert(!lib_i32_minmax(numbers, 0, &low, &high) && low == INT32_MIN);
    assert(!lib_i32_minmax(numbers, 5, &low, &low));
    assert(lib_clamp_i32(INT32_MIN, -10, 10) == -10);
    assert(lib_clamp_i32(123, -10, 10) == 10 && lib_clamp_i32(0, -10, 10) == 0);
    assert(lib_lowest_set_bit(0u) == -1 && lib_lowest_set_bit(UINT32_C(0x80000000)) == 31);
    assert(lib_lowest_set_bit(0x28u) == 3);
    assert(lib_popcount32(UINT32_MAX) == 32u && lib_popcount32(0u) == 0u);
}

static void test_time(void)
{
    lib_datetime value = {1970, 1, 1, 0, 0, 0};
    uint32_t stamp = 99u;
    assert(lib_elapsed_u32(UINT32_MAX - 4u, 3u) == 8u);
    assert(lib_timeout_expired(UINT32_MAX - 4u, 3u, 8u));
    assert(!lib_timeout_expired(UINT32_MAX - 4u, 3u, 9u));
    assert(lib_timeout_expired(9u, 9u, 0u));
    assert(lib_datetime_to_unix(&value, &stamp) && stamp == 0u);
    value.year = 2000;
    value.month = 2;
    value.day = 29;
    assert(lib_datetime_to_unix(&value, &stamp) && stamp == UINT32_C(951782400));
    value.year = 2100;
    assert(!lib_datetime_is_valid(&value));
    assert(!lib_datetime_to_unix(&value, &stamp) && stamp == UINT32_C(951782400));
    assert(lib_unix_to_datetime(UINT32_MAX, &value));
    assert(value.year == 2106 && value.month == 2 && value.day == 7);
    assert(value.hour == 6 && value.minute == 28 && value.second == 15);
    assert(lib_datetime_to_unix(&value, &stamp) && stamp == UINT32_MAX);
    value.second = 16;
    assert(!lib_datetime_to_unix(&value, &stamp) && stamp == UINT32_MAX);
    value.month = 0;
    assert(!lib_datetime_is_valid(&value));
    assert(!lib_datetime_is_valid(NULL) && !lib_unix_to_datetime(0u, NULL));
    /* 覆盖整个 uint32_t 时间范围内的月/年边界。 */
    for (uint64_t seconds = 0u; seconds <= UINT32_MAX; seconds += 86399u)
    {
        assert(lib_unix_to_datetime((uint32_t)seconds, &value));
        assert(lib_datetime_to_unix(&value, &stamp) && stamp == seconds);
    }
}

int main(void)
{
    test_float();
    test_endian();
    test_checksums();
    test_bcd_hex();
    test_buffer_math();
    test_time();
    puts("All utility library tests passed.");
    return 0;
}
