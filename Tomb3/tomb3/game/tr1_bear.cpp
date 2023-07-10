#include "../tomb3/pch.h"
#include "tr1_bear.h"
#include "box.h"
#include "control.h"
#include "../specific/game.h"
#include "lara.h"
#include "effects.h"

namespace TR1
{
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

	constexpr auto BEAR_CHARGE_DAMAGE = 3;
	constexpr auto BEAR_SLAM_DAMAGE = 200;
	constexpr auto BEAR_ATTACK_DAMAGE = 200;
	constexpr auto BEAR_PAT_DAMAGE = 400;
	constexpr auto BEAR_ROAR_CHANCE = 0x50;
	constexpr auto BEAR_REAR_CHANCE = 0x300;
	constexpr auto BEAR_DROP_CHANCE = 0x600;
	constexpr auto BEAR_REAR_RANGE = SQUARE(BLOCK(2));
	constexpr auto BEAR_ATTACK_RANGE = SQUARE(BLOCK(1));
	constexpr auto BEAR_PAT_RANGE = SQUARE(CLICK(3) / 2);
	constexpr auto BEAR_RUN_TURN = ANGLE(5);
	constexpr auto BEAR_WALK_TURN = ANGLE(2);
	constexpr auto BEAR_DEATH_TURN = ANGLE(1);
	constexpr auto BEAR_EAT_RANGE = SQUARE(BLOCK(3) / 4);
	constexpr auto BEAR_EAT_ANGLE = ANGLE(20);
	constexpr auto BEAR_DEATH_ANIM = 20;
	constexpr auto BEAR_HEAD_TOUCHBITS = MESH_BITS(14);
	constexpr auto BEAR_PAT_LEFT_TOUCHBITS = MESH_BITS(4) | MESH_BITS(5) | MESH_BITS(6);
	constexpr auto BEAR_PAT_RIGHT_TOUCHBITS = MESH_BITS(1) | MESH_BITS(2) | MESH_BITS(3);
	constexpr auto BEAR_CHARGE_TOUCHBITS = BEAR_PAT_LEFT_TOUCHBITS | BEAR_PAT_RIGHT_TOUCHBITS | BEAR_HEAD_TOUCHBITS | MESH_BITS(13); // Front leg/foot + head + neck
	const BiteInfo BearHeadBite = MakeBiteInfo(0, 96, 335, 14);
	const BiteInfo BearPatRightBite = MakeBiteInfo(0, 50, 100, 6);
	const BiteInfo BearPatLeftBite = MakeBiteInfo(0, 50, 100, 3);

