#include "../tomb3/pch.h"
#include "ffplay.h"
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "../specific/winmain.h"

// GetProcAddress Macro
#define GET_DLL_PROC(dll, proc) { \
	*(FARPROC *)&(proc) = GetProcAddress((dll), #proc); \
	if( proc == NULL ) throw #proc; \
}

constexpr auto FFPLAY_DLL_NAME = "ffplay.dll";
static HMODULE hFFplay = NULL;

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

	if (ffplay_init(App.WindowHandle, 2, "winmm") != 0)
	{
		// failed to init FFplay
		FFPlayRelease();
		return false;
	}

	return true;
}

void FFPlay(const char* fileName)
{
	if (hFFplay != NULL)
	{
		char extFileName[256]{};
		char* extension;

		strncpy(extFileName, fileName, sizeof(extFileName) - 1);
		extension = PathFindExtension(extFileName);

		if (extension == NULL)
		{
			extension = strchr(extFileName, 0);
			*extension = '.';
		}

		for (unsigned int i = 0; i < sizeof(videoExts) / 4; ++i)
		{
			memcpy(extension + 1, videoExts[i], 4);
			if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(extFileName))
			{
				ffplay_play_video(extFileName, 0, 0, 0, 100);
				return;
			}
		}

		ffplay_play_video(fileName, 0, 0, 0, 100);
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
