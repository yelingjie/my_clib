#ifndef EMBEDDED_LIB_MATH_H
#define EMBEDDED_LIB_MATH_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 最低置位的下标 0..31；value == 0 返回 -1。 */
int lib_lowest_set_bit(uint32_t value);
/* 统计 32 位值中为 1 的位数，返回 0..32。 */
unsigned int lib_popcount32(uint32_t value);

/* 数组极值。成功返回 1；空数组/空指针/两个输出指针相同返回 0，输出不变。 */
int lib_i32_minmax(const int32_t *data, size_t length, int32_t *minimum, int32_t *maximum);

/* 将 value 限制到 [minimum, maximum]。调用者保证 minimum <= maximum。 */
int32_t lib_clamp_i32(int32_t value, int32_t minimum, int32_t maximum);

#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_MATH_H */
