//#ifndef RIRU_SSAGEHOOK_GUIUTILS_H
//#define RIRU_SSAGEHOOK_GUIUTILS_H

// 命名空间
namespace GuiUtils {
    // 滚动条是否被触发
    bool IsAnyScrollBarActive();
    // 在标题栏的鼠标
    bool MouseOnImguiTitleBarWindow();
    // 当被拖动时滚动
    void ScrollWhenDraggingOnVoid();
};

// GuiShow类
class GuiShow{
public:
    // 渲染画面
    void HackRender();

//    void ModFly(bool checked);
//    void AntiCheckpoint(bool checked);
//    void FastFall(bool checked);

private:
//    struct {
//        MemoryPatch ModFly;
//        MemoryPatch AntiCheckpoint;
//        MemoryPatch FastFall;
//    } m_gameHack;

//    struct _stGameHackState {
//        bool ModFlyChecked;
//        bool AntiCheckpointChecked;
//        bool FastFallChecked;
//    } m_gameHackState;

};

//#endif //RIRU_SSAGEHOOK_GUIUTILS_H