#pragma once

LIGHTNING_STRUCT* TriggerLightning(PHD_VECTOR* s, PHD_VECTOR* d, char variation, long rgb, uchar flags, uchar size, uchar segments);
long ExplodingDeath2(short item_number, long mesh_bits, short Flags);
void DrawGunshells();
void TriggerGunSmoke(long x, long y, long z, long xVel, long yVel, long zVel, long notLara, long weaponType, long shade);
void LaraBubbles(ITEM_INFO* item);
void UpdateDrips();
long GetFreeFireSpark();
void TriggerGlobalStaticFlame();
void TriggerGlobalFireFlame();
void keep_those_fires_burning();
void UpdateFireSparks();
void ClearFires();
void AddFire(long x, long y, long z, long size, short room_number, short fade);
void S_DrawFires();
long GetFreeSmokeSpark();
void UpdateSmokeSparks();
void TriggerShatterSmoke(long x, long y, long z);
void DrawLensFlares(ITEM_INFO* item);
void DrawWeaponMissile(ITEM_INFO* item);
long GetFreeGunshell();
void TriggerGunShell(short leftright, short objnum, long weapon);
void UpdateGunShells();
void TriggerSmallSplash(long x, long y, long z, long num);
void TriggerGunflash(SVECTOR* pos);
void SetGunFlash(short weapon);
void DrawGunflashes();
long GetFreeBlood();
void UpdateBlood();
void TriggerBlood(long x, long y, long z, long angle, long num);
long GetFreeBubble();
void CreateBubble(PHD_3DPOS* pos, short room_number, long size, long biggest);
void UpdateBubbles();
long GetFreeDrip();
void TriggerLaraDrips();
long GetFreeShockwave();
void TriggerShockwave(PHD_VECTOR* pos, long InnerOuterRads, long speed, long rgb, long XRotFlags);
void TriggerShockwaveHitEffect(long x, long y, long z, long rgb, short dir, long speed);
void UpdateShockwaves();
void UpdateLightning();
long LSpline(long x, long* knots, long nk);
void CalcLightningSpline(PHD_VECTOR* pos, SVECTOR* dest, LIGHTNING_STRUCT* lptr);
void TriggerLightningGlow(long x, long y, long z, long rgb);
void TriggerFlashSmoke(long x, long y, long z, short room_number);
void S_DrawSparks();
void SetFadeClip(short height, short speed);
void UpdateFadeClip();
void SetScreenFadeOut(short speed, short back);
void SetScreenFadeIn(short speed);
void Fade();

extern NODEOFFSET_INFO NodeOffsets[16];
extern LIGHTNING_STRUCT Lightning[16];
extern GUNSHELL_STRUCT Gunshells[24];
extern DRIP_STRUCT Drips[32];
extern SMOKE_SPARKS smoke_spark[32];
extern BUBBLE_STRUCT Bubbles[40];
extern SHOCKWAVE_STRUCT ShockWaves[16];
extern FIRE_SPARKS fire_spark[20];
extern BLOOD_STRUCT blood[32];
extern GUNFLASH_STRUCT Gunflashes[4];
extern FIRE_LIST fires[32];
extern long next_fire_spark;
extern long next_smoke_spark;
extern long next_gunshell;
extern long next_bubble;
extern long next_drip;
extern long next_blood;
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
extern char tsv_buffer[16384];
