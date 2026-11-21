#ifndef EMBEDDED_LIB_BUFFER_H
#define EMBEDDED_LIB_BUFFER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* 原地反转 length 字节。length == 0 时 data 可为 NULL。 */
void lib_reverse_bytes(void *data, size_t length);

/* 按显式长度搜索，允许数据中包含 '\0'，不需要字符串终止符。
 * 找到返回数据内的只读地址；找不到返回 NULL。
 * 空 needle 匹配 data 起点；长度非零时对应指针必须非空。
 */
const void *lib_mem_find(const void *data, size_t length, const void *needle, size_t needle_length);

/* ASCII 字符串忽略大小写搜索；只折叠 A..Z，不依赖 locale，不处理 UTF-8 大小写。
 * 参数为非空、以 '\0' 结尾的字符串；空 needle 返回 text。
 */
const char *lib_str_find_nocase(const char *text, const char *needle);

/* 从有效数据中移除一个区间，并把尾部向前移动。
 * 成功返回 1，同时更新 *length；范围或指针无效返回 0，输入不变。
 * data 容量至少为 *length；尾部腾出的字节未清零。
 */
int lib_buffer_remove(void *data, size_t *length, size_t offset, size_t count);

/* 普通复制/重叠复制/填充请直接使用 memcpy/memmove/memset。 */

#ifdef __cplusplus
}
#endif

#endif /* EMBEDDED_LIB_BUFFER_H */
