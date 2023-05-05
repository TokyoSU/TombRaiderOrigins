#pragma once
#include "../global/types.h"

void InitFont();
void UpdatePulseColour();
long GetStringLength(const char* string, long* top, long* bottom);
void DrawChar(long x, long y, unsigned short col, CHARDEF* def);
void PrintString(long x, long y, unsigned char col, const char* string, unsigned short flags);

extern long stash_font_height;
extern long smol_font_height;
extern long small_font;
extern long font_height;
extern long GnFrameCounter;
