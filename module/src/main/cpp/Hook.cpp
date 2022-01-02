//
// Created by 泓清 on 2022/1/1.
//

#include "utilities/Tools.h"
#include "Hook.h"

const char* libName = "libil2cpp.so";

int (*old_puts)(const char *) = nullptr;
void (*origOnGUI)();

void myImGUI()
{
    LOGI("Hook OK");
    origOnGUI();
}

/*
 * 测试hook应当以稳定的app为例
 * 这里暂时以32位的国服为例
 */
void *hack_thread(void *arg)
{
    LOGD("注入进程 :%d", gettid());

    LOGD("开始注入游戏");

    // 填写注入内容

    do {// 等待游戏初始化完成
        sleep(1);
        LOGD("延迟进程");
    } while (!isLibraryLoaded(libName));// "il2cpp.so"已经完全加载

    // 开始进行 inlinehook

    /*
     * 被注释的hook
     * 在32位进程中 使用 Substrate Hook 体系
     * 在64位进程中 使用 And64InlineHook 体系
     * Substrate Hook 暂时貌似很稳
     * And64InlineHook 在hook完64位进程以后
     * 如果调用原函数 貌似存在崩溃现象 原因未知
     * 有待完善
     */
    // myHook((void *const)puts,(void *)new_puts, (void **)&old_puts);

    /*
     * DobbyHook 是最近才出现的
     * 一款跨平台可用的 inlinehook 框架体系
     * (在Android这块 貌似兼容aarch64和arm 甚至是支持x86)
     * 关注热度较高 整个框架更新频繁 活力十足
     * 未来较有发展前景 目前存在一部分小bug
     */
    void *addr = DobbySymbolResolver(nullptr,"OnGUI");
    if (addr) {
        LOGI("打开函数于 : %p", addr);
        DobbyHook(addr, (void *) myImGUI, (void **) &origOnGUI);
    }

    LOGD("注入游戏结束");
    return NULL;
}