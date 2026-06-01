/*
 * encoding_fix.h — 解决 Windows 控制台中文乱码
 * ---------------------------------------------
 * 用途：
 *   在本仓库的示例程序中，经常会输出中文说明。Windows 的旧版控制台
 *   默认代码页可能不是 UTF-8，导致中文显示为乱码。
 *
 * 用法：
 *   在每个 .cpp 文件开头添加：
 *     #include "../encoding_fix.h"
 *   （不同子目录下相对路径可能不同，以实际 include 路径为准）
 *
 * 原理：
 *   仅在 Windows 平台（_WIN32）下，调用 Windows API：
 *     - SetConsoleOutputCP(65001)：设置输出代码页为 UTF-8
 *     - SetConsoleCP(65001)：设置输入代码页为 UTF-8
 *   通过一个静态对象在 main 执行前完成初始化。
 *
 * 备注/易踩坑：
 *   - 这只能影响“控制台代码页”，不改变源文件本身的编码。
 *   - 若仍乱码，通常与终端字体/渲染有关：优先使用 Windows Terminal，
 *     并选择支持中文的字体（如“微软雅黑”“等线”等）。
 *   - 若你不想在代码里 include，也可以在终端手动执行：`chcp 65001`。
 */
#ifndef ENCODING_FIX_H
#define ENCODING_FIX_H

#ifdef _WIN32
#include <windows.h>
// 在程序启动时设置控制台输出编码为 UTF-8
static struct EncodingFix {
    EncodingFix() {
        SetConsoleOutputCP(65001);       // 控制台输出 UTF-8
        SetConsoleCP(65001);             // 控制台输入 UTF-8
    }
} _encoding_fix;
#endif

#endif // ENCODING_FIX_H
