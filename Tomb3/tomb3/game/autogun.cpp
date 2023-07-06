#include "../tomb3/pch.h"
#include "autogun.h"
#include "objects.h"
#include "../specific/game.h"
#include "../3dsystem/3d_gen.h"
#include "sphere.h"
#include "effect2.h"
#include "box.h"
#include "missile.h"
#include "lot.h"
#include "items.h"
#include "control.h"
#include "people.h"
#include "sound.h"

static CreatureBiteOffset offset_left(MakeBiteInfo(110, -30, -560, 2), 0, ANGLE(90), ANGLE(180), 0, true, true);
static CreatureBiteOffset offset_right(MakeBiteInfo(-110, -30, -560, 2), 0, ANGLE(90), ANGLE(180), 0, true, true);

enum AutogunState
{
	AUTO_SHOOT,
	AUTO_IDLE,
	AUTO_DEATH
};

enum AutogunAnim
{
	AUTO_ANIM_SHOOT,
	AUTO_ANIM_IDLE,
	AUTO_ANIM_DEATH
};

void InitialiseAutogun(short item_number)
{
	auto* item = &items[item_number];
	InitialiseCreature(item);
	item->anim_number = objects[ROBOT_SENTRY_GUN].anim_index + AUTO_ANIM_IDLE;
	item->frame_number = anims[item->anim_number].frame_base;
	item->current_anim_state = AUTO_IDLE;
	item->goal_anim_state = AUTO_IDLE;
}

void AutogunControl(short item_number)
{
	if (!CreatureActive(item_number))
		return;

	auto* item = &items[item_number];
	auto* gun = GetCreatureInfo(item);
	short x_rot = 0, y_rot = 0;
	if (item->hit_status)
		item->really_active = TRUE;

	if (item->hit_points <= 0)
	{
		if (item->current_anim_state != AUTO_DEATH)
		{
			item->anim_number = objects[item->object_number].anim_index + AUTO_ANIM_DEATH;
			item->frame_number = anims[item->anim_number].frame_base;
			item->current_anim_state = AUTO_DEATH;
			item->goal_anim_state = AUTO_DEATH;
		}
		PHD_VECTOR pos{};
		GetJointAbsPosition(item, &pos, 2);
		TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, LG_PISTOLS, 32);
	}
	else
	{
		if (!item->really_active)
			return;

		AI_INFO info;
		CreatureAIInfo(item, &info);
		item->item_flags[0] = y_rot = info.angle;
		x_rot = -info.x_angle;

		switch (item->current_anim_state)
		{
		case AUTO_SHOOT:
			if (!Targetable(item, &info))
			{
				item->goal_anim_state = AUTO_IDLE;
			}
			else if (item->frame_number == anims[item->anim_number].frame_base)
			{
				gun->offset[0] = offset_left;
				gun->offset[1] = offset_right;
				ShotLaraNew(item, &info, gun, 0, info.angle, 10);
				ShotLaraNew(item, &info, gun, 1, info.angle, 10);
				SoundEffect(SFX_LARA_UZI_STOP, &item->pos, SFX_DEFAULT);
			}

			break;

		case AUTO_IDLE:
			if (Targetable(item, &info))
				item->goal_anim_state = AUTO_SHOOT;
			break;
		}
	}

	CreatureJoint(item, 0, item->item_flags[0]);
	CreatureJoint(item, 1, x_rot, ANGLE(90), ANGLE(80));
	AnimateItem(item);
}
