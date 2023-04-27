#pragma once
#include "../global/types.h"

bool WinDXInit(DEVICEINFO* device, DXCONFIG* config, bool createNew);
void WinAppExit();
LRESULT CALLBACK WinAppProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long WinRegisterWindow(HINSTANCE hinstance);
float WinFrameRate();
void WinFreeDX(bool free_dd);
void WinSetStyle(bool fullscreen, ulong& set);
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd);
void S_ExitSystem(const char* msg);

extern WINAPP App;
extern HWCONFIG HWConfig;
extern char* G_lpCmdLine;
extern long game_closedown;
extern bool GtWindowClosed;
extern long distanceFogValue;
extern long farz;
