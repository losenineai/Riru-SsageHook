#include "GuiUtils.h"
#include "GuiLib.h"
#include "log.h"

bool Test = false;

bool GuiUtils::IsAnyScrollBarActive() {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImGuiID active_id = ImGui::GetActiveID();
    return (active_id && (active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_X) || active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y)));
}

bool GuiUtils::MouseOnImguiTitleBarWindow() {
    ImGuiContext& g = *GImGui;
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImRect title_bar_rect = window->TitleBarRect();

    // Maybe we no need g.HoveredWindow == window?
    if (g.HoveredWindow == window && g.HoveredId == 0 && g.HoveredIdPreviousFrame == 0 &&
        io.MousePos.x >= title_bar_rect.Min.x && io.MousePos.x <= title_bar_rect.Max.x &&
        io.MousePos.y >= title_bar_rect.Min.y && io.MousePos.y <= title_bar_rect.Max.y
    ) {
        // I don't know why if return true is detect the body.
        return false;
    }

    return true;
}

void GuiUtils::ScrollWhenDraggingOnVoid() {
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    if (!IsAnyScrollBarActive() &&
        io.MousePos.x >= windowPos.x && io.MousePos.x <= windowPos.x + windowSize.x &&
        io.MousePos.y >= windowPos.y && io.MousePos.y <= windowPos.y + windowSize.y
    ) {
        ImVec2 mouse_delta = io.MouseDelta;
        if (mouse_delta.x != 0.0f && mouse_delta.x <= 100.0f) {
            ImGui::SetScrollX(ImGui::GetScrollX() + -mouse_delta.x);
        }

        if (mouse_delta.y != 0.0f && mouse_delta.y <= 100.0f) {
            ImGui::SetScrollY(ImGui::GetScrollY() + -mouse_delta.y);
        }
    }
}

void GuiShow::HackRender() {

    // 开始 ImGui 窗口
    ImGui::Begin("SsageHook", nullptr, ImGuiWindowFlags_NoResize);

    // 当鼠标停留在标题栏时 只移动鼠标
    if (GuiUtils::MouseOnImguiTitleBarWindow()) {
        ImGuiContext& g = *GImGui;
        g.MovingWindow = nullptr;
    }

    // Checkbox
    ImGui::Checkbox("Checkbox One", &Test);
//    ModFly(m_gameHackState.ModFlyChecked);

    ImGui::Checkbox("Checkbox Two", &Test);
//    AntiCheckpoint(m_gameHackState.AntiCheckpointChecked);

    ImGui::Checkbox("Checkbox Three", &Test);
//    AntiCheckpoint(m_gameHackState.FastFallChecked);

    // 滚动而不按滚动条
    GuiUtils::ScrollWhenDraggingOnVoid();

    // 结束 ImGui 窗口
    ImGui::End();
}