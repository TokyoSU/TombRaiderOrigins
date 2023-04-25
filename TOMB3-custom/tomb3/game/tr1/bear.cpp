#include "pch.h"
#include "bear.h"
#include "box.h"
#include "control.h"
#include "effects.h"
#include "game.h"
#include "lara.h"

#define BEAR_CHARGE_DAMAGE 3
#define BEAR_SLAM_DAMAGE   200
#define BEAR_ATTACK_DAMAGE 200
#define BEAR_PAT_DAMAGE    400

#define BEAR_ROAR_CHANCE 0x50
#define BEAR_REAR_CHANCE 0x300
#define BEAR_DROP_CHANCE 0x600

#define BEAR_REAR_RANGE   SQUARE(WALL_SIZE * 2)
#define BEAR_ATTACK_RANGE SQUARE(WALL_SIZE)
#define BEAR_PAT_RANGE    SQUARE(100)

#define BEAR_IDLE_ANIM 0

#define BEAR_RUN_TURN  (5*ONE_DEGREE)
#define BEAR_WALK_TURN (2*ONE_DEGREE)

#define BEAR_EAT_RANGE SQUARE(WALL_SIZE*3/4)
#define BEAR_EAT_ANGLE (20*ONE_DEGREE)

enum BearAnims
{
	BEAR_STROLL,
	BEAR_STOP,
	BEAR_WALK,
	BEAR_RUN,
	BEAR_REAR,
	BEAR_ROAR,
	BEAR_ATTACK1,
	BEAR_ATTACK2,
	BEAR_EAT,
	BEAR_DEATH
};

static BITE_INFO bear_handfront_left_bite = { 0, 0, 0, 3 };
static BITE_INFO bear_handfront_right_bite = { 0, 0, 0, 6 };
static BITE_INFO bear_head_bite = { 0, 96, 335, 14 };

void InitialiseBear(short item_number)
{
	ITEM_INFO* item = &items[item_number];
	InitialiseCreature(item_number);
	SetAnimation(item, BEAR_IDLE_ANIM);
}

