#ifndef EMBEDDED_LIB_ENDIAN_H
#define EMBEDDED_LIB_ENDIAN_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* be：高字节在前；le：低字节在前。与主机大小端无关。
 * 缓冲区须非空且足够大：u16 为 2 字节，u24 为 3 字节，u32/float 为 4 字节，u64 为 8 字节。
 * 允许非对齐的普通内存，不适用于 volatile 硬件寄存器。
 */
int lib_is_little_endian(void);

/* 无条件反转字节顺序，如 0x12345678 -> 0x78563412。 */
uint16_t lib_bswap16(uint16_t value);
uint32_t lib_bswap32(uint32_t value);
uint64_t lib_bswap64(uint64_t value);

/* 仅交换两个 16 位字，字内字节不变：0x12345678 -> 0x56781234。
 * 可用于 Modbus 的低字在前格式。
 */
uint32_t lib_swap_words32(uint32_t value);

/* 从字节数组读取数值，结果与主机大小端无关。 */
uint16_t lib_read_u16_be(const uint8_t *data);
uint16_t lib_read_u16_le(const uint8_t *data);
uint32_t lib_read_u24_be(const uint8_t *data);
uint32_t lib_read_u24_le(const uint8_t *data);
uint32_t lib_read_u32_be(const uint8_t *data);
uint32_t lib_read_u32_le(const uint8_t *data);
uint64_t lib_read_u64_be(const uint8_t *data);
uint64_t lib_read_u64_le(const uint8_t *data);

/* 将数值按指定字节顺序写入数组。 */
void lib_write_u16_be(uint8_t *data, uint16_t value);
void lib_write_u16_le(uint8_t *data, uint16_t value);
/* u24 写入仅保留 value 的低 24 位。 */
void lib_write_u24_be(uint8_t *data, uint32_t value);
void lib_write_u24_le(uint8_t *data, uint32_t value);
void lib_write_u32_be(uint8_t *data, uint32_t value);
void lib_write_u32_le(uint8_t *data, uint32_t value);
void lib_write_u64_be(uint8_t *data, uint64_t value);
void lib_write_u64_le(uint8_t *data, uint64_t value);

/* float 在字节数组中的大/小端编解码，均使用 4 字节。 */
float lib_read_float_be(const uint8_t *data);
float lib_read_float_le(const uint8_t *data);
void lib_write_float_be(uint8_t *data, float value);
void lib_write_float_le(uint8_t *data, float value);


#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_ENDIAN_H */
