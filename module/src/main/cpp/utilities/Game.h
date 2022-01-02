//
// Created by 泓清 on 2022/1/2.
//

#pragma once
#include "KittyMemory/KittyMemory.hpp"
#include "KittyMemory/MemoryBackup.hpp"
#include "KittyMemory/MemoryPatch.hpp"

class Game {
public:
    Game();
    ~Game() {};

    void HackRender();

    void ModFly(bool checked);
    void AntiCheckpoint(bool checked);
    void FastFall(bool checked);

private:
    struct {
        MemoryPatch ModFly;
        MemoryPatch AntiCheckpoint;
        MemoryPatch FastFall;
    } m_gameHack;

    struct _stGameHackState {
        bool ModFlyChecked;
        bool AntiCheckpointChecked;
        bool FastFallChecked;
    } m_gameHackState;
};

#endif //RIRU_SSAGEHOOK_GAME_H
