#include "../tomb3/pch.h"
#include "tr1_dartemitter.h"
#include "../global/types.h"
#include "../3dsystem/phd_math.h"
#include "control.h"
#include "lara.h"
#include "effects.h"
#include "items.h"
#include "objects.h"

enum TR1_DartEmitterState
{
	DE_STATE_WAIT,
	DE_STATE_SHOOT
};

constexpr auto DART_DETECTION_RANGE = BLOCK(8);
constexpr auto DART_DAMAGE = 50;
constexpr auto DART_POISONED_DAMAGE = 160;

void Tr1DartControl(short item_number)
{
	auto* item = &items[item_number];
	if (item->touch_bits)
	{
		lara_item->hit_points -= DART_DAMAGE;
		lara_item->hit_status = 1;
		lara.poisoned += DART_POISONED_DAMAGE;
		DoBloodSplat(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, lara_item->speed, item->pos.y_rot - ANGLE(180), lara_item->room_number);
		KillItem(item_number);
	}
	else
	{
		auto x = item->pos.x_pos;
		auto z = item->pos.z_pos;
		auto speed = (item->speed * phd_cos(item->pos.x_rot)) >> W2V_SHIFT;
		item->pos.x_pos += (speed * phd_sin(item->pos.y_rot)) >> W2V_SHIFT;
		item->pos.y_pos -= (item->speed * phd_sin(item->pos.x_rot)) >> W2V_SHIFT;
		item->pos.z_pos += (speed * phd_cos(item->pos.y_rot)) >> W2V_SHIFT;
		auto room_num = item->room_number;
		auto* floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_num);

		if (item->room_number != room_num)
			ItemNewRoom(item_number, room_num);

		item->floor = GetHeight(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
		if (item->pos.y_pos >= item->floor)
			KillItem(item_number);
	}
}

static void Tr1DartEmitterCreate(ITEM_INFO* item)
{
	auto dart_number = CreateItem();
	if (dart_number != NO_ITEM)
	{
		auto* dart = &items[dart_number];
		dart->object_number = TR1_DART;
		dart->room_number = item->room_number;
		dart->pos.x_pos = item->pos.x_pos;
		dart->pos.y_pos = item->pos.y_pos;
		dart->pos.z_pos = item->pos.z_pos;
		InitialiseItem(dart_number);
		dart->pos.x_rot = 0;
		dart->pos.y_rot = item->pos.y_rot;
		dart->speed = 256;
		AddActiveItem(dart_number);
		dart->status = ITEM_ACTIVE;
	}
}

void Tr1DartEmitterControl(short item_number)
{
	auto* item = &items[item_number];
	if (!TriggerActive(item))
		return;

	switch (item->current_anim_state)
	{
	case DE_STATE_WAIT: // IDLE
		item->goal_anim_state = DE_STATE_SHOOT;
		break;
	case DE_STATE_SHOOT: // SHOOT
		if (item->frame_number == anims[item->anim_number].frame_base)
			Tr1DartEmitterCreate(item);
		break;
	}

	AnimateItem(item);
}

