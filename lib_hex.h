#ifndef EMBEDDED_LIB_HEX_H
#define EMBEDDED_LIB_HEX_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 返回 0..15；非法字符返回 -1。接受 0..9、a..f、A..F。 */
int lib_hex_digit(char value);

/* 字节数组转大写十六进制字符串，自动补 '\0'。
 * out_size 必须至少为 length * 2 + 1；不添加空格或分隔符。
 * out 必须非空，length == 0 时 data 可为 NULL，生成空字符串。
 */
int lib_bytes_to_hex(const uint8_t *data, size_t length, char *out, size_t out_size);

/* 严格解析指定长度的十六进制文本，不接受奇数长度、空格、0x 或非法字符。
 * out_size 至少为 text_length / 2；不依赖文本的 '\0'。
 * text_length == 0 时 text 和 out 均可为 NULL。
 * 两个转换函数成功返回 1，失败返回 0 且不修改输出；输入输出不得重叠。
 */
int lib_hex_to_bytes(const char *text, size_t text_length, uint8_t *out, size_t out_size);

#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_HEX_H */
