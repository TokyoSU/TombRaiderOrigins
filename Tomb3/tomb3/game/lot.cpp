#include "../tomb3/pch.h"
#include "lot.h"
#include "../specific/init.h"
#include "objects.h"
#include "box.h"
#include "control.h"
#include "camera.h"
#include "lara.h"

long slots_used;
CREATURE_INFO* baddie_slots;
static long nonlot_slots_used;
static CREATURE_INFO* non_lot_slots;

void InitialiseLOTarray()
{
	baddie_slots = (CREATURE_INFO*)game_malloc(MAX_LOT * sizeof(CREATURE_INFO));
	for (int i = 0; i < MAX_LOT; i++)
	{
		auto* creature = &baddie_slots[i];
		creature->item_num = NO_ITEM;
		creature->LOT.node = (BOX_NODE*)game_malloc(sizeof(BOX_NODE) * number_boxes);
	}
	slots_used = 0;

	non_lot_slots = (CREATURE_INFO*)game_malloc(MAX_NONLOT * sizeof(CREATURE_INFO));
	for (int i = 0; i < MAX_NONLOT; i++)
	{
		auto* creature = &non_lot_slots[i];
		creature->item_num = NO_ITEM;
	}
	nonlot_slots_used = 0;
}

void InitialiseNonLotAI(short item_number, long slot)
{
	auto* item = &items[item_number];
	auto* creature = &non_lot_slots[slot];

	if (item_number == lara.item_number)
		lara.creature = creature;
	else
		item->data = creature;

	creature->item_num = item_number;
	creature->mood = BORED_MOOD;
	creature->joint_rotation[0] = 0;
	creature->joint_rotation[1] = 0;
	creature->joint_rotation[2] = 0;
	creature->joint_rotation[3] = 0;
	creature->alerted = 0;
	creature->head_left = 0;
	creature->head_right = 0;
	creature->reached_goal = 0;
	creature->hurt_by_lara = 0;
	creature->patrol2 = 0;
	creature->maximum_turn = 182;
	creature->flags = 0;
	creature->enemy = 0;
	creature->LOT.step = 256;
	creature->LOT.drop = -512;
	creature->LOT.block_mask = 0x4000;
	creature->LOT.fly = 0;
	creature->LOT.zone = BASIC_ZONE;

	switch (item->object_number)
	{
	case LARA:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 256;
		break;

	case WHALE:
	case DIVER:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 16;
		creature->LOT.zone = CROC_ZONE;
		if (item->object_number == WHALE)
			creature->LOT.block_mask = 0x8000;
		break;
	case CROW:
	case VULTURE:
	case TR1_BAT:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 16;
		creature->LOT.zone = FLYER_ZONE;
		break;

	case LIZARD_MAN:
	case MP1:
		creature->LOT.step = 1024;
		creature->LOT.drop = -1024;
		creature->LOT.zone = HUMAN_ZONE;
		break;
	}

	nonlot_slots_used++;
}

bool EnableNonLotAI(short item_number, long Always)
{
	if (nonlot_slots_used < MAX_NONLOT)
	{
		for (int i = 0; i < MAX_NONLOT; i++)
		{
			auto* creature = &non_lot_slots[i];
			if (creature->item_num == NO_ITEM)
			{
				InitialiseNonLotAI(item_number, i);
				return true;
			}
		}
	}

	long worstdist = 0;
	long worstslot = -1;
	if (!Always)
	{
		auto* item = &items[item_number];
		auto x = (item->pos.x_pos - camera.pos.x) >> 8;
		auto y = (item->pos.y_pos - camera.pos.y) >> 8;
		auto z = (item->pos.z_pos - camera.pos.z) >> 8;
		worstdist = SQUARE(x) + SQUARE(y) + SQUARE(z);
	}

	for (auto slot = 0; slot < MAX_NONLOT; slot++)
	{
		auto* creature = &non_lot_slots[slot];
		auto* item = &items[creature->item_num];
		auto x = (item->pos.x_pos - camera.pos.x) >> 8;
		auto y = (item->pos.y_pos - camera.pos.y) >> 8;
		auto z = (item->pos.z_pos - camera.pos.z) >> 8;
		auto dist = SQUARE(x) + SQUARE(y) + SQUARE(z);
		if (dist > worstdist)
		{
			worstslot = slot;
			worstdist = dist;
		}
	}

	if (worstslot >= 0)
	{
		items[non_lot_slots[worstslot].item_num].status = ITEM_INVISIBLE;
		DisableBaddieAI(non_lot_slots[worstslot].item_num);
		InitialiseNonLotAI(item_number, worstslot);
		return true;
	}
	return false;
}

void DisableBaddieAI(short item_number)
{
	CREATURE_INFO* creature;
	auto* item = &items[item_number];

	if (item_number == lara.item_number)
	{
		creature = lara.creature;
		lara.creature = nullptr;
	}
	else
	{
		creature = (CREATURE_INFO*)item->data;
		item->data = nullptr;
	}

	if (creature != nullptr)
	{
		creature->item_num = NO_ITEM;
		if (objects[item->object_number].non_lot)
			nonlot_slots_used--;
		else
			slots_used--;
	}
}

void ClearLOT(LOT_INFO* lot)
{
	lot->tail = NO_BOX;
	lot->head = NO_BOX;
	lot->search_number = 0;
	lot->target_box = NO_BOX;
	lot->required_box = NO_BOX;

	auto* node = lot->node;
	for (int i = 0; i < number_boxes; i++)
	{
		node->next_expansion = NO_BOX;
		node->exit_box = NO_BOX;
		node->search_number = 0;
		node++;
	}
}

