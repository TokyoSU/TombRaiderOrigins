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
	ITEM_INFO* item = &items[item_number];
	Sound.PlayEffect(SFX_METAL_SCRAPE_LOOP1, &item->pos);

	if (!CreatureActive(item_number))
		return;

	if (item->hit_status)
		lara_item->hit_points += item->hit_points - 1000;

	item->hit_points = 1000;
	AnimateItem(item);
}
