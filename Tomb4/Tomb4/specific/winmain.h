#pragma once


bool WinRunCheck(LPSTR WindowName, LPSTR ClassName, HANDLE* mutex);
void WinProcessCommandLine(LPSTR cmd);
void WinClose();
float WinFrameRate();
void WinDisplayString(long x, long y, char* string, ...);
void WinProcMsg();
void WinProcessCommands(long cmd);
LRESULT CALLBACK WinMainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ClearSurfaces();
bool WinRegisterWindow(HINSTANCE hinstance);
bool WinCreateWindow();
void WinSetStyle(bool fullscreen, ulong& set);

extern WINAPP App;
extern char* cutseqpakPtr;
extern long resChangeCounter;
