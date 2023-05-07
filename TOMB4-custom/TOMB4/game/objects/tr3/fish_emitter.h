#pragma once
#include "global/types.h"

constexpr auto MAX_FISHES = 24;

struct FISH_INFO
{
	short x = 0;
	short y = 0;
	short z = 0;
	unsigned short angle = 0;
	short type = 0;
	short desty = 0;
	char angadd = 0;
	unsigned char speed = 0;
	unsigned char acc = 0;
	unsigned char swim = 0;

	FISH_INFO() {}
};

struct LEADER_INFO
{
	short angle = 0;
	unsigned char speed = 0;
	unsigned char on = 0;
	short angle_time = 0;
	short speed_time = 0;
	short Xrange = 0;
	short Zrange = 0;
	short Yrange = 0;
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	short hit_wait = 0;
	short carcass_item = NO_ITEM;

	LEADER_INFO() {}
};

extern void SetupShoal(ITEM_INFO* item, LEADER_INFO* leader, short shoal_number);
extern void SetupFish(ITEM_INFO* item);
extern void InitialiseFish(short item_number);
extern void ControlFish(short item_number);
extern void S_DrawFish(ITEM_INFO* item);
