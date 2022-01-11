//
// Created by 泓清 on 2022/1/6.
//

#ifndef RIRU_SSAGEHOOK_GUILIB_H
#define RIRU_SSAGEHOOK_GUILIB_H
#pragma once
#ifdef __ANDROID__
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include <GLES2/gl2.h>
#endif

class Gui {
public:
    Gui();
    ~Gui();

    // 初始化Gui
    void Init();
    // 渲染Gui
    void Render();
    // 触控事件回调
    void OnTouchEvent(int type, bool multi, float x, float y);

private:
    // 触控类型枚举
    enum eTouchEvent {
        TOUCH_MOVE,
        TOUCH_POP,
        TOUCH_PUSH
    };

public:
    ImVec2 m_screenSize;
    int m_sendOnTouchEvent;

private:
    bool m_initialized;
    bool m_needClearMousePos;
};

#endif //RIRU_SSAGEHOOK_GUILIB_H
