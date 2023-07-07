#include "../tomb3/pch.h"
#include "ffplay.h"
// Required for PathFindExtension()
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "../specific/winmain.h"

// GetProcAddress Macro
#define GET_DLL_PROC(dll, proc) { \
	*(FARPROC *)&(proc) = GetProcAddress((dll), #proc); \
	if( proc == NULL ) throw #proc; \
}

constexpr auto FFPLAY_HWND_CLASS = "Tomb3::FmvClass";
constexpr auto FFPLAY_DLL_NAME = "ffplay.dll";
static HMODULE hFFplay = NULL;
static HWND hWnd = NULL; // Another window on top of the previous one !

// Imports from ffplay.dll
static int(__stdcall* ffplay_init)(HWND, int, const char*);
static int(__stdcall* ffplay_play_video)(const char*, int, int, int, int);
static int(__stdcall* ffplay_cleanup)(void);

static const char videoExts[][4] = {
	"MP4",
	"BIK",
	"RPL",
};

static void FFPlayRelease()
{
	FreeLibrary(hFFplay);
	hFFplay = NULL;
}

bool FFPlayInit()
{
	if (hFFplay != NULL)
		return true;

	hFFplay = LoadLibrary(FFPLAY_DLL_NAME);
	if (hFFplay == NULL)
	{
		// failed to load DLL
		return false;
	}

	try
	{
		GET_DLL_PROC(hFFplay, ffplay_init);
		GET_DLL_PROC(hFFplay, ffplay_play_video);
		GET_DLL_PROC(hFFplay, ffplay_cleanup);
	}
	catch (LPCTSTR procName)
	{
		// failed to load one of the procs
		FFPlayRelease();
		return false;
	}

	App.WindowClass.hIcon = 0;
	App.WindowClass.lpszMenuName = 0;
	App.WindowClass.lpszClassName = FFPLAY_HWND_CLASS;
	App.WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	App.WindowClass.hInstance = App.hInstance;
	App.WindowClass.style = CS_DBLCLKS;
	App.WindowClass.lpfnWndProc = WinAppProc;
	App.WindowClass.cbClsExtra = 0;
	App.WindowClass.cbWndExtra = 0;
	App.WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
	if (!RegisterClass(&App.WindowClass))
		return false;

	hWnd = CreateWindowEx(WS_EX_TOPMOST, FFPLAY_HWND_CLASS, "FMV Player", WS_POPUP, 0, 0, 640, 480, App.WindowHandle, NULL, App.hInstance, NULL);
	if (hWnd == NULL)
		return false;

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
	return true;
}

void FFPlay(const char* fileName)
{
	if (hFFplay != NULL)
	{
		char extFileName[256]{};
		char* extension;

		int length = strlen(fileName);
		*extFileName = '\0';
		strncat(extFileName, fileName, length - 1); // This fix not null terminating.
		extension = PathFindExtension(extFileName);

		if (extension == NULL)
		{
			extension = strchr(extFileName, 0);
			*extension = '.';
		}

		ShowWindow(hWnd, SW_SHOW);
		if (App.Windowed)
			SetWindowPos(hWnd, HWND_TOPMOST, App.rScreen.left, App.rScreen.top, App.rScreen.right, App.rScreen.bottom, 0);
		else
			SetWindowPos(hWnd, HWND_TOPMOST, App.rViewport.left, App.rViewport.top, App.rViewport.right, App.rViewport.bottom, 0);
		UpdateWindow(hWnd);
		SetForegroundWindow(hWnd);
		if (ffplay_init(hWnd, 2, "winmm") != 0)
		{
			// failed to init FFplay
			FFPlayRelease();
			App.WinPlayLoaded = FALSE;
			return;
		}

		for (unsigned int i = 0; i < sizeof(videoExts) / 4; ++i)
		{
			memcpy(extension + 1, videoExts[i], 4);
			if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(extFileName))
			{
				ffplay_play_video(extFileName, 0, 0, 0, 100);
				ffplay_cleanup();
				ShowWindow(hWnd, SW_HIDE);
				UpdateWindow(hWnd);
				SetForegroundWindow(App.WindowHandle);
				return;
			}
		}

		// NOTE: If the extension is not found, then don't play anything !
	}
}

void FFPlayCleanup()
{
	if (hFFplay != NULL)
	{
		ffplay_cleanup();
		FFPlayRelease();
	}
}
