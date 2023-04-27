#include "pch.h"
#include "tr1_larson.h"
#include "box.h"
#include "draw.h"
#include "control.h"
#include "people.h"
#include "lara.h"
#include "game.h"
#include "lot.h"
#include "items.h"

enum LarsonState
{
	LARSON_EMPTY,
	LARSON_STOP,
	LARSON_WALK,
	LARSON_RUN,
	LARSON_AIM,
	LARSON_DEATH,
	LARSON_POSE,
	LARSON_SHOOT
};

#define LARSON_WALK_TURN (ONE_DEGREE*3)
#define LARSON_RUN_TURN (ONE_DEGREE*6)
#define LARSON_WALK_RANGE SQUARE(WALL_SIZE*3)
#define LARSON_SHOT_DAMAGE 50
#define LARSON_IDLE_ANIM 0
#define LARSON_DIE_ANIM 15
#define LARSON_POSE_CHANCE 0x60

BITE_INFO larson_gun = { -60, 170, 10, 14 };

void InitialiseTR1Larson(short item_number)
{
	ITEM_INFO* item = &items[item_number];
	InitialiseCreature(item_number);
	SetAnimation(item, LARSON_IDLE_ANIM);
}

void TR1LarsonControl(short item_number)
{
	if (!CreatureActive(item_number))
		return;

	ITEM_INFO* item = &items[item_number];
	CREATURE_INFO* larson = (CREATURE_INFO*)item->data;
	AI_INFO info{};
	short angle = 0, head = 0, tilt = 0;

	if (item->hit_points <= 0)
	{
		if (item->current_anim_state != LARSON_DEATH)
			SetAnimation(item, LARSON_DIE_ANIM);
	}
	else
	{
		CreatureAIInfo(item, &info);
		if (info.ahead)
			head = info.angle;
		GetCreatureMood(item, &info, TIMID);
		CreatureMood(item, &info, TIMID);
		angle = CreatureTurn(item, larson->maximum_turn);

		switch (item->current_anim_state)
		{
		case LARSON_STOP:
			if (item->required_anim_state)
				item->goal_anim_state = item->required_anim_state;
			else if (larson->mood == BORED_MOOD)
				item->goal_anim_state = (GetRandomControl() < LARSON_POSE_CHANCE) ? LARSON_POSE : LARSON_WALK;
			else if (larson->mood == ESCAPE_MOOD)
				item->goal_anim_state = LARSON_RUN;
			else
				item->goal_anim_state = LARSON_WALK;
			break;

		case LARSON_POSE:
			if (larson->mood != BORED_MOOD)
				item->goal_anim_state = LARSON_STOP;
			else if (GetRandomControl() < LARSON_POSE_CHANCE)
			{
				item->required_anim_state = LARSON_WALK;
				item->goal_anim_state = LARSON_STOP;
			}
			break;

		case LARSON_WALK:
			larson->maximum_turn = LARSON_WALK_TURN;

			if (larson->mood == BORED_MOOD && GetRandomControl() < LARSON_POSE_CHANCE)
			{
				item->required_anim_state = LARSON_POSE;
				item->goal_anim_state = LARSON_STOP;
			}
			else if (larson->mood == ESCAPE_MOOD)
			{
				item->required_anim_state = LARSON_RUN;
				item->goal_anim_state = LARSON_STOP;
			}
			else if (Targetable(item, &info))
			{
				item->required_anim_state = LARSON_AIM;
				item->goal_anim_state = LARSON_STOP;
			}
			else if (!info.ahead || info.distance > LARSON_WALK_RANGE)
			{
				item->required_anim_state = LARSON_RUN;
				item->goal_anim_state = LARSON_STOP;
			}
			break;

		case LARSON_RUN:
			larson->maximum_turn = LARSON_RUN_TURN;
			tilt = angle / 2;

			if (larson->mood == BORED_MOOD && GetRandomControl() < LARSON_POSE_CHANCE)
			{
				item->required_anim_state = LARSON_POSE;
				item->goal_anim_state = LARSON_STOP;
			}
			else if (Targetable(item, &info))
			{
				item->required_anim_state = LARSON_AIM;
				item->goal_anim_state = LARSON_STOP;
			}
			else if (info.ahead && info.distance < LARSON_WALK_RANGE)
			{
				item->required_anim_state = LARSON_WALK;
				item->goal_anim_state = LARSON_STOP;
			}

			break;

		case LARSON_AIM:
			if (item->required_anim_state != 0)
				item->goal_anim_state = item->required_anim_state;
			else if (Targetable(item, &info))
				item->goal_anim_state = LARSON_SHOOT;
			else
				item->goal_anim_state = LARSON_STOP;
			break;

		case LARSON_SHOOT:
			if (item->required_anim_state == 0)
			{
				ShotLara(item, &info, &larson_gun, head, LARSON_SHOT_DAMAGE);
				item->fired_weapon[0] = 2;
				item->required_anim_state = LARSON_AIM;
			}

			if (larson->mood == ESCAPE_MOOD)
				item->required_anim_state = LARSON_STOP;
			break;
		}
	}

	CreatureTilt(item, tilt);
	CreatureJoint(item, 0, head);
	CreatureAnimation(item_number, angle, tilt);
}
