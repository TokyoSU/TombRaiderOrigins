#include "../tomb3/pch.h"
#include "tr1_wolf.h"
#include "../global/types.h"
#include "box.h"
#include "control.h"
#include "objects.h"
#include "effects.h"
#include "../specific/game.h"
#include "people.h"
#include "lara.h"

namespace TR1
{
	enum WolfState : short
	{
		WOLF_EMPTY, WOLF_STOP, WOLF_WALK, WOLF_RUN, WOLF_JUMP, WOLF_STALK, WOLF_ATTACK,
		WOLF_HOWL, WOLF_SLEEP, WOLF_CROUCH, WOLF_FASTTURN, WOLF_DEATH, WOLF_BITE
	};

	constexpr auto WOLF_BITE_DAMAGE = 100;
	constexpr auto WOLF_POUNCE_DAMAGE = 50;

	constexpr auto WOLF_SLEEP_ANIM = 0;
	constexpr auto WOLF_SLEEP_FRAME = 96;
	constexpr auto WOLF_RUN_ANIM = 7;

	constexpr auto WOLF_WALK_TURN = ANGLE(2);
	constexpr auto WOLF_RUN_TURN = ANGLE(5);
	constexpr auto WOLF_STALK_TURN = ANGLE(2);

	constexpr auto WOLF_ATTACK_RANGE = SQUARE(BLOCK(3) / 2);
	constexpr auto WOLF_STALK_RANGE = SQUARE(BLOCK(3));
	constexpr auto WOLF_BITE_RANGE = SQUARE(BLOCK(1) / 3);

	constexpr auto WOLF_WAKE_CHANCE = 0x20;
	constexpr auto WOLF_SLEEP_CHANCE = 0x20;
	constexpr auto WOLF_HOWL_CHANCE = 0x180;

	constexpr auto WOLF_UPPER_JAW_MESH = 3;
	constexpr auto WOLF_LOWER_JAW_MESH = 6;
	constexpr auto WOLF_TOUCH_BITS = MESH_BITS(WOLF_UPPER_JAW_MESH) | MESH_BITS(WOLF_LOWER_JAW_MESH);

	const short WolfDieAnims[] = { 20, 21, 22 };
	const short WolfDieAnimsSize = _countof(WolfDieAnims);
	const BiteInfo WolfJawBite = MakeBiteInfo(0, -14, 174, WOLF_LOWER_JAW_MESH);

	static int GetWolfDieAnim(ITEM_INFO* item)
	{
		auto result = GetRandomControl() & WolfDieAnimsSize;
		if (result < 0 || result >= WolfDieAnimsSize)
			return WolfDieAnims[0];
		return WolfDieAnims[result];
	}

	void InitialiseWolf(short item_number)
	{
		auto* item = &items[item_number];
		InitialiseCreature(item);

		switch (item->ocb)
		{
		case 0:
			SetAnimation(item, WOLF_SLEEP_ANIM, WOLF_SLEEP_FRAME);
			break;
		case 1:
			SetAnimation(item, WOLF_RUN_ANIM);
			break;
		}
	}

