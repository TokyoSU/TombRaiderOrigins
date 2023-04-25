#include "pch.h"
#include "atlantean_centaur.h"
#include "box.h"
#include "control.h"
#include "effects.h"
#include "items.h"
#include "people.h"
#include "game.h"
#include "missile.h"
#include "objects.h"
#include "lara.h"
#include "sound.h"

enum centaur_anims
{
	CENTAUR_EMPTY,
	CENTAUR_STOP,
	CENTAUR_SHOOT,
	CENTAUR_RUN,
	CENTAUR_AIM,
	CENTAUR_DEATH,
	CENTAUR_WARNING
};

static BITE_INFO centaur_rocket = { 11, 415, 41, 13 };
static BITE_INFO centaur_rear = { 50, 30, 0, 5 };

#define CENTAUR_REAR_DAMAGE 200
#define CENTAUR_PART_DAMAGE 100
#define CENTAUR_IDLE_ANIM 0
#define CENTAUR_DIE_ANIM 8
#define ROCKET_SPEED 220
#define ROCKET_DAMAGE 100
#define CENTAUR_TURN (ONE_DEGREE*4)
#define CENTAUR_REAR_CHANCE 0x60
#define CENTAUR_REAR_RANGE SQUARE(WALL_SIZE*3/2)

static short RocketGun(long x, long y, long z, short speed, short yrot, short room_number)
{
	short fx_number = CreateEffect(room_number);
	if (fx_number != NO_ITEM)
	{
		FX_INFO* fx = &effects[fx_number];
		fx->pos.x_pos = x;
		fx->pos.y_pos = y;
		fx->pos.z_pos = z;
		fx->room_number = room_number;
		fx->pos.x_rot = fx->pos.z_rot = 0;
		fx->pos.y_rot = yrot;
		fx->speed = ROCKET_SPEED;
		fx->frame_number = 0;
		fx->object_number = TR1_ATLANTEAN_BOMB;
		fx->shade = 16 * 256;
		fx->flag2 = ROCKET_DAMAGE;
		ShootAtLara(fx);
	}

	return (fx_number);
}

void InitialiseAtlanteanCentaur(short item_number)
{
	ITEM_INFO* item = &items[item_number];
	InitialiseCreature(item_number);
	SetAnimation(item, CENTAUR_IDLE_ANIM);
}

void AtlanteanCentaurControl(short item_number)
{
	if (!CreatureActive(item_number))
		return;
	ITEM_INFO* item = &items[item_number];
	CREATURE_INFO* centaur = (CREATURE_INFO*)item->data;
	AI_INFO info{};
	short head = 0, angle = 0;

	if (item->hit_points <= 0)
	{
		if (item->current_anim_state != CENTAUR_DEATH)
			SetAnimation(item, CENTAUR_DIE_ANIM);
	}
	else
	{
		CreatureAIInfo(item, &info);
		if (info.ahead)
			head = info.angle;
		GetCreatureMood(item, &info, VIOLENT);
		CreatureMood(item, &info, VIOLENT);
		angle = CreatureTurn(item, CENTAUR_TURN);

		switch (item->current_anim_state)
		{
		case CENTAUR_STOP:
			head = 0;
			if (item->required_anim_state)
				item->goal_anim_state = item->required_anim_state;
			else if (info.bite && info.distance < CENTAUR_REAR_RANGE)
				item->goal_anim_state = CENTAUR_RUN;
			else if (Targetable(item, &info))
				item->goal_anim_state = CENTAUR_AIM;
			else
				item->goal_anim_state = CENTAUR_RUN;
			break;

		case CENTAUR_RUN:
			if (info.bite && info.distance < CENTAUR_REAR_RANGE)
			{
				item->required_anim_state = CENTAUR_WARNING;
				item->goal_anim_state = CENTAUR_STOP;
			}
			else if (Targetable(item, &info))
			{
				item->required_anim_state = CENTAUR_AIM;
				item->goal_anim_state = CENTAUR_STOP;
			}
			else if (GetRandomControl() < CENTAUR_REAR_CHANCE)
			{
				item->required_anim_state = CENTAUR_WARNING;
				item->goal_anim_state = CENTAUR_STOP;
			}
			break;

		case CENTAUR_AIM:
			if (item->required_anim_state)
				item->goal_anim_state = item->required_anim_state;
			else if (Targetable(item, &info))
				item->goal_anim_state = CENTAUR_SHOOT;
			else
				item->goal_anim_state = CENTAUR_STOP;
			break;

		case CENTAUR_SHOOT:
			if (!item->required_anim_state)
			{
				item->required_anim_state = CENTAUR_AIM;
				CreatureEffect(item, &centaur_rocket, RocketGun);
			}
			break;

		case CENTAUR_WARNING:
			if (!item->required_anim_state && item->is_colliding_with_target(centaur_rear.mesh_num))
			{
				CreatureEffect(item, &centaur_rear, DoBloodSplat);
				lara_item->hit_points -= CENTAUR_REAR_DAMAGE;
				lara_item->hit_status = 1;
				item->required_anim_state = CENTAUR_STOP;
			}
			break;
		}
	}

	CreatureJoint(item, 0, head);
	CreatureAnimation(item_number, angle, 0);

	if (item->status == ITEM_DEACTIVATED)
	{
		SoundEffect(171, &item->pos, NULL);
		ExplodingDeath(item_number, 0xffffffff, 0, CENTAUR_PART_DAMAGE);
		KillItem(item_number);
		item->status = ITEM_DEACTIVATED;
	}
}
