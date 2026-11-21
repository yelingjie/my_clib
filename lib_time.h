#ifndef EMBEDDED_LIB_TIME_H
#define EMBEDDED_LIB_TIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    uint16_t year; /* 完整年份，如 2026。 */
    uint8_t month; /* 1..12 */
    uint8_t day;   /* 1..31，按月份和闰年检查。 */
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} lib_datetime;

/* 32 位递增计数器的无符号差值，允许跨一次回绕边界。
 * 调用者保证实际间隔小于 2^32 个计数；三参数使用相同单位（如 ms）。
 * 到达超时时间即返回 1；timeout == 0 时立即超时。无需 RTOS。
 */
uint32_t lib_elapsed_u32(uint32_t start, uint32_t now);
int lib_timeout_expired(uint32_t start, uint32_t now, uint32_t timeout);

/* UTC 公历，不使用本地时区、静态返回缓冲区或 libc 的 time_t。
 * calendar 校验支持 1970..2106；不支持闰秒（second 必须为 0..59）。
 * Unix 秒范围：1970-01-01 00:00:00 .. 2106-02-07 06:28:15。
 * 成功返回 1；空指针、非法日期或溢出返回 0，输出保持不变。
 */
int lib_datetime_is_valid(const lib_datetime *value);
int lib_datetime_to_unix(const lib_datetime *value, uint32_t *timestamp);
int lib_unix_to_datetime(uint32_t timestamp, lib_datetime *value);

#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_TIME_H */