	void BearControl(short item_number)
	{
		if (!CreatureActive(item_number))
			return;

		auto* item = &items[item_number];
		auto* creature = GetCreatureInfo(item);
		short angle = 0, headY = 0, headX = 0;
		const bool deadEnemy = creature->enemy && creature->enemy->hit_points <= 0;

		// Don't do rear state if hit by bullet...
		if (item->hit_status)
			item->item_flags[0] = 1;
		else
			item->item_flags[0] = 0;

		if (item->hit_points <= 0)
		{
			angle = CreatureTurn(item, creature->maximum_turn);
			switch (item->current_anim_state)
			{
			case BEAR_WALK:
				item->goal_anim_state = BEAR_REAR;
				break;
			case BEAR_RUN:
			case BEAR_STROLL:
				item->goal_anim_state = BEAR_STOP;
				break;
			case BEAR_REAR:
				creature->flags = 1; // Do slam damage if lara is in range...
				item->goal_anim_state = BEAR_DEATH;
				break;
			case BEAR_STOP:
				creature->maximum_turn = 0;
				creature->flags = 0;
				item->goal_anim_state = BEAR_DEATH;
				break;
			case BEAR_DEATH:
				creature->maximum_turn = 0;
				if (creature->flags != 0 && (item->touch_bits & BEAR_CHARGE_TOUCHBITS))
				{
					lara_item->hit_points -= BEAR_SLAM_DAMAGE;
					lara_item->hit_status = 1;
					creature->flags = 0;
				}
				break;
			}
		}
		else
		{
			AI_INFO info;
			CreatureAIInfo(item, &info);
			if (info.ahead)
			{
				headY = info.angle;
				headX = info.x_angle;
			}
			GetCreatureMood(item, &info, 1);
			CreatureMood(item, &info, 1);
			angle = CreatureTurn(item, creature->maximum_turn);

			switch (item->current_anim_state)
			{
			case BEAR_ATTACK1: // Charge
				creature->maximum_turn = 0;
				if (item->required_anim_state == 0 && (item->touch_bits & BEAR_CHARGE_TOUCHBITS))
				{
					CreatureEffect(item, &BearHeadBite, DoBloodSplat);
					lara_item->hit_points -= BEAR_ATTACK_DAMAGE;
					lara_item->hit_status = 1;
					item->required_anim_state = BEAR_STOP;
				}
				break;
			case BEAR_ATTACK2: // Pat
				creature->maximum_turn = 0;

				if (item->required_anim_state == 0)
				{
					if (item->touch_bits & BEAR_PAT_LEFT_TOUCHBITS)
					{
						CreatureEffect(item, &BearPatLeftBite, DoBloodSplat);
						lara_item->hit_points -= BEAR_PAT_DAMAGE / 2;
						lara_item->hit_status = 1;
					}

					if (item->touch_bits & BEAR_PAT_RIGHT_TOUCHBITS)
					{
						CreatureEffect(item, &BearPatRightBite, DoBloodSplat);
						lara_item->hit_points -= BEAR_PAT_DAMAGE / 2;
						lara_item->hit_status = 1;
					}
					
					item->required_anim_state = BEAR_STOP;
				}
				break;
			case BEAR_RUN:
				creature->maximum_turn = BEAR_RUN_TURN;

				if (item->touch_bits & BEAR_CHARGE_TOUCHBITS)
				{
					lara_item->hit_points -= BEAR_CHARGE_DAMAGE;
					lara_item->hit_status = 1;
				}

				if (creature->mood == BORED_MOOD || deadEnemy)
				{
					item->goal_anim_state = BEAR_STOP;
				}
				else if (info.ahead && item->required_anim_state == 0)
				{
					if (item->item_flags[0] == 0 && info.distance < BEAR_REAR_RANGE && GetRandomControl() < BEAR_REAR_CHANCE)
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
			case BEAR_STROLL:
				creature->maximum_turn = BEAR_WALK_TURN;

				if (deadEnemy && (item->touch_bits & BEAR_CHARGE_TOUCHBITS) && info.ahead)
				{
					item->goal_anim_state = BEAR_STOP;
				}
				else if (creature->mood != BORED_MOOD)
				{
					item->goal_anim_state = BEAR_STOP;
					if (creature->mood == ESCAPE_MOOD)
						item->required_anim_state = 0;
				}
				else if (GetRandomControl() < BEAR_ROAR_CHANCE)
				{
					item->required_anim_state = BEAR_ROAR;
					item->goal_anim_state = BEAR_STOP;
				}
				break;
			case BEAR_REAR:
				if (item->item_flags[0] != 0)
				{
					item->required_anim_state = 0;
					item->goal_anim_state = BEAR_STOP;
				}
				else if (item->required_anim_state)
					item->goal_anim_state = item->required_anim_state;
				else if (creature->mood == BORED_MOOD || creature->mood == ESCAPE_MOOD)
					item->goal_anim_state = BEAR_STOP;
				else if (info.bite && info.distance < BEAR_PAT_RANGE)
					item->goal_anim_state = BEAR_ATTACK2;
				else
					item->goal_anim_state = BEAR_WALK;

				break;
			case BEAR_STOP:
				creature->maximum_turn = 0;

				if (deadEnemy)
				{
					if (info.bite && info.distance < BEAR_EAT_RANGE)
						item->goal_anim_state = BEAR_EAT;
					else
						item->goal_anim_state = BEAR_STROLL;
				}
				else if (item->required_anim_state)
					item->goal_anim_state = item->required_anim_state;
				else if (creature->mood == BORED_MOOD)
					item->goal_anim_state = BEAR_STROLL;
				else
					item->goal_anim_state = BEAR_RUN;
				break;
			case BEAR_WALK:
				creature->maximum_turn = BEAR_WALK_TURN;

				if (item->item_flags[0] == 0)
				{
					item->required_anim_state = 0;
					item->goal_anim_state = BEAR_REAR;
				}
				else if (info.ahead && item->touch_bits & BEAR_CHARGE_TOUCHBITS)
				{
					item->goal_anim_state = BEAR_REAR;
				}
				else if (creature->mood == ESCAPE_MOOD)
				{
					item->goal_anim_state = BEAR_REAR;
					item->required_anim_state = 0;
				}
				else if (creature->mood == BORED_MOOD || GetRandomControl() < BEAR_ROAR_CHANCE)
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
			}
		}

		CreatureJoint(item, 0, headY);
		CreatureJoint(item, 1, headX);
		CreatureAnimation(item_number, angle, 0);
	}
}
