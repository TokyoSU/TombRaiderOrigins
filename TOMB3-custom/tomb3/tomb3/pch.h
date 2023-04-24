// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

//#define RANDO_STUFF	1
#define TROYESTUFF	1

#define DIRECT3D_VERSION	0x0500
#define DIRECTDRAW_VERSION	0x0500
#define DIRECTSOUND_VERSION	0x0500
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#if (DIRECT3D_VERSION >= 0x900)
#include <d3d9.h>
#else
#include <d3d.h>
#endif
#include <cmath>
#include <stdio.h>
#include <dinput.h>
#include <dsound.h>
#include <mmreg.h>
#include <mmeapi.h>
#include <ctime>
#include <MSAcm.h>

#define ONE_DEGREE 182
#define CLICK_SIZE 256

extern void InitLog();
extern void Log(const char* s, ...);		//NOT present in original code

extern bool DoFolderExist(const char* folderName);
extern bool DoFileExist(const char* fileName);
extern bool DoCreateFolder(const char* folderName);

#endif //PCH_H
