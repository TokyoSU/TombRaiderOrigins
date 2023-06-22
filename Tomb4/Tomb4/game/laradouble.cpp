#include "pch.h"
#include "laradouble.h"
#include "box.h"
#include "sound.h"
#include "control.h"
#include "lara.h"

void InitialiseLaraDouble(short item_number)
{
	InitialiseCreature(item_number);
}

void LaraDoubleControl(short item_number)
{
	if (!CreatureActive(item_number))
		return;

	ITEM_INFO* item = &items[item_number];
	if (item->hit_status)
	{
		lara_item->hit_points += item->hit_points - LARA_HITPOINTS;
		item->hit_points = LARA_HITPOINTS;
	}
	SoundEffect(SFX_METAL_SCRAPE_LOOP1, &item->pos, SFX_DEFAULT);
	AnimateItem(item);
}