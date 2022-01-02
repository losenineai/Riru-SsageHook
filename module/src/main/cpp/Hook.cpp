//
// Created by 泓清 on 2022/1/1.
//

#include "utilities/Tools.h"
#include "Hook.h"

const char* libName = "libgrowtopia.so";

unsigned long base_addr;

void (*origOnGUI)();

void myImGUI()
{
    LOGD("Hook1 OK");
    return origOnGUI();
}

void *hack_thread(void *arg)
{
    LOGD("注入进程 :%d", gettid());

    LOGD("开始注入游戏");

    // 填写注入内容

    do {// 等待游戏初始化完成
        sleep(1);
        LOGD("延迟进程");
    } while (!isLibraryLoaded(libName));// so已经完全加载
    sleep(1);
    base_addr = getModuleBase(libName);
    sleep(1);
    LOGD("so_addr is %lx",base_addr);

    // 开始进行 inlinehook

    /*
     * 被注释的hook
     * 在32位进程中 使用 Substrate Hook 体系
     * 在64位进程中 使用 And64InlineHook 体系
     * Substrate Hook 暂时貌似很稳
     * And64InlineHook 在hook完64位进程以后
     * 如果调用原函数 存在崩溃现象 原因未知 // 即使参数正确
     * 有待完善
     */
    /*
//    sleep(1);
//    if (hack_addr1) {
//        LOGI("打开函数于 : %lx", hack_addr1);
//        myHook((void *)hack_addr, (void *)myImGUI, (void **) &origOnGUI);
//    }
    */

    /*
     * DobbyHook 是最近才出现的
     * 一款跨平台可用的 inlinehook 框架体系
     * (在Android这块 貌似兼容aarch64和arm 甚至是支持x86)
     * 关注热度较高 整个框架更新频繁 活力十足
     * 未来较有发展前景 目前存在一部分小bug
     * 已测试 正常支持32位和64位的inlinehook
     * - 测试机型 Xiaomi MI 6
     * - 测试环境 Android 9 API 28
     * - 测试应用 普通64位小游戏
     */
    // 通过 基地址计算得到 地址指针
    unsigned long hack_addr = base_addr + 0x936130;
    // 通过 函数名映射得到 地址指针
//    void *addr = DobbySymbolResolver(nullptr,"_ZN7BaseApp4DrawEv");// ?/help
    sleep(1);
    if (hack_addr) {
        LOGI("打开函数于 : %lx", hack_addr);
        DobbyHook((void *)hack_addr, (void *)myImGUI, (void **) &origOnGUI) == RS_SUCCESS ?
        LOGD("Success Hook at %lx", hack_addr) : LOGE("Fail Hook at %lx", hack_addr);
    }

    LOGD("注入游戏结束");
    return NULL;
}