void CreateZone(ITEM_INFO* item)
{
	auto* creature = (CREATURE_INFO*)item->data;
	auto* r = &room[item->room_number];
	item->box_number = r->floor[((item->pos.z_pos - r->z) >> WALL_SHIFT) + r->x_size * ((item->pos.x_pos - r->x) >> WALL_SHIFT)].box;

	if (creature->LOT.fly > 0)
	{
		creature->LOT.zone_count = 0;

		auto* node = creature->LOT.node;
		for (int i = 0; i < number_boxes; i++)
		{
			node->box_number = i;
			node++;
			creature->LOT.zone_count++;
		}
	}
	else
	{
		auto* zone = ground_zone[creature->LOT.zone][FALSE];
		auto* flip = ground_zone[creature->LOT.zone][TRUE];
		auto zone_number = zone[item->box_number];
		auto flip_number = flip[item->box_number];
		creature->LOT.zone_count = 0;

		auto* node = creature->LOT.node;
		for (int i = 0; i < number_boxes; i++)
		{
			if (*zone == zone_number || *flip == flip_number)
			{
				node->box_number = i;
				node++;
				creature->LOT.zone_count++;
			}

			zone++;
			flip++;
		}
	}
}

void InitialiseSlot(short item_number, long slot)
{
	auto* item = &items[item_number];
	auto* creature = &baddie_slots[slot];

	if (item_number == lara.item_number)
		lara.creature = creature;
	else
		item->data = creature;

	creature->item_num = item_number;
	creature->mood = BORED_MOOD;
	creature->joint_rotation[0] = 0;
	creature->joint_rotation[1] = 0;
	creature->joint_rotation[2] = 0;
	creature->joint_rotation[3] = 0;
	creature->alerted = FALSE;
	creature->head_left = FALSE;
	creature->head_right = FALSE;
	creature->reached_goal = FALSE;
	creature->hurt_by_lara = FALSE;
	creature->patrol2 = FALSE;
	creature->maximum_turn = ANGLE(1);
	creature->flags = NULL;
	creature->enemy = nullptr;
	creature->LOT.step = 256;
	creature->LOT.drop = -512;
	creature->LOT.block_mask = BLOCK_SEARCH;
	creature->LOT.fly = 0;
	creature->LOT.zone = BASIC_ZONE;

	switch (item->object_number)
	{
	case LARA:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 256;
		break;

	case WHALE:
		creature->LOT.block_mask = BLOCKED_SEARCH; // fallthrough to get the zone step etc...
	case DIVER:
	case CROCODILE:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 16;
		creature->LOT.zone = CROC_ZONE;
		break;

	case CROW:
	case VULTURE:
	case TINNOS_WASP:
	case TR1_BAT:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 16;
		creature->LOT.zone = FLYER_ZONE;
		break;

	case LIZARD_MAN:
	case WILLARD_BOSS:
	case PUNK1:
	case MP1:
	case BOB:
	case CIVVIE:
	case MONKEY:
		creature->LOT.step = 1024;
		creature->LOT.drop = -1024;
		creature->LOT.zone = HUMAN_ZONE;
		break;

	case LON_BOSS:
		creature->LOT.step = 1024;
		creature->LOT.drop = -768;
		creature->LOT.zone = HUMAN_ZONE;
		break;

	case SHIVA:
	case TREX:
		creature->LOT.block_mask = BLOCKED_SEARCH;
		break;
	}

	ClearLOT(&creature->LOT);
	if (item_number != lara.item_number)
		CreateZone(item);
	slots_used++;
}

bool EnableBaddieAI(short item_number, long Always)
{
	auto* item = &items[item_number];
	if (lara.item_number == item_number)
	{
		if (lara.creature != nullptr)
			return true;
	}
	else
	{
		if (item->data != nullptr)
			return true;
		if (objects[item->object_number].non_lot)
			return EnableNonLotAI(item_number, Always);
	}

	if (slots_used < MAX_LOT)
	{
		for (auto slot = 0; slot < MAX_LOT; slot++)
		{
			auto* creature = &baddie_slots[slot];
			if (creature->item_num == NO_ITEM)
			{
				InitialiseSlot(item_number, slot);
				return true;
			}
		}
	}

	long worstdist = 0;
	long worstslot = -1;
	if (!Always)
	{
		auto x = (item->pos.x_pos - camera.pos.x) >> 8;
		auto y = (item->pos.y_pos - camera.pos.y) >> 8;
		auto z = (item->pos.z_pos - camera.pos.z) >> 8;
		worstdist = SQUARE(x) + SQUARE(y) + SQUARE(z);
	}

	for (auto slot = 0; slot < MAX_LOT; slot++)
	{
		auto* creature = &baddie_slots[slot];
		item = &items[creature->item_num];
		auto x = (item->pos.x_pos - camera.pos.x) >> 8;
		auto y = (item->pos.y_pos - camera.pos.y) >> 8;
		auto z = (item->pos.z_pos - camera.pos.z) >> 8;
		auto dist = SQUARE(x) + SQUARE(y) + SQUARE(z);
		if (dist > worstdist)
		{
			worstslot = slot;
			worstdist = dist;
		}
	}

	if (worstslot >= 0)
	{
		items[baddie_slots[worstslot].item_num].status = ITEM_INVISIBLE;
		DisableBaddieAI(baddie_slots[worstslot].item_num);
		InitialiseSlot(item_number, worstslot);
		return true;
	}
	return false;
}
