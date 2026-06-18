/*
 * encoding_fix.h — Windows 控制台 UTF-8 编码修正
 * --------------------------------------------------
 * 用法：在所有会输出中文到控制台的 .cpp 中加入
 *       #include "../encoding_fix.h"
 *
 * 两者选一即可，程序开头调用 init_encoding() 或直接包含本头文件：
 *   - chcp 65001：将当前控制台代码页切换为 UTF-8（推荐，侵入性最小）
 *   - SetConsoleOutputCP(65001)：API 级设置，更稳健
 *
 * 同时用 #pragma 确保 MSVC 按 UTF-8 编译源文件。
 */

#ifndef ENCODING_FIX_H
#define ENCODING_FIX_H

#ifdef _WIN32
#include <windows.h>
#include <cstdlib>

/**
 * 自动初始化：头文件被包含时，静态对象构造即自动设置
 * 无需在每个 main() 中手动调用。
 */
namespace {
struct EncodingInitializer {
    EncodingInitializer() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }
};
static EncodingInitializer __encoding_init__;
} // anonymous namespace
#endif

#if defined(_MSC_VER)
#pragma execution_character_set("utf-8")
#endif

#endif // ENCODING_FIX_H
