#pragma once
#include "effect/effect_spark.h"

enum SPRITE_TRANSTYPE
{
	SEMITRANS = 0x1,
	COLADD = 0x2,
	COLSUB = 0x3,
	WEIRD = 0x4
};

LIGHTNING_STRUCT* TriggerLightning(PHD_VECTOR* s, PHD_VECTOR* d, char variation, long rgb, unsigned char flags, unsigned char size, unsigned char segments);
long ExplodingDeath2(short item_number, long mesh_bits, short flags, short alternate_object_number = -1);
void DrawGunshells();
void TriggerGunSmoke(long x, long y, long z, long xVel, long yVel, long zVel, long notLara, long weaponType, long shade);
void LaraBubbles(ITEM_INFO* item);
void UpdateDrips();
void TriggerGlobalStaticFlame();
void TriggerGlobalFireFlame();
void keep_those_fires_burning();
void UpdateFireSparks();
void ClearFires();
void AddFire(long x, long y, long z, long size, short room_number, short fade);
void S_DrawFires();
void UpdateSmokeSparks();
void TriggerShatterSmoke(long x, long y, long z);
void DrawLensFlares(ITEM_INFO* item);
void DrawWeaponMissile(ITEM_INFO* item);
void TriggerGunShell(short leftright, short objnum, long weapon);
void UpdateGunShells();
void TriggerSmallSplash(long x, long y, long z, long num);
void TriggerGunflash(SVECTOR* pos);
void SetGunFlash(short weapon);
void DrawGunflashes();
void UpdateBlood();
void TriggerBlood(long x, long y, long z, long angle, long num);
void CreateBubble(PHD_3DPOS* pos, short room_number, long size, long biggest);
void UpdateBubbles();
void TriggerLaraDrips();
void TriggerShockwave(PHD_VECTOR* pos, long InnerOuterRads, long speed, long rgb, long XRotFlags);
void TriggerShockwaveHitEffect(long x, long y, long z, long rgb, short dir, long speed);
void UpdateShockwaves();
void UpdateLightning();
long LSpline(long x, long* knots, long nk);
void CalcLightningSpline(PHD_VECTOR* pos, SVECTOR* dest, LIGHTNING_STRUCT* lptr);
void TriggerLightningGlow(long x, long y, long z, long rgb);
void TriggerFlashSmoke(long x, long y, long z, short room_number);
void SetFadeClip(short height, short speed);
void UpdateFadeClip();
void SetScreenFadeOut(short speed, short back);
void SetScreenFadeIn(short speed);
void Fade();

extern NODEOFFSET_INFO NodeOffsets[16];
extern CEffectSparks Sparks;
extern std::vector<LIGHTNING_STRUCT> Lightning;
extern std::vector<GUNSHELL_STRUCT> Gunshells;
extern std::vector<DRIP_STRUCT> Drips;
extern std::vector<SMOKE_SPARKS> SmokeSparks;
extern std::vector<BUBBLE_STRUCT> Bubbles;
extern std::vector<SHOCKWAVE_STRUCT> ShockWaves;
extern std::vector<FIRE_SPARKS> FireSparks;
extern std::vector<BLOOD_STRUCT> Bloods;
extern std::vector<GUNFLASH_STRUCT> Gunflashes;
extern std::vector<FIRE_LIST> Fires;
extern short FlashFadeR;
extern short FlashFadeG;
extern short FlashFadeB;
extern short FlashFader;
extern short ScreenFade;
extern short dScreenFade;
extern short ScreenFadeBack;
extern short ScreenFadedOut;
extern short ScreenFading;
extern short FadeScreenHeight;
extern short DestFadeScreenHeight;
extern short FadeClipSpeed;
extern short ScreenFadeSpeed;
extern SPLASH_SETUP splash_setup;
extern long wibble;
extern long SplashCount;
extern long KillEverythingFlag;
extern long SmokeCountL;
extern long SmokeCountR;
extern long SmokeWeapon;
extern long SmokeWindX;
extern long SmokeWindZ;
extern char tsv_buffer[16384];
