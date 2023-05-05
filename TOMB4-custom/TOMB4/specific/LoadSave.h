#pragma once
#include "../global/types.h"

void DoOptions();
void DisplayStatsUCunt();
long S_DisplayPauseMenu(long reset);
long DoLoadSave(long LoadSave);
long S_LoadSave(long load_or_save, long mono, long inv_active);
void S_DrawTile(long x, long y, long w, long h, LPDIRECT3DTEXTUREX t, long tU, long tV, long tW, long tH, long c0, long c1, long c2, long c3);
void S_DisplayMonoScreen();
void CreateMonoScreen();
void FreeMonoScreen();
void RGBM_Mono(unsigned char* r, unsigned char* g, unsigned char* b);
void ConvertSurfaceToTextures(LPDIRECTDRAWSURFACEX surface);
void CheckKeyConflicts();
long S_PauseMenu();
long GetSaveLoadFiles();

extern long MusicVolume;
extern long SFXVolume;
extern long ControlMethod;
extern char MonoScreenOn;
