#ifndef EMBEDDED_LIB_CHECKSUM_H
#define EMBEDDED_LIB_CHECKSUM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 校验覆盖 data[0..length-1]；length 为 0 时允许 data 为 NULL。
 * CRC16/MODBUS：初值 0xffff，反射多项式 0xa001，结果无最终异或。
 * 返回 CRC 数值，在线上发送时低字节在前，可用 lib_write_u16_le()。
 * "123456789" 的 CRC 为 0x4b37；空数据 CRC 为 0xffff。
 * sum8 为累加和的低 8 位；xor8 为逐字节异或；空数据二者均为 0。
 */
uint16_t lib_crc16_modbus(const uint8_t *data, size_t length);
/* CRC-8/MAXIM-DOW：反射多项式 0x8c，初值可指定，无最终异或。
 * init=0 时 "123456789" -> 0xa1，对应原 GetCrc8(0, ...)。
 */
uint8_t lib_crc8_maxim(const uint8_t *data, size_t length, uint8_t init);
/* CRC-8/SMBUS：多项式 0x07，初值 0，不反射，无最终异或。
 * "123456789" -> 0xf4，对应原 dll_pkt_compute_crc8。
 */
uint8_t lib_crc8_smbus(const uint8_t *data, size_t length);
/* CRC-16 非反射多项式 0x1021，无最终异或，可指定初值。
 * init=0：XMODEM，"123456789" -> 0x31c3；
 * init=0xffff：CCITT-FALSE，"123456789" -> 0x29b1；
 * init=0x5555：对应原项目 LIB_InCRC。
 */
uint16_t lib_crc16_ccitt(const uint8_t *data, size_t length, uint16_t init);
/* CRC-32/ISO-HDLC：反射多项式 0xedb88320，初值/最终异或均为 0xffffffff。
 * "123456789" -> 0xcbf43926，空数据 -> 0。
 */
uint32_t lib_crc32_ieee(const uint8_t *data, size_t length);
uint8_t lib_sum8(const uint8_t *data, size_t length);
uint16_t lib_sum16(const uint8_t *data, size_t length);
uint8_t lib_xor8(const uint8_t *data, size_t length);


#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_CHECKSUM_H */
