#include "lib_time.h"

static int is_leap_year(uint16_t year)
{
    return (year % 4u == 0u) && ((year % 100u != 0u) || (year % 400u == 0u));
}

/* 内部调用保证 month 位于 1..12。 */
static uint8_t days_in_month(uint16_t year, uint8_t month)
{
    static const uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (uint8_t)(days[month - 1u] + ((month == 2u && is_leap_year(year)) ? 1u : 0u));
}

uint32_t lib_elapsed_u32(uint32_t start, uint32_t now)
{
    return now - start;
}

int lib_timeout_expired(uint32_t start, uint32_t now, uint32_t timeout)
{
    return lib_elapsed_u32(start, now) >= timeout;
}

int lib_datetime_is_valid(const lib_datetime *value)
{
    if (value == 0 || value->year < 1970u || value->year > 2106u ||
        value->month < 1u || value->month > 12u || value->day < 1u ||
        value->hour > 23u || value->minute > 59u || value->second > 59u)
    {
        return 0;
    }
    return value->day <= days_in_month(value->year, value->month);
}

int lib_datetime_to_unix(const lib_datetime *value, uint32_t *timestamp)
{
    uint32_t days = 0u;
    uint64_t seconds;
    if (timestamp == 0 || !lib_datetime_is_valid(value))
    {
        return 0;
    }
    for (uint16_t year = 1970u; year < value->year; ++year)
    {
        days += is_leap_year(year) ? 366u : 365u;
    }
    for (uint8_t month = 1u; month < value->month; ++month)
    {
        days += days_in_month(value->year, month);
    }
    days += value->day - 1u;
    seconds = (uint64_t)days * 86400u + (uint32_t)value->hour * 3600u +
              (uint32_t)value->minute * 60u + value->second;
    if (seconds > UINT32_MAX)
    {
        return 0;
    }
    *timestamp = (uint32_t)seconds;
    return 1;
}

int lib_unix_to_datetime(uint32_t timestamp, lib_datetime *value)
{
    lib_datetime result = {1970, 1, 1, 0, 0, 0};
    uint32_t days = timestamp / 86400u;
    uint32_t seconds = timestamp % 86400u;
    if (value == 0)
    {
        return 0;
    }
    for (;;)
    {
        uint32_t year_days = is_leap_year(result.year) ? 366u : 365u;
        if (days < year_days)
        {
            break;
        }
        days -= year_days;
        ++result.year;
    }
    while (days >= days_in_month(result.year, result.month))
    {
        days -= days_in_month(result.year, result.month);
        ++result.month;
    }
    result.day = (uint8_t)(days + 1u);
    result.hour = (uint8_t)(seconds / 3600u);
    result.minute = (uint8_t)((seconds / 60u) % 60u);
    result.second = (uint8_t)(seconds % 60u);
    *value = result;
    return 1;
}
