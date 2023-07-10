#include "../tomb3/pch.h"
#include "tr1_bat.h"
#include "control.h"
#include "box.h"
#include "effects.h"
#include "lara.h"
#include "../specific/game.h"

namespace TR1
{
	enum BatAnims : short
	{
		BAT_EMPTY,
		BAT_STOP,
		BAT_FLY,
		BAT_ATTACK,
		BAT_FALL,
		BAT_DEATH
	};

	constexpr auto BAT_DAMAGE = 4;
	constexpr auto BAT_TURN = ANGLE(20);
	constexpr auto BAT_WAKE_RANGE = SQUARE(BLOCK(5));
	constexpr auto BAT_ATTACK_RANGE = SQUARE(CLICK(1));
	const BiteInfo BatBite = MakeBiteInfo(0, 16, 45, 4);

	void BatControl(short item_number)
	{
		if (!CreatureActive(item_number))
			return;

		auto* item = &items[item_number];
		auto* creature = GetCreatureInfo(item);
		short angle = 0;

		if (item->hit_points <= 0)
		{
			if (item->pos.y_pos < item->floor)
			{
				item->gravity_status = TRUE;
				item->goal_anim_state = BAT_FALL;
				item->speed = 0;
			}
			else if (item->pos.y_pos >= item->floor)
			{
				item->gravity_status = FALSE;
				item->goal_anim_state = BAT_DEATH;
				item->pos.y_pos = item->floor;
			}
		}
		else
		{
			AI_INFO info;
			CreatureAIInfo(item, &info);
			GetCreatureMood(item, &info, 0);
			if (creature->flags != 0)
				creature->mood = ESCAPE_MOOD;
			CreatureMood(item, &info, 0);
			angle = CreatureTurn(item, BAT_TURN);

			switch (item->current_anim_state)
			{
			case BAT_STOP:
				if (info.distance < BAT_WAKE_RANGE || item->hit_status || creature->hurt_by_lara)
					item->goal_anim_state = BAT_FLY;
				break;
			case BAT_FLY:
				creature->flags = 0;
				if (item->touch_bits)
					item->goal_anim_state = BAT_ATTACK;
				break;
			case BAT_ATTACK:
				if (creature->flags == 0 && item->touch_bits)
				{
					CreatureEffect(item, &BatBite, DoBloodSplat);
					DamageTarget(creature->enemy, BAT_DAMAGE);
					creature->flags = 1;
				}
				else
				{
					item->goal_anim_state = BAT_FLY;
					creature->mood = BORED_MOOD;
				}
				break;
			}
		}

		CreatureAnimation(item_number, angle, 0);
	}
}
