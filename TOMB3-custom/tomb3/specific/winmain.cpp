#include "pch.h"
#include "winmain.h"
#include <sys/stat.h>
#include <memory>
#include "dxshell.h"
#include "drawprimitive.h"
#include "hwrender.h"
#include "texture.h"
#include "init.h"
#include "specific.h"
#include "smain.h"
#include "utils.h"
#include "ds.h"
#include "timer.h"
#include "di.h"
#include "audio.h"
#include "display.h"
#include "picture.h"
#include "output.h"
#include "fmv.h"
#include "../game/invfunc.h"
#include "../newstuff/setupdlg.h"
#include "../tomb3/tomb3.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

WINAPP App;
HWCONFIG HWConfig;
char* G_lpCmdLine;
long game_closedown;
bool GtWindowClosed;
long distanceFogValue;
long farz;

static std::shared_ptr<spdlog::logger> m_fileLog = spdlog::basic_logger_mt("file", "logs/debug.txt", true);

bool WinDXInit(DEVICEINFO* device, DXCONFIG* config, bool createNew)
{
	return DXStartRenderer(device, config, createNew, tomb3.Windowed);
}

void WinAppExit()
{
	ShutdownGame();
	exit(0);
}

LRESULT CALLBACK WinAppProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_ACTIVATE:

		switch ((short)wParam)
		{
		case WA_INACTIVE:
			App.bFocus = 0;
			break;

		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			App.bFocus = 1;
			break;
		}

		break;

	case WM_CLOSE:
		WinAppExit();
		PostQuitMessage(0);
		break;

	case WM_MOVE:
		DXMove((short)lParam, short((lParam >> 16) & 0xFFFF));
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

long WinRegisterWindow(HINSTANCE hinstance)
{
	App.WindowClass.hIcon = LoadIcon(hinstance, (LPCSTR)115);		//todo: icon resource define
	App.WindowClass.lpszMenuName = 0;
	App.WindowClass.lpszClassName = "Window Class";
	App.WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	App.WindowClass.hInstance = hinstance;
	App.WindowClass.style = CS_VREDRAW | CS_HREDRAW;
	App.WindowClass.lpfnWndProc = WinAppProc;
	App.WindowClass.cbClsExtra = 0;
	App.WindowClass.cbWndExtra = 0;
	App.WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
	return RegisterClass(&App.WindowClass);
}

static bool WinCreateWindow()
{
	App.WindowHandle = CreateWindowEx(WS_EX_APPWINDOW, "Window Class", "Tomb Raider III", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, App.hInstance, 0);

	if (!App.WindowHandle)
		return 0;

	return 1;
}

float WinFrameRate()
{
	double t, time_now;
	static float fps;
	static long time, counter;
	static char first_time;

	if (!(first_time & 1))
	{
		first_time |= 1;
		time = clock();
	}

	counter++;

	if (counter == 10)
	{
		time_now = clock();
		t = (time_now - time) / (double)CLOCKS_PER_SEC;
		time = (long)time_now;
		fps = float(counter / t);
		counter = 0;
	}

	App.fps = fps;
	return fps;
}

void WinFreeDX(bool free_dd)
{
	DXFreeTPages();

	if (App.D3DView)
	{
		App.D3DView->Release();
		App.D3DView = 0;
	}

	if (App.D3DDev)
	{
		App.D3DDev->Release();
		App.D3DDev = 0;
	}

	if (App.ZBuffer)
	{
		App.ZBuffer->Release();
		App.ZBuffer = 0;
	}

	if (App.BackBuffer)
	{
		App.BackBuffer->Release();
		App.BackBuffer = 0;
	}

	if (App.FrontBuffer)
	{
		App.FrontBuffer->Release();
		App.FrontBuffer = 0;
	}

	if (App.PictureBuffer)
	{
		App.PictureBuffer->Release();
		App.PictureBuffer = 0;
	}

	if (free_dd)
	{
		if (App.D3D)
		{
			App.D3D->Release();
			App.D3D = 0;
		}

		if (App.DDraw)
		{
			App.DDraw->Release();
			App.DDraw = 0;
		}
	}
}