	void WolfControl(short item_number)
	{
		if (!CreatureActive(item_number))
			return;

		auto* item = &items[item_number];
		auto* creature = GetCreatureInfo(item);
		short angle = 0, headY = 0, headX = 0, tilt = 0;

		if (item->hit_points <= 0)
		{
			if (item->current_anim_state != WOLF_DEATH)
				SetAnimation(item, GetWolfDieAnim(item));
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
			GetCreatureMood(item, &info, TRUE);
			CreatureMood(item, &info, TRUE);
			angle = CreatureTurn(item, creature->maximum_turn);

			// TODO: add the hurt sound here with item->hit_status !

			switch (item->current_anim_state)
			{
			case WOLF_STOP:
				creature->maximum_turn = 0;
				creature->flags = 0;

				if (item->required_anim_state)
					item->goal_anim_state = item->required_anim_state;
				else
					item->goal_anim_state = WOLF_WALK;
				break;
			case WOLF_WALK:
				creature->maximum_turn = WOLF_WALK_TURN;
				creature->flags = 0;

				if (creature->mood != BORED_MOOD)
				{
					item->goal_anim_state = WOLF_STALK;
					item->required_anim_state = 0;
				}
				else if (GetRandomControl() < WOLF_SLEEP_CHANCE)
				{
					item->required_anim_state = WOLF_SLEEP;
					item->goal_anim_state = WOLF_STOP;
				}
				break;
			case WOLF_RUN:
				creature->maximum_turn = WOLF_RUN_TURN;
				creature->flags = 0;
				tilt = angle;

				if (info.ahead && info.distance < WOLF_ATTACK_RANGE)
				{
					if (info.distance > WOLF_ATTACK_RANGE / 2 && (info.enemy_facing > ANGLE(45) || info.enemy_facing < -ANGLE(45)))
					{
						item->required_anim_state = WOLF_STALK;
						item->goal_anim_state = WOLF_CROUCH;
					}
					else
					{
						item->required_anim_state = 0;
						item->goal_anim_state = WOLF_ATTACK;
					}
				}
				else if (creature->mood == STALK_MOOD && info.distance < WOLF_STALK_RANGE)
				{
					item->required_anim_state = WOLF_STALK;
					item->goal_anim_state = WOLF_CROUCH;
				}
				else if (creature->mood == BORED_MOOD)
				{
					item->goal_anim_state = WOLF_CROUCH;
				}
				break;
			case WOLF_CROUCH:
				if (item->required_anim_state)
					item->goal_anim_state = item->required_anim_state;
				else if (creature->mood == ESCAPE_MOOD)
					item->goal_anim_state = WOLF_RUN;
				else if (info.distance < WOLF_BITE_RANGE && info.bite)
					item->goal_anim_state = WOLF_BITE;
				else if (creature->mood = STALK_MOOD)
					item->goal_anim_state = WOLF_STALK;
				else if (creature->mood == BORED_MOOD)
					item->goal_anim_state = WOLF_STOP;
				else
					item->goal_anim_state = WOLF_RUN;
				break;
			case WOLF_STALK:
				creature->maximum_turn = WOLF_STALK_TURN;

				if (creature->mood == ESCAPE_MOOD)
					item->goal_anim_state = WOLF_RUN;
				else if (info.distance < WOLF_BITE_RANGE && info.bite)
					item->goal_anim_state = WOLF_BITE;
				else if (info.distance > WOLF_STALK_RANGE)
					item->goal_anim_state = WOLF_RUN;
				else if (creature->mood == ATTACK_MOOD)
				{
					if (!info.ahead || (info.distance > WOLF_ATTACK_RANGE) || (info.enemy_facing < ANGLE(45) && info.enemy_facing > -ANGLE(45)))
						item->goal_anim_state = WOLF_RUN;
				}
				else if (GetRandomControl() < WOLF_HOWL_CHANCE)
				{
					item->required_anim_state = WOLF_HOWL;
					item->goal_anim_state = WOLF_CROUCH;
				}
				else if (creature->mood == BORED_MOOD)
					item->goal_anim_state = WOLF_CROUCH;
				break;
			case WOLF_ATTACK:
				tilt = angle;

				if (creature->flags == 0 && item->required_anim_state == 0 && (item->touch_bits & WOLF_TOUCH_BITS))
				{
					CreatureEffect(item, &WolfJawBite, DoBloodSplat);
					lara_item->hit_points -= WOLF_POUNCE_DAMAGE;
					lara_item->hit_status = 1;
					creature->flags = 1;
				}

				item->goal_anim_state = WOLF_RUN;
				break;
			case WOLF_BITE:
				if (creature->flags == 0 && item->required_anim_state == 0 && (item->touch_bits & WOLF_TOUCH_BITS) && info.ahead)
				{
					CreatureEffect(item, &WolfJawBite, DoBloodSplat);
					lara_item->hit_points -= WOLF_BITE_DAMAGE;
					lara_item->hit_status = 1;
					creature->flags = 1;
				}
				break;
			case WOLF_SLEEP:
				creature->maximum_turn = 0;
				headY = 0; // reset head until the wolf have other states...

				if (creature->mood == ESCAPE_MOOD || info.zone_number == info.enemy_zone)
				{
					item->required_anim_state = WOLF_CROUCH;
					item->goal_anim_state = WOLF_STOP;
				}
				else if (GetRandomControl() < WOLF_WAKE_CHANCE)
				{
					item->required_anim_state = WOLF_WALK;
					item->goal_anim_state = WOLF_STOP;
				}
				break;
			}
		}

		CreatureTilt(item, tilt);
		CreatureJoint(item, 0, headY);
		CreatureJoint(item, 1, headX);
		CreatureAnimation(item_number, angle, tilt);
	}
}
