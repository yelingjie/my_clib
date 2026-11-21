#ifndef EMBEDDED_LIB_BCD_H
#define EMBEDDED_LIB_BCD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 十进制数值与压缩 BCD 互转。成功返回 1，失败返回 0 且不修改输出。
 * 输出指针须非空；会检查非法 BCD 数字（如 0x1a）。
 * byte 接口范围 0..99；u32 接口范围 0..99999999（8 位十进制数）。
 * BCD 是整数位布局，与主机字节序无关；字节数组另用 lib_endian 读写。
 * 示例：lib_u32_to_bcd(12345678, &bcd) -> bcd == 0x12345678。
 */
int lib_u8_to_bcd(uint8_t value, uint8_t *bcd);
int lib_bcd_to_u8(uint8_t bcd, uint8_t *value);
int lib_u32_to_bcd(uint32_t value, uint32_t *bcd);
int lib_bcd_to_u32(uint32_t bcd, uint32_t *value);

#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_BCD_H */
