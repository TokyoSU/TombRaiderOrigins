// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define DIRECTINPUT_VERSION 0x0800
#define _USE_MATH_DEFINES
#define D3D_OVERLOADS
#include <windows.h>
#include <windowsx.h>
#include <algorithm>
#include <d3d.h>
#include <d3dtypes.h>
#include <directxtk/SimpleMath.h>
#include <vector>
#include <cmath>
#include <string>
#include <stdio.h>
#include <process.h>
#include <dinput.h>
#include <mmreg.h>

#include "../../resource.h"
#define ONE_DEGREE 182

using namespace DirectX;
using namespace DirectX::SimpleMath;

extern void WinDisplayString(long x, long y, const char* string, ...);

#endif //PCH_H
