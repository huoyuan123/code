/*
 * encoding_fix.h — 解决 Windows 控制台中文乱码
 * 在每个 .cpp 文件开头 #include "../encoding_fix.h" 即可
 * 原理：调用 Windows API 将控制台输出编码设置为 UTF-8
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
