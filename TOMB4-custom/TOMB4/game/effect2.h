#pragma once
#include "global/types.h"

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
extern std::vector<DYNAMIC> Dynamics;
