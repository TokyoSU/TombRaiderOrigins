#include "pch.h"
#include "wolf.h"
#include "box.h"
#include "lot.h"
#include "control.h"
#include "game.h"
#include "lara.h"
#include "effects.h"
#include <sound.h>

#define MESH_BITS(x) 1 << x

#define WOLF_TOUCH (0x774F)

enum wolf_anims {
    WOLF_EMPTY, WOLF_STOP, WOLF_WALK, WOLF_RUN, WOLF_JUMP, WOLF_STALK, WOLF_ATTACK,
    WOLF_HOWL, WOLF_SLEEP, WOLF_CROUCH, WOLF_FASTTURN, WOLF_DEATH, WOLF_BITE
};

#define WOLF_BITE_DAMAGE   100
#define WOLF_POUNCE_DAMAGE 50

#define WOLF_SLEEP_FRAME 96
#define WOLF_SLEEP_ANIM 0
#define WOLF_DIE_ANIM  20

#define WOLF_WALK_TURN  (2*182)
#define WOLF_RUN_TURN   (5*182)
#define WOLF_STALK_TURN (2*182)

#define WOLF_ATTACK_RANGE SQUARE(1024*3/2)
#define WOLF_STALK_RANGE  SQUARE(1024*3)
#define WOLF_BITE_RANGE	  SQUARE(345)

#define WOLF_WAKE_CHANCE  0x20
#define WOLF_SLEEP_CHANCE 0x20
#define WOLF_HOWL_CHANCE  0x180

BITE_INFO	wolf_jaw_bite = { 0,-14,174,6 };

void InitialiseWolf(short item_number)
{
    auto* item = &items[item_number];
    InitialiseCreature(item_number);
    SetAnimation(item, WOLF_SLEEP_ANIM, WOLF_SLEEP_FRAME);
}

void WolfControl(short item_number)
{
    if (!CreatureActive(item_number))
        return;

    ITEM_INFO* item = &items[item_number];
    CREATURE_INFO* wolf = (CREATURE_INFO*)item->data;
    AI_INFO info{};
    short head = 0, angle = 0, tilt = 0;

	if (item->hit_status)
		SoundEffect(SFX_WOLF_HURT, &item->pos, SFX_ALWAYS);

    if (item->hit_points <= 0)
    {
		if (item->current_anim_state != WOLF_DEATH)
			SetAnimation(item, WOLF_DIE_ANIM);
    }
    else
    {
		CreatureAIInfo(item, &info);

		if (info.ahead)
			head = info.angle;

		GetCreatureMood(item, &info, TIMID);
		CreatureMood(item, &info, TIMID);
		angle = CreatureTurn(item, wolf->maximum_turn);

        switch (item->current_anim_state)
        {
        case WOLF_SLEEP:
            head = 0;

            if (wolf->mood == ESCAPE_MOOD || info.zone_number == info.enemy_zone)
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
		case WOLF_STOP:
			if (item->required_anim_state)
				item->goal_anim_state = item->required_anim_state;
			else
				item->goal_anim_state = WOLF_WALK;
			break;

		case WOLF_WALK:
			wolf->maximum_turn = WOLF_WALK_TURN;

			if (wolf->mood != BORED_MOOD)
			{
				item->goal_anim_state = WOLF_STALK;
				item->required_anim_state = WOLF_EMPTY;
			}
			else if (GetRandomControl() < WOLF_SLEEP_CHANCE)
			{
				item->required_anim_state = WOLF_SLEEP;
				item->goal_anim_state = WOLF_STOP;
			}
			break;

		case WOLF_CROUCH:
			if (item->required_anim_state)
				item->goal_anim_state = item->required_anim_state;
			else if (wolf->mood == ESCAPE_MOOD)
				item->goal_anim_state = WOLF_RUN;
			else if (info.distance < WOLF_BITE_RANGE && info.bite)
				item->goal_anim_state = WOLF_BITE;
			else if (wolf->mood == STALK_MOOD)
				item->goal_anim_state = WOLF_STALK;
			else if (wolf->mood == BORED_MOOD)
				item->goal_anim_state = WOLF_STOP;
			else
				item->goal_anim_state = WOLF_RUN;
			break;

		case WOLF_STALK:
			wolf->maximum_turn = WOLF_STALK_TURN;

			if (wolf->mood == ESCAPE_MOOD)
				item->goal_anim_state = WOLF_RUN;
			else if (info.distance < WOLF_BITE_RANGE && info.bite)
				item->goal_anim_state = WOLF_BITE;
			else if (info.distance > WOLF_STALK_RANGE)
				item->goal_anim_state = WOLF_RUN;
			else if (wolf->mood == ATTACK_MOOD)
			{
				if (!info.ahead || info.distance > WOLF_ATTACK_RANGE ||
					(info.enemy_facing < FRONT_ARC && info.enemy_facing > -FRONT_ARC))
					item->goal_anim_state = WOLF_RUN;
			}
			else if (GetRandomControl() < WOLF_HOWL_CHANCE)
			{
				item->required_anim_state = WOLF_HOWL;
				item->goal_anim_state = WOLF_CROUCH;
			}
			else if (wolf->mood == BORED_MOOD)
				item->goal_anim_state = WOLF_CROUCH;
			break;

		case WOLF_RUN:
			wolf->maximum_turn = WOLF_RUN_TURN;
			tilt = angle / 2;

			if (info.ahead && info.distance < WOLF_ATTACK_RANGE)
			{
				if (info.distance > (WOLF_ATTACK_RANGE / 2) && (info.enemy_facing > FRONT_ARC || info.enemy_facing < -FRONT_ARC))
				{
					item->required_anim_state = WOLF_STALK;
					item->goal_anim_state = WOLF_CROUCH;
				}
				else
				{
					item->goal_anim_state = WOLF_ATTACK;
					item->required_anim_state = 0;
				}
			}
			else if (wolf->mood == STALK_MOOD && info.distance < WOLF_STALK_RANGE)
			{
				item->required_anim_state = WOLF_STALK;
				item->goal_anim_state = WOLF_CROUCH;
			}
			else if (wolf->mood == BORED_MOOD)
				item->goal_anim_state = WOLF_CROUCH;
			break;

		case WOLF_ATTACK:
			tilt = angle / 2;
			if (!item->required_anim_state && (item->touch_bits & WOLF_TOUCH))
			{
				CreatureEffect(item, &wolf_jaw_bite, DoBloodSplat);
				lara_item->hit_points -= WOLF_POUNCE_DAMAGE;
				lara_item->hit_status = 1;
				item->required_anim_state = WOLF_RUN;
			}
			item->goal_anim_state = WOLF_RUN;
			break;

		case WOLF_BITE:
			if (!item->required_anim_state && (item->touch_bits & WOLF_TOUCH) && info.ahead)
			{
				CreatureEffect(item, &wolf_jaw_bite, DoBloodSplat);
				lara_item->hit_points -= WOLF_BITE_DAMAGE;
				lara_item->hit_status = 1;
				item->required_anim_state = WOLF_CROUCH;
			}
			break;
        }
    }

	CreatureTilt(item, tilt);
    CreatureJoint(item, 0, head);
    CreatureAnimation(item_number, angle, tilt);
}
