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
#include <vector>
#include <map>
#include <any>
#include <optional>
#include <array>
#include <cmath>
#include <string>
#include <stdio.h>
#include <filesystem>
#include <regex>
#include <process.h>
#include <dinput.h>
#include <mmreg.h>
#include <time.h>
#include <d3d.h>
#include <d3dtypes.h>
#include <directxtk/SimpleMath.h>

#include "resource.h"

#define ONE_DEGREE 182
constexpr short ANGLE(short value) { return value * ONE_DEGREE; }
constexpr short DISTANCE(short value) { return value * 1024; }

using namespace DirectX;
using namespace DirectX::SimpleMath;

extern void WinDisplayString(long x, long y, const char* string, ...);
extern void WinDisplayString(long x, long y, unsigned char flags, const char* string, ...);

#include <angelscript.h>
#include "script/autowrapper/aswrappedcall.h"
#include "script/contextmgr/contextmgr.h"
#include "script/datetime/datetime.h"
#include "script/debugger/debugger.h"
#include "script/scriptany/scriptany.h"
#include "script/scriptarray/scriptarray.h"
#include "script/scriptbuilder/scriptbuilder.h"
#include "script/scriptdictionary/scriptdictionary.h"
#include "script/scriptfile/scriptfile.h"
#include "script/scriptfile/scriptfilesystem.h"
#include "script/scriptgrid/scriptgrid.h"
#include "script/scripthandle/scripthandle.h"
#include "script/scripthelper/scripthelper.h"
#include "script/scriptmath/scriptmath.h"
#include "script/scriptmath/scriptmathcomplex.h"
#include "script/scriptstdstring/scriptstdstring.h"
#include "script/serializer/serializer.h"
#include "script/weakref/weakref.h"

#endif //PCH_H
