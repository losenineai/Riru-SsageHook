//
// Created by 泓清 on 2022/1/1.
//

#include "utilities/Tools.h"
#include "Hook.h"
#include "utilities/GuiLib.h"
#include "utilities/GuiUtils.h"

bool g_initialized = false; // gui 初始化标识
Gui* g_Gui = nullptr;
GuiShow* g_Show = nullptr;
const char* libName = "libgrowtopia.so";
unsigned long base_addr;
void (*origOnGUI)(void* thiz);
void (*AppOnTouch)(void* a1, void* a2, int a3, float a4, float a5, int a6);
static struct _stOnTouchEvent {
    void* ontouch_a1;
    void* ontouch_a2;
    int ontouch_type;
    float ontouch_x;
    float ontouch_y;
    int ontouch_multi;
} stOnTouchEvent;// 触摸数据结构

// 画面hook
void myImGUI(void* thiz)
{
    origOnGUI(thiz); // 进行原画面渲染 - 此步必须 然则崩溃
    // TODO: 初始化
    if (!g_initialized) {
        LOGD("Hook 画面 OK");
        g_Show = new GuiShow;
        g_Gui = new Gui;
        g_Gui->Init();
        g_initialized = true;
    }
    // TODO: 渲染
    if (g_Gui) {
        g_Gui->Render();
    }
}

// 监听触摸线程
void* ontouch_thread(void*) {
    do {
        sleep(0);
    } while (g_Gui->m_sendOnTouchEvent == 0);

    if (g_Gui->m_sendOnTouchEvent == 2) {
        AppOnTouch(stOnTouchEvent.ontouch_a1, stOnTouchEvent.ontouch_a2, stOnTouchEvent.ontouch_type,
                   stOnTouchEvent.ontouch_x, stOnTouchEvent.ontouch_y, stOnTouchEvent.ontouch_multi);
    }
    else if (g_Gui->m_sendOnTouchEvent == 1) {
        // Type: Pop, X: 0.0, Y: 0.0, Multi: 0
        AppOnTouch(stOnTouchEvent.ontouch_a1, stOnTouchEvent.ontouch_a2, 1, 0.0, 0.0, 0);
    }

    g_Gui->m_sendOnTouchEvent = 0;

    // Now we can exit the thread
    pthread_exit(nullptr);
}

// 触摸hook
void AppOnTouch_hook(void* a1, void* a2, int type, float x, float y, int multi) {

    if (g_Gui && (x > 0.0 || y > 0.0)) {
        g_Gui->OnTouchEvent(type, multi, x, y);
    }

    stOnTouchEvent.ontouch_a1 = a1;
    stOnTouchEvent.ontouch_a2 = a2;
    stOnTouchEvent.ontouch_type = type;
    stOnTouchEvent.ontouch_x = x;
    stOnTouchEvent.ontouch_y = y;
    stOnTouchEvent.ontouch_multi = multi;

    // Create a new thread because we dont want do while loop make ontouch thread stuck
    pthread_t ptid;
    pthread_create(&ptid, nullptr, ontouch_thread, nullptr);
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
    // - 测试结果 可行
    // unsigned long hack_addr = base_addr + 0x936130;
    unsigned long hack_addr = base_addr + 0x936130;
    sleep(1);
    if (hack_addr) {
        LOGI("打开函数于 : %lx", hack_addr);
        DobbyHook((void *)hack_addr, (void *)myImGUI, (void **) &origOnGUI) == RS_SUCCESS ?
        LOGD("Hook 画面绘制 成功 于 %lx", hack_addr) : LOGE("Hook 画面绘制 失败 于 %lx", hack_addr);
    }// Hook画面绘制

    // 通过 符号集linker得到 地址指针
    // - 测试结果 可行
    // void *addr = DobbySymbolResolver(nullptr,"_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi");
    void *addr = DobbySymbolResolver(NULL,"_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi");
    if (addr) {
        DobbyHook((void *)addr, (void*)AppOnTouch_hook, (void**)&AppOnTouch) == RS_SUCCESS ?
        LOGD("Hook 触摸系统 成功 于 %lx", hack_addr) : LOGE("Hook 触摸系统 失败 于 %lx", hack_addr);
    }// Hook触摸系统


    LOGD("注入游戏结束");
    return NULL;
}