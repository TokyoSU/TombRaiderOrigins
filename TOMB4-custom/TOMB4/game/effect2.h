#pragma once
#include "global/types.h"

enum SPRITE_TRANSTYPE
{
	SEMITRANS = 0x1,
	COLADD = 0x2,
	COLSUB = 0x3,
	WEIRD = 0x4
};

enum SPRITE_EFFECTS
{
	SP_FLAT = 0x1,
	SP_SCALE = 0x2,
	SP_BLOOD = 0x4,
	SP_DEF = 0x8,
	SP_ROTATE = 0x10,
	SP_EXPLOSION = 0x20,
	SP_FX = 0x40,
	SP_ITEM = 0x80,
	SP_WIND = 0x100,
	SP_EXPDEF = 0x200,
	SP_USEFXOBJPOS = 0x400,
	SP_UNDERWEXP = 0x800,
	SP_NODEATTACH = 0x1000,
	SP_PLASMAEXP = 0x2000
};

void ControlSmokeEmitter(short item_number);
void TriggerExplosionSmokeEnd(long x, long y, long z, long uw);
void TriggerExplosionSmoke(long x, long y, long z, long uw);
void TriggerFlareSparks(long x, long y, long z, long xvel, long yvel, long zvel, long smoke);
void TriggerDynamic(long x, long y, long z, long falloff, long r, long g, long b);
void TriggerDynamic_MIRROR(long x, long y, long z, long falloff, long r, long g, long b);
void ClearDynamics();
void ControlEnemyMissile(short fx_number);
void SetupRipple(long x, long y, long z, long size, long flags);
void TriggerUnderwaterBlood(long x, long y, long z, long size);
void TriggerWaterfallMist(long x, long y, long z, long ang);
void TriggerDartSmoke(long x, long y, long z, long xv, long zv, long hit);
void KillAllCurrentItems(short item_number);
void KillEverything();
void TriggerExplosionBubble(long x, long y, long z, short room_number);
void ControlColouredLights(short item_number);
void DetatchSpark(long num, long type);
void UpdateSparks();
void TriggerRocketFlameTR3(long x, long y, long z, long xv, long yv, long zv, long item_no);
void TriggerRocketSmokeTR3(long x, long y, long z, long body_part);
void TriggerRicochetSpark(GAME_VECTOR* pos, long ang, long num, long smoke_only);
void TriggerExplosionSparks(long x, long y, long z, long extras, long dynamic, long uw, short room_number);
void TriggerFireFlame(long x, long y, long z, long body_part, long type);
void TriggerSuperJetFlame(ITEM_INFO* item, long yvel, long deadly);
void TriggerRocketSmokeTR4(long x, long y, long z, long col);
void SetupSplash(SPLASH_SETUP* setup);
void UpdateSplashes();

extern SPLASH_STRUCT splashes[4];
extern RIPPLE_STRUCT ripples[16];
extern SPLASH_SETUP splash_setup;
extern std::vector<DYNAMIC> Dynamics;
extern std::vector<SPARKS> Sparks;
extern long wibble;
extern long SplashCount;
extern long KillEverythingFlag;
extern long SmokeCountL;
extern long SmokeCountR;
extern long SmokeWeapon;
extern long SmokeWindX;
extern long SmokeWindZ;
