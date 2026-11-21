#ifndef EMBEDDED_LIB_CONVERT_H
#define EMBEDDED_LIB_CONVERT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 要求 IEEE 754 binary32（32 位 float）。 */
/* 保留浮点的原始位模式，不做数值取整。取整请使用 (uint32_t)value，
 * 并由调用者保证 value 有限且截断后的值在 uint32_t 范围内。
 * 使用 memcpy，避免指针强转造成的严格别名和对齐问题。
 */
uint32_t lib_float_to_u32(float value);
float lib_u32_to_float(uint32_t bits);


#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_CONVERT_H */
