#pragma once

#define FFPLAY_FEATURE

extern bool FFPlayInit();
extern void FFPlay(const char* fileName);
extern void FFPlayCleanup();
