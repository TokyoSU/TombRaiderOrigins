#pragma once
#include "../global/types.h"

enum BoneFlags
{
	BN_None = 0x0,
	BN_Pop = 0x1,
	BN_Push = 0x2,
	BN_X = 0x4,
	BN_Y = 0x8,
	BN_Z = 0x10,
	BN_Rotation = BN_X|BN_Y|BN_Z
};

void GetAIPickups();
void GetCarriedItems();
void InitialiseLevelFlags();
void InitialiseGameFlags();
long InitialiseLevel(long level, long type);
void BuildOutsideTable();
void InitialiseObjects();

extern short IsRoomOutsideNo;
extern char* OutsideRoomTable;
extern short OutsideRoomOffsets[729];
extern BOSS_STRUCT bossdata;
