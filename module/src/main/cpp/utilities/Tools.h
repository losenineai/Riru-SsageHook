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



static int enable_hack;
static char *game_data_dir = NULL;

int isGame(JNIEnv *env, jstring appDataDir);// 判断目标程序
uint64_t getModuleBase(const char* module_name);// 获取模块地址
uint64_t getBssModuleBase(const char* module_name);// 获取模块bss地址
bool isLibraryLoaded(const char *libraryName);// 判断so是否加载

#endif //RIRU_SSAGEHOOK_TOOLS_H
