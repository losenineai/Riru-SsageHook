//
// Created by 泓清 on 2022/1/1.
//

#ifndef RIRU_SSAGEHOOK_GUI_H
#define RIRU_SSAGEHOOK_GUI_H

#pragma once
#ifdef __ANDROID__
#include "../include/Imgui/imgui.h"
#include "../include/Imgui/imgui_internal.h"
#include "../include/Imgui/imgui_impl_opengl3.h"
#include <GLES2/gl2.h>
#endif

class Gui {
public:
    Gui();
    ~Gui();

    void Init();
    void Render();
    void OnTouchEvent(int type, bool multi, float x, float y);

private:
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

#endif //RIRU_SSAGEHOOK_GUI_H
