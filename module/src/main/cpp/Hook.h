//
// Created by 泓清 on 2022/1/1.
//

#ifndef RIRU_SSAGEHOOK_HOOK_H
#define RIRU_SSAGEHOOK_HOOK_H

typedef unsigned long DWORD;

void myImGUI(void* thiz);
void* ontouch_thread(void*);
void AppOnTouch_hook(void* a1, void* a2, int type, float x, float y, int multi);
void *hack_thread(void *arg);

#endif //RIRU_SSAGEHOOK_HOOK_H