void BearControl(short item_number)
{
	if (!CreatureActive(item_number))
		return;

	ITEM_INFO* item = &items[item_number];
	CREATURE_INFO* bear = (CREATURE_INFO*)item->data;
	AI_INFO info{};
	short head = 0, angle = 0;

	if (item->hit_points <= 0)
	{

	}
	else
	{
		CreatureAIInfo(item, &info);

		if (info.ahead)
		{
			head = info.angle;
		}

		GetCreatureMood(item, &info, VIOLENT);
		CreatureMood(item, &info, VIOLENT);
		angle = CreatureTurn(item, bear->maximum_turn);
		const bool dead_enemy = (bear->enemy->hit_points <= 0);

		if (item->hit_status)
			bear->flags = 1;

		switch (item->current_anim_state)
		{
		case BEAR_STOP:
			if (dead_enemy)
			{
				if (info.bite && info.distance < BEAR_EAT_RANGE)
				{
					item->goal_anim_state = BEAR_EAT;
				}
				else
				{
					item->goal_anim_state = BEAR_STROLL;
				}
			}
			else if (item->required_anim_state)
			{
				item->goal_anim_state = item->required_anim_state;
			}
			else if (bear->mood == BORED_MOOD)
			{
				item->goal_anim_state = BEAR_STROLL;
			}
			else
			{
				item->goal_anim_state = BEAR_RUN;
			}

			break;

		case BEAR_STROLL:
			bear->maximum_turn = BEAR_WALK_TURN;

			if (dead_enemy && item->is_colliding_with_target(bear_head_bite.mesh_num) && info.ahead)
			{
				item->goal_anim_state = BEAR_STOP;
			}
			else if (bear->mood != BORED_MOOD)
			{
				item->goal_anim_state = BEAR_STOP;
				if (bear->mood == ESCAPE_MOOD)
				{
					item->required_anim_state = 0;
				}
			}
			else if (GetRandomControl() < BEAR_ROAR_CHANCE)
			{
				item->required_anim_state = BEAR_ROAR;
				item->goal_anim_state = BEAR_STOP;
			}

			break;

		case BEAR_RUN:
			bear->maximum_turn = BEAR_RUN_TURN;

			if (item->is_colliding_with_target(bear_head_bite.mesh_num))
			{
				lara_item->hit_points -= BEAR_CHARGE_DAMAGE;
				lara_item->hit_status = 1;
			}

			if (bear->mood == BORED_MOOD || dead_enemy)
			{
				item->goal_anim_state = BEAR_STOP;
			}
			else if (info.ahead && !item->required_anim_state)
			{
				if (!bear->flags && info.distance < BEAR_REAR_RANGE && GetRandomControl() < BEAR_REAR_CHANCE)
				{
					item->required_anim_state = BEAR_REAR;
					item->goal_anim_state = BEAR_STOP;
				}
				else if (info.distance < BEAR_ATTACK_RANGE)
				{
					item->goal_anim_state = BEAR_ATTACK1;
				}
			}

			break;

		case BEAR_REAR:
			if (bear->flags)
			{
				item->required_anim_state = BEAR_STROLL;
				item->goal_anim_state = BEAR_STOP;
			}
			else if (bear->mood == BORED_MOOD || bear->mood == ESCAPE_MOOD)
			{
				item->goal_anim_state = BEAR_STOP;
			}
			else if (info.bite && info.distance < BEAR_PAT_RANGE)
			{
				item->goal_anim_state = BEAR_ATTACK2;
			}
			else if (item->required_anim_state)
			{
				item->goal_anim_state = item->required_anim_state;
			}
			else
			{
				item->goal_anim_state = BEAR_WALK;
			}

			break;

		case BEAR_WALK:
			if (bear->flags)
			{
				item->required_anim_state = BEAR_STROLL;
				item->goal_anim_state = BEAR_REAR;
			}
			else if (info.ahead && item->is_colliding_with_target(bear_head_bite.mesh_num))
			{
				item->goal_anim_state = BEAR_REAR;
			}
			else if (bear->mood == ESCAPE_MOOD)
			{
				item->goal_anim_state = BEAR_REAR;
				item->required_anim_state = 0;
			}
			else if (bear->mood == BORED_MOOD || GetRandomControl() < BEAR_ROAR_CHANCE)
			{
				item->required_anim_state = BEAR_ROAR;
				item->goal_anim_state = BEAR_REAR;
			}
			else if (info.distance > BEAR_REAR_RANGE || GetRandomControl() < BEAR_DROP_CHANCE)
			{
				item->required_anim_state = BEAR_STOP;
				item->goal_anim_state = BEAR_REAR;
			}

			break;

		case BEAR_ATTACK2:
			if (!item->required_anim_state && item->is_colliding_with_target(bear_handfront_left_bite.mesh_num))
			{
				lara_item->hit_points -= BEAR_PAT_DAMAGE / 2;
				lara_item->hit_status = 1;
			}

			if (!item->required_anim_state && item->is_colliding_with_target(bear_handfront_right_bite.mesh_num))
			{
				lara_item->hit_points -= BEAR_PAT_DAMAGE / 2;
				lara_item->hit_status = 1;
			}

			item->required_anim_state = BEAR_REAR;
			break;

		case BEAR_ATTACK1:
			if (!item->required_anim_state && item->is_colliding_with_target(bear_head_bite.mesh_num))
			{
				CreatureEffect(item, &bear_head_bite, DoBloodSplat);
				lara_item->hit_points -= BEAR_ATTACK_DAMAGE;
				lara_item->hit_status = 1;
				item->required_anim_state = BEAR_STOP;
			}
			break;
		}

	}

	CreatureJoint(item, 0, head);
	CreatureAnimation(item_number, angle, 0);
}
