#include "pch.h"
#include "tr1_pierre.h"
#include "box.h"
#include "control.h"
#include "camera.h"
#include "people.h"
#include "lara.h"
#include "game.h"
#include "lot.h"
#include "items.h"

enum PierreState
{
	PIERRE_EMPTY,
	PIERRE_STOP,
	PIERRE_WALK,
	PIERRE_RUN,
	PIERRE_AIM,
	PIERRE_DEATH,
	PIERRE_POSE,
	PIERRE_SHOOT
};

enum PierreOcb: short
{
	OCB_MORTAL = 0,
	OCB_IMMORTAL = 1, // will flee if hp is less than PIERRE_RUN_HITPOINTS
};

#define PIERRE_WALK_TURN (ONE_DEGREE*3)
#define PIERRE_RUN_TURN (ONE_DEGREE*6)
#define PIERRE_WALK_RANGE SQUARE(WALL_SIZE*3)
#define PIERRE_RUN_HITPOINTS 40
#define PIERRE_SHOT_DAMAGE 50
#define PIERRE_DISAPPEAR 10
#define PIERRE_IDLE_ANIM 0
#define PIERRE_DIE_ANIM 12
#define PIERRE_WIMP_CHANCE 0x2000
#define PIERRE_POSE_CHANCE 0x60

BITE_INFO pierre_gun1 = { 60, 200, 0, 11 };
BITE_INFO pierre_gun2 = { -60, 200, 0, 14 };

void InitialiseTR1Pierre(short item_number)
{
	ITEM_INFO* item = &items[item_number];
	InitialiseCreature(item_number);
	SetAnimation(item, PIERRE_IDLE_ANIM);
}

void TR1PierreControl(short item_number)
{
	if (!CreatureActive(item_number))
		return;

	ITEM_INFO* item = &items[item_number];
	CREATURE_INFO* pierre = (CREATURE_INFO*)item->data;
	AI_INFO info{};
	short angle = 0, head = 0, tilt = 0;

	if (item->hit_points <= PIERRE_RUN_HITPOINTS && item->ocb & OCB_IMMORTAL)
	{
		item->hit_points = PIERRE_RUN_HITPOINTS;
		pierre->flags++;
	}

	if (item->hit_points <= 0)
	{
		if (item->current_anim_state != PIERRE_DEATH)
			SetAnimation(item, PIERRE_DIE_ANIM);
		// TODO: spawn item from pierre or from TE pickup ?
	}
	else
	{
		CreatureAIInfo(item, &info);
		if (info.ahead)
			head = info.angle;
		GetCreatureMood(item, &info, TIMID);
		if (pierre->flags)
		{
			info.enemy_zone = -1;
			item->hit_status = 1;
			pierre->mood = ESCAPE_MOOD;
		}
		CreatureMood(item, &info, TIMID);
		angle = CreatureTurn(item, pierre->maximum_turn);

		switch (item->current_anim_state)
		{
		case PIERRE_STOP:
			if (item->required_anim_state)
				item->goal_anim_state = item->required_anim_state;
			else if (pierre->mood == BORED_MOOD)
				item->goal_anim_state = (GetRandomControl() < PIERRE_POSE_CHANCE) ? PIERRE_POSE : PIERRE_WALK;
			else if (pierre->mood == ESCAPE_MOOD)
				item->goal_anim_state = PIERRE_RUN;
			else
				item->goal_anim_state = PIERRE_WALK;
			break;

		case PIERRE_POSE:
			if (pierre->mood != BORED_MOOD)
				item->goal_anim_state = PIERRE_STOP;
			else if (GetRandomControl() < PIERRE_POSE_CHANCE)
			{
				item->required_anim_state = PIERRE_WALK;
				item->goal_anim_state = PIERRE_STOP;
			}
			break;

		case PIERRE_WALK:
			pierre->maximum_turn = PIERRE_WALK_TURN;

			if (pierre->mood == BORED_MOOD && GetRandomControl() < PIERRE_POSE_CHANCE)
			{
				item->required_anim_state = PIERRE_POSE;
				item->goal_anim_state = PIERRE_STOP;
			}
			else if (pierre->mood == ESCAPE_MOOD)
			{
				item->required_anim_state = PIERRE_RUN;
				item->goal_anim_state = PIERRE_STOP;
			}
			else if (Targetable(item, &info))
			{
				item->required_anim_state = PIERRE_AIM;
				item->goal_anim_state = PIERRE_STOP;
			}
			else if (!info.ahead || info.distance > PIERRE_WALK_RANGE)
			{
				item->required_anim_state = PIERRE_RUN;
				item->goal_anim_state = PIERRE_STOP;
			}
			break;

		case PIERRE_RUN:
			pierre->maximum_turn = PIERRE_RUN_TURN;
			tilt = angle / 2;

			if (pierre->flags == 0) // Avoid him trying to target lara
			{
				if (pierre->mood == BORED_MOOD && GetRandomControl() < PIERRE_POSE_CHANCE)
				{
					item->required_anim_state = PIERRE_POSE;
					item->goal_anim_state = PIERRE_STOP;
				}
				else if (Targetable(item, &info))
				{
					item->required_anim_state = PIERRE_AIM;
					item->goal_anim_state = PIERRE_STOP;
				}
				else if (info.ahead && info.distance < PIERRE_WALK_RANGE)
				{
					item->required_anim_state = PIERRE_WALK;
					item->goal_anim_state = PIERRE_STOP;
				}
			}

			break;

		case PIERRE_AIM:
			if (item->required_anim_state != 0)
				item->goal_anim_state = item->required_anim_state;
			else if (Targetable(item, &info))
				item->goal_anim_state = PIERRE_SHOOT;
			else
				item->goal_anim_state = PIERRE_STOP;
			break;

		case PIERRE_SHOOT:
			if (item->required_anim_state == 0)
			{
				ShotLara(item, &info, &pierre_gun1, head, PIERRE_SHOT_DAMAGE / 2, false);
				item->fired_weapon[0] = 2;
				ShotLara(item, &info, &pierre_gun2, head, PIERRE_SHOT_DAMAGE / 2, true);
				item->fired_weapon[1] = 2;

				item->required_anim_state = PIERRE_AIM;
			}

			if (pierre->mood == ESCAPE_MOOD && GetRandomControl() < PIERRE_WIMP_CHANCE)
				item->required_anim_state = PIERRE_STOP;
			break;
		}
	}

	CreatureTilt(item, tilt);
	CreatureJoint(item, 0, head);
	CreatureAnimation(item_number, angle, tilt);

	if (pierre->flags != 0)
	{
		GAME_VECTOR s{}, t{};
		s.x = item->pos.x_pos;
		s.y = item->pos.y_pos - 768;
		s.z = item->pos.z_pos;
		s.room_number = item->room_number;
		t.x = camera.pos.x;
		t.y = camera.pos.y;
		t.z = camera.pos.z;
		t.room_number = camera.pos.room_number;
		if (LOS(&s, &t))
		{
			pierre->flags = 1;
		}
		else if (pierre->flags > PIERRE_DISAPPEAR)
		{
			item->hit_points = DONT_TARGET;
			DisableBaddieAI(item_number);
			KillItem(item_number);
		}
	}
}
