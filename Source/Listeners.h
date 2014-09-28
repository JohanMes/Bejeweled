#ifndef LISTENERS_INCLUDED
#define LISTENERS_INCLUDED

#include <windows.h>
#include "JohanEngine\Component.h"

void ToggleFullscreen(void* data);
void ToggleConsole(void* data);
void ToggleOptions(void* data);
void OnRenderFrame(void* sender,float dt);
void OnMouseMove(WPARAM wParam,LPARAM lParam,int dx,int dy);
void OnMouseDown(WPARAM wParam,LPARAM lParam,bool handled);
void OnMouseWheel(WPARAM wParam,LPARAM lParam);
void RefillGame(Component* sender);
void ResizeGame(Component* sender);

#endif
