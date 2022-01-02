//
// Created by 泓清 on 2021/12/30.
//

#ifndef RIRU_SSAGEHOOK_TOOLS_H
#define RIRU_SSAGEHOOK_TOOLS_H

#include <jni.h>
#include <cstdio>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <cstdlib>
#include <cerrno>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include <sys/system_properties.h>
#include <dobby.h> // 尝试一个新的跨平台hook框架 类型为 inlinehook

#include "config.h"
#include "log.h"
#if defined (__aarch64__) // 对64位hook的支持
#include "And64InlineHook/And64InlineHook.hpp"
#elif defined(__arm__) // 对32位hook的支持
#include "Substrate/CydiaSubstrate.h"
#endif

static int enable_hack;
static char *game_data_dir = NULL;

int isGame(JNIEnv *env, jstring appDataDir);// 判断目标程序
uint64_t getModuleBase(const char* module_name);// 获取模块地址
uint64_t getBssModuleBase(const char* module_name);// 获取模块bss地址
bool isLibraryLoaded(const char *libraryName);// 判断so是否加载
void myHook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr);// 32位&64位native层hook支持

#endif //RIRU_SSAGEHOOK_TOOLS_H
