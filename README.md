# 嵌入式通用工具库

根据充电桩项目 `bsp/stm32/stm32f4/user_lib` 的工具用途分类整理，合并重复接口，并补充浮点转换等常用功能。当前提供 **8 个模块、55 个公开函数**。原充电桩工程未修改。

| 模块 | 函数数 | 用途 |
| --- | ---: | --- |
| `lib_convert.c/.h` | 2 | IEEE 754 float 与 uint32_t 位模式互转 |
| `lib_endian.c/.h` | 25 | 16/24/32/64 位大小端读写、字节反转、32 位字交换、浮点字节读写 |
| `lib_checksum.c/.h` | 8 | CRC8 MAXIM/SMBUS、CRC16 Modbus/CCITT、CRC32 IEEE、sum8/sum16/xor8 |
| `lib_bcd.c/.h` | 4 | 压缩 BCD 与十进制数值互转，包含合法性校验 |
| `lib_hex.c/.h` | 3 | 十六进制字符、文本与字节数组互转 |
| `lib_buffer.c/.h` | 4 | 字节反转、带长度的二进制搜索、ASCII 忽略大小写搜索、区间删除 |
| `lib_math.c/.h` | 4 | 最低置位、置位计数、数组最小/最大值、整数限幅 |
| `lib_time.c/.h` | 5 | 32 位计数器回绕差值/超时、UTC 日期与 Unix 秒互转 |

原代码数量、全部函数位置、重复项和迁移注意事项见 [充电桩工具盘点](充电桩工具盘点.md)。这次整理提供统一的新接口，未为每个旧名称保留包装函数，也未复制平台内存池、消息队列、MD5/SHA1/Base64 或充电业务处理。

## 接入

把需要的 `.c/.h` 复制到工程，增加头文件搜索路径。统一包含 `lib.h`，或只包含某个模块的 `.h`。采用 C99，头文件兼容 C++，不依赖 RT-Thread、芯片 HAL、动态分配或 `libm`。

`lib_endian.c` 的浮点接口调用 `lib_convert.c`，使用该模块时一并加入编译。其余模块可独立编译。浮点接口要求目标平台使用常规 IEEE 754 binary32 布局。

```c
#include "lib.h"

void example(void)
{
    uint8_t data[4];
    uint32_t bits = lib_float_to_u32(1.0f); /* 0x3f800000，不是数值取整。 */
    lib_write_u32_be(data, bits);          /* 3f 80 00 00 */
    float value = lib_read_float_be(data); /* 1.0f */
    (void)value;

    uint32_t bcd;
    if (lib_u32_to_bcd(12345678u, &bcd))
    {
        lib_write_u32_be(data, bcd);       /* 12 34 56 78 */
    }
}
```

CRC 计算返回数值，发送顺序由调用者明确指定：

```c
uint8_t request[8] = {1, 3, 0, 0, 0, 10, 0, 0};
uint16_t crc = lib_crc16_modbus(request, 6); /* 0xcdc5 */
lib_write_u16_le(request + 6, crc);         /* 尾部 c5 cd，符合 Modbus RTU。 */
```

读写缓冲区的最小容量、空指针约定和错误返回值均在各头文件中。时间戳使用 UTC，范围为 1970-01-01 00:00:00 至 2106-02-07 06:28:15；不会根据设备所在地自动转换时区。

## 验证

在工具库目录（与本 README 同级）执行：

```powershell
python -X utf8 tests/run_tests.py
# 指定其他编译器：
python -X utf8 tests/run_tests.py --cc C:/toolchain/bin/gcc.exe
```

脚本默认查找 GCC，也可通过 `CC` 环境变量或 `--cc` 指定主机编译器。显式指定时也支持 Clang/Zig。测试采用 `-std=c99 -O2 -fstrict-aliasing -Wall -Wextra -Wpedantic -Werror`，显式启用测试断言。覆盖固定字节序向量、非对齐读写、浮点特殊值、标准 CRC 向量、非法 BCD/HEX、容量不足、二进制零字节、计数器回绕、闰年和时间戳边界。构建产物在 `build/`。

ARM 目标使用 GCC 单独检查，不运行 Windows 可执行文件：

```powershell
python -X utf8 tests/run_tests.py --arm-check --cc C:/toolchain/bin/arm-none-eabi-gcc.exe
```

此模式也支持 `ARM_CC` 环境变量，默认在 PATH 查找 `arm-none-eabi-gcc`。采用与充电桩 GCC 配置一致的 `-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard`，生成 `build/arm/libembedded_utils.a` 和 `compile_commands.json`。预编译库仅适用于匹配的 CPU/FPU/ABI；其他目标请用自己的参数重新编译源码。

本次已完成：Arm GNU Toolchain 14.3.1 的 8 个模块目标编译及归档、Zig/Clang 的主机运行测试。主机运行与 ARM 编译属于两种验证，不代表已经在充电桩上运行。

这是主机验证的通用库；接入旧协议时还需按盘点中的字节顺序、CRC 变体和日期字段约定适配调用处。