void WinSetStyle(bool fullscreen, ulong& set)
{
	ulong style;

	style = GetWindowLong(App.WindowHandle, GWL_STYLE);

	if (fullscreen)
		style = (style & ~WS_OVERLAPPEDWINDOW) | WS_POPUP;
	else
		style = (style & ~WS_POPUP) | WS_OVERLAPPEDWINDOW;

	style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU);
	SetWindowLong(App.WindowHandle, GWL_STYLE, style);

	if (set)
		set = style;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	DIRECT3DINFO* d3dinfo;
	HWND desktop;
	HDC hdc;
	DEVMODE devmode;
	static ulong bpp;

	InitLog();
	G_lpCmdLine = lpCmdLine;
	memset(&App, 0, sizeof(WINAPP));
	App.hInstance = hInstance;
	tomb3.WindowStyle = WS_OVERLAPPEDWINDOW;

	if (!hPrevInstance && !WinRegisterWindow(hInstance))
	{
		MessageBox(0, "Unable To Register Window Class", "", MB_OK);
		return 0;
	}

	if (!WinCreateWindow())
	{
		MessageBox(0, "Unable To Create Window", "", MB_OK);
		return 0;
	}

	ShowWindow(App.WindowHandle, SW_HIDE);
	UpdateWindow(App.WindowHandle);

	DXGetDeviceInfo(&App.DeviceInfo, App.WindowHandle, App.hInstance);
	App.lpDXConfig = &App.DXConfig;
	App.lpDeviceInfo = &App.DeviceInfo;

	tomb3.gold = UT_FindArg("gold");

	if (tomb3.gold)
		memcpy(LevelSecrets, gLevelSecrets, sizeof(LevelSecrets));

	if ((!S_LoadSettings() || UT_FindArg("setup")))
	{
		SetupDialog(&App.DeviceInfo, &App.DXConfig, App.hInstance);
		DXFreeDeviceInfo(&App.DeviceInfo);

		desktop = GetDesktopWindow();
		hdc = GetDC(desktop);
		bpp = GetDeviceCaps(hdc, BITSPIXEL);
		ReleaseDC(desktop, hdc);

		desktop = GetDesktopWindow();
		hdc = GetDC(desktop);
		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmBitsPerPel = bpp;
		ReleaseDC(desktop, hdc);
		devmode.dmFields = DM_BITSPERPEL;
		ChangeDisplaySettings(&devmode, 0);
		return 0;
	}

	SetWindowPos(App.WindowHandle, 0, tomb3.rScreen.left, tomb3.rScreen.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	desktop = GetDesktopWindow();
	hdc = GetDC(desktop);
	bpp = GetDeviceCaps(hdc, BITSPIXEL);
	ReleaseDC(desktop, hdc);

	if (!WinDXInit(&App.DeviceInfo, &App.DXConfig, 1))
	{
		WinFreeDX(1);
		DXFreeDeviceInfo(&App.DeviceInfo);
		ShowWindow(App.WindowHandle, 0);
		MessageBox(App.WindowHandle, "Tomb Raider 3 Failed To Initialise, Please Run Setup", "Error", MB_ICONEXCLAMATION);
		return 0;
	}

	if (tomb3.Windowed)
	{
		WinSetStyle(0, tomb3.WindowStyle);
		SetCursor(LoadCursor(0, IDC_ARROW));
		ShowCursor(1);
	}
	else
	{
		WinSetStyle(1, tomb3.WindowStyle);
		SetCursor(0);
		ShowCursor(0);
	}

	UpdateWindow(App.WindowHandle);
	ShowWindow(App.WindowHandle, nShowCmd);

	d3dinfo = &App.lpDeviceInfo->DDInfo[App.lpDXConfig->nDD].D3DInfo[App.lpDXConfig->nD3D];

	if (!(d3dinfo->DeviceDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHA))
		d3dinfo->Texture[App.lpDXConfig->D3DTF].bAlpha = 0;

	HWConfig.bPersp = 1;
	HWConfig.bDither = App.DXConfig.Dither;
	HWConfig.nFilter = D3DFILTER_NEAREST + (App.DXConfig.Filter != 0);
	HWConfig.nShadeMode = D3DSHADE_GOURAUD;
	HWConfig.nFillMode = D3DFILL_SOLID;

	framedump = 0;
	App.nUVAdd = 256;
	UT_InitAccurateTimer();
	DXResetPalette(PictureTextures);
	InitDrawPrimitive(App.D3DDev, App.BackBuffer);
	farz = 0x5000;
	distanceFogValue = 0x3000;
	TIME_Init();
	HWR_Init();
	DS_Start(0);
	DI_Start();
	ACMInit();
	setup_screen_size();
	game_closedown = 0;
	GtWindowClosed = 0;
	GtFullScreenClearNeeded = 0;
	GameMain();

	desktop = GetDesktopWindow();
	hdc = GetDC(desktop);
	devmode.dmSize = sizeof(DEVMODE);
	devmode.dmBitsPerPel = bpp;
	ReleaseDC(desktop, hdc);
	devmode.dmFields = DM_BITSPERPEL;
	ChangeDisplaySettings(&devmode, 0);
	return 0;
}

void S_ExitSystem(const char* msg)
{
	DXSetCooperativeLevel(App.DDraw, App.WindowHandle, DDSCL_NORMAL);
	MessageBox(App.WindowHandle, msg, 0, MB_OK);
	ShutdownGame();
	strcpy(exit_message, msg);
	exit(1);
}



void InitLog()
{
#ifdef _DEBUG
	if (m_fileLog)
		m_fileLog->set_level(spdlog::level::trace);
#endif
}

void Log(const char* s, ...)		// NOT present in original code
{
#ifdef _DEBUG
	va_list list;
	char buf[4096];

	va_start(list, s);
	vsprintf(buf, s, list);
	va_end(list);

	m_fileLog->debug(buf);
#endif
}

bool DoFolderExist(const char* folderName)
{
	struct stat sb;
	return stat(folderName, &sb) == 0;
}

bool DoFileExist(const char* fileName)
{
	struct stat sb;
	return stat(fileName, &sb) == 0 && !(sb.st_mode & S_IFDIR);
}

bool DoCreateFolder(const char* folderName)
{
	return _mkdir(folderName) == 0;
}
