#pragma once
#include "global/types.h"

typedef void(__cdecl* rgbfunc)(unsigned char*, unsigned char*, unsigned char*);
LPDIRECTDRAWSURFACEX CreateTexturePage(long w, long h, long MipMapCount, long* pSrc, rgbfunc RGBM, long format);
void FreeTextures();

extern TEXTURE* Textures;
extern long nTextures;
