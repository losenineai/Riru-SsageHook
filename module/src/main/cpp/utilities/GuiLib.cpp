//
// Created by 泓清 on 2022/1/6.
//

#include "GuiLib.h"
#include "GuiUtils.h"
#include "log.h"

#define MULT_X	0.00052083333f	// 1/1920

#define MULT_Y	0.00092592592f 	// 1/1080

extern GuiShow* g_Show;

Gui::Gui() {
    // 构建函数
    m_initialized = false;
    m_screenSize = { 0.0, 0.0 };
    m_sendOnTouchEvent = 0;

    // We need to clear the pos at first time
    m_needClearMousePos = true;
}

Gui::~Gui() {
    // 析构函数
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    m_initialized = false;
}

void Gui::Init() {

    if (m_initialized) {
        return;
    }

    LOGD("开始初始化ImGui ...");

    // 开始设置 ImGui 内容
    IMGUI_CHECKVERSION(); // 检测版本
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // TODO: 完善参数支持
    do {
        void *addr1 = DobbySymbolResolver(NULL,"_Z15GetScreenSizeXfv");
        void *addr2 = DobbySymbolResolver(NULL,"_Z15GetScreenSizeYfv");
        Gui::m_screenSize.x = KittyMemory::callFunction<float>(addr1);
        Gui::m_screenSize.y = KittyMemory::callFunction<float>(addr2);
    } while (Gui::m_screenSize.x == 0.0 || Gui::m_screenSize.y == 0.0);

    io.DisplaySize = ImVec2(Gui::m_screenSize.x, Gui::m_screenSize.y);

    // Disable loading/saving of .ini file from disk.
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename = nullptr;

    // 设置ImGui风格
    // ImGui::StyleColorsDark(); // 经典风格
    ImGui::StyleColorsClassic(); // 暗黑风格

    // 设置渲染引擎
    ImGui_ImplOpenGL3_Init();

    // 设置字库 TODO: 暂时不支持中文字库 将来支持
    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    // FIXME: Put some effort into DPI awareness.
    // Important: when calling AddFontFromMemoryTTF(), ownership of font_data is transfered by Dear ImGui by default (deleted is handled by Dear ImGui), unless we set FontDataOwnedByAtlas=false in ImFontConfig
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    // 任意扩大
    // FIXME: Put some effort into DPI awareness
    ImGui::GetStyle().ScaleAllSizes(3.0f);


    // 初始化结束
    m_initialized = true;
}

void Gui::Render() {
    // 确认是否初始化
    if (!m_initialized) {
        return;
    }

    // For imgui checkbox
    static bool checked = false;

    ImGuiIO& io = ImGui::GetIO();

    // 开始 ImGui 框架
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // 渲染游戏 ImGui 窗口
    if (g_Show){
        g_Show->HackRender();
    }

    // 渲染ing
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::IsAnyItemActive()) {
        m_sendOnTouchEvent = 1;
    } else {
        m_sendOnTouchEvent = 2;
    }

    if (m_needClearMousePos) {
        io.MousePos = ImVec2(-1, -1);
        m_needClearMousePos = false;
    }

}

// 响应触控事件
void Gui::OnTouchEvent(int type, bool multi, float x, float y) {
    if (!m_initialized) {
        m_sendOnTouchEvent = 1;
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    switch(type) {
        case eTouchEvent::TOUCH_MOVE:
            io.MousePos = ImVec2(x, y);
            io.MouseDown[0] = true;
            break;

        case eTouchEvent::TOUCH_POP:
            io.MouseDown[0] = false;
            m_needClearMousePos = true;
            break;

        case eTouchEvent::TOUCH_PUSH:
            io.MousePos = ImVec2(x, y);
            break;

        default:
            break;
    }
}

