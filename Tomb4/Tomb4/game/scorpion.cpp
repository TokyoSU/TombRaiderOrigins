#include "pch.h"
#include "scorpion.h"
#include "box.h"
#include "objects.h"
#include "3dmath.h"
#include "control.h"
#include "lot.h"
#include "items.h"
#include "function_stubs.h"
#include "effects.h"
#include "lara.h"
#include "deltapak.h"
#include "gameflow.h"

static BiteInfo stinger{ 0, 0, 0, 8 };		// for the big one
static BiteInfo pincer{ 0, 0, 0, 23 };

static BiteInfo s_stinger{ 0, 0, 0, 8 };	// for the small one
static BiteInfo s_pincer{ 0, 0, 0, 23 };

void InitialiseScorpion(short item_number)
{
	ItemInfo* item = &items[item_number];
	InitialiseCreature(item_number);
	if (item->trigger_flags == 1)
		SetAnimation(item, 7);
	else
		SetAnimation(item, 2);
}

void ScorpionControl(short item_number)
{
	AIInfo info;
	long s, c, dist, bestdist;
	short target_num, hp;

	if (!CreatureActive(item_number))
		return;

	auto* item = &items[item_number];
	auto* creature = (CreatureInfo*)item->data;
	short angle = 0;
	auto slope = RotateItemBasedOnSlope_Begin(item, creature, 684);

	if (item->hit_points <= 0)
	{
		item->hit_points = 0;
		s = 1;

		if (item->current_anim_state != 6)
		{
			if (item->trigger_flags > 0 && item->trigger_flags < 7)
			{
				cutseq_num = 4;
				item->anim_number = objects[SCORPION].anim_index + 5;
				item->frame_number = anims[item->anim_number].frame_base;
				item->current_anim_state = 6;
				item->status = ITEM_INVISIBLE;
				creature->maximum_turn = 0;

				for (target_num = room[item->room_number].item_number; target_num != NO_ITEM; target_num = creature->enemy->next_item)
				{
					auto* enemy = &items[target_num];
					if (enemy->object_number == TROOPS && enemy->trigger_flags == 1)
					{
						DisableBaddieAI(target_num);
						KillItem(target_num);
						enemy->flags |= IFL_CLEARBODY;
						break;
					}
				}

				s = 0;
			}
			else if (item->current_anim_state != 7)
			{
				item->anim_number = objects[SCORPION].anim_index + 5;
				item->frame_number = anims[item->anim_number].frame_base;
				item->current_anim_state = 6;
				s = 0;
			}
		}

		if (s)
		{
			if (cutseq_num == 4)
			{
				item->frame_number = anims[item->anim_number].frame_end - 1;
				item->status = ITEM_INVISIBLE;
			}
			else if (item->current_anim_state == 6)
			{
				if (item->status == ITEM_INVISIBLE)
					item->status = ITEM_ACTIVE;
			}
		}
	}
	else
	{
		if (item->ai_bits)
		{
			GetAITarget(creature);
		}
		else
		{
			if (creature->hurt_by_lara && item->current_anim_state != 8)
			{
				creature->enemy = lara_item;
			}
			else
			{
				creature->enemy = nullptr;
				bestdist = 0x7FFFFFFF;

				for (int i = 0; i < 5; i++)
				{
					target_num = baddie_slots[i].item_num;

					if (target_num != NO_ITEM && target_num != item_number)
					{
						auto* enemy = &items[target_num];
						if (enemy->object_number != LARA && enemy->object_number != SCORPION && (enemy != lara_item || creature->hurt_by_lara))
						{
							s = enemy->pos.x_pos - item->pos.x_pos;
							c = enemy->pos.z_pos - item->pos.z_pos;
							dist = SQUARE(s) + SQUARE(c);
							if (dist < bestdist)
							{
								creature->enemy = enemy;
								bestdist = dist;
							}
						}
					}
				}
			}
		}

		CreatureAIInfo(item, &info);
		GetCreatureMood(item, &info, 1);
		CreatureMood(item, &info, 1);
		angle = CreatureTurn(item, creature->maximum_turn);

		switch (item->current_anim_state)
		{
		case 1:
			creature->maximum_turn = 0;
			creature->flags = 0;

			if (info.distance > 0x1C6E39)
				item->goal_anim_state = 2;
			else if (info.bite)
			{
				creature->maximum_turn = 364;
				if ((GetRandomControl() & 1) || (creature->enemy->object_number == TROOPS && creature->enemy->hit_points <= 15))
					item->goal_anim_state = 4;
				else
					item->goal_anim_state = 5;
			}
			else if (!info.ahead)
				item->goal_anim_state = 2;

			break;

		case 2:
			creature->maximum_turn = 364;

			if (info.distance < 0x1C6E39)
				item->goal_anim_state = 1;
			else
				item->goal_anim_state = 3;

			break;

		case 3:
			creature->maximum_turn = 546;

			if (info.distance < 0x1C6E39)
				item->goal_anim_state = 1;

			break;

		case 4:
		case 5:
			hp = lara_item->hit_points;

			if (abs(info.angle) < 364)
				item->pos.y_rot += info.angle;
			else if (info.angle < 0)
				item->pos.y_rot -= 364;
			else
				item->pos.y_rot += 364;

			if (!creature->flags && creature->enemy && creature->enemy != lara_item && info.distance < 0x1C6E39)
			{
				creature->DamageTarget(15);
				if (creature->enemy->hit_points <= 0)
				{
					item->goal_anim_state = 7;
					creature->maximum_turn = 0;
				}
				creature->flags = 1;
				CreatureEffectT(item, &stinger, 10, item->pos.y_rot + 0x8000, DoBloodSplat);
			}
			else if (!creature->flags && item->touch_bits & 0x1B00100)
			{
				creature->DamageTarget(120);

				if (item->current_anim_state == 5)
				{
					lara.dpoisoned += 2048;
					CreatureEffectT(item, &stinger, 10, item->pos.y_rot + 0x8000, DoBloodSplat);
				}
				else
				{
					item->poisoned += 2048;
					CreatureEffectT(item, &pincer, 10, item->pos.y_rot + 0x8000, DoBloodSplat);
				}

				creature->flags = 1;
				if (hp && lara_item->hit_points <= 0)
				{
					CreatureKill(item, 6, 7, 442);
					creature->maximum_turn = 0;
					return;
				}
			}

			break;

		case 8:
			creature->maximum_turn = 0;

			if (item->frame_number == anims[item->anim_number].frame_end)
				item->trigger_flags++;

			if ((creature->enemy && creature->enemy->hit_points <= 0) || item->trigger_flags > 6)
			{
				item->goal_anim_state = 7;
				creature->enemy->hit_points = 0;
			}

			break;
		}
	}

	RotateItemBasedOnSlope_End(item, slope);
	if (!cutseq_num)
		CreatureAnimation(item_number, angle, 0);
}

void InitialiseSmallScorpion(short item_number)
{
	ItemInfo* item = &items[item_number];
	InitialiseCreature(item_number);
	SetAnimation(item, 2);
}

void SmallScorpionControl(short item_number)
{
	ItemInfo* item;
	ItemInfo* enemy;
	CreatureInfo* scorpion;
	AIInfo info;
	short angle;

	if (!CreatureActive(item_number))
		return;

	angle = 0;
	item = &items[item_number];
	scorpion = (CreatureInfo*)item->data;

	if (item->hit_points <= 0)
	{
		item->hit_points = 0;

		if (item->current_anim_state != 6 && item->current_anim_state != 7)
		{
			item->anim_number = objects[SMALL_SCORPION].anim_index + 5;
			item->frame_number = anims[item->anim_number].frame_base;
			item->current_anim_state = 6;
		}
	}
	else
	{
		if (item->ai_bits)
			GetAITarget(scorpion);
		else
			scorpion->enemy = lara_item;

		CreatureAIInfo(item, &info);
		enemy = scorpion->enemy;

		if (enemy != lara_item)
			phd_atan(lara_item->pos.z_pos - item->pos.z_pos, lara_item->pos.x_pos - item->pos.x_pos);

		GetCreatureMood(item, &info, 1);
		CreatureMood(item, &info, 1);
		angle = CreatureTurn(item, scorpion->maximum_turn);

		switch (item->current_anim_state)
		{
		case 1:
			scorpion->maximum_turn = 0;
			scorpion->flags = 0;

			if (info.distance > 0x1C6E39)
				item->goal_anim_state = 2;
			else if (info.bite)
			{
				scorpion->maximum_turn = 1092;

				if (GetRandomControl() & 1 || enemy->object_number == TROOPS && enemy->hit_points <= 2)
					item->goal_anim_state = 4;
				else
					item->goal_anim_state = 5;
			}
			else if (!info.ahead)
				item->goal_anim_state = 2;

			break;

		case 2:
			scorpion->maximum_turn = 1092;

			if (info.distance >= 0x1C639)
				item->goal_anim_state = 3;
			else
				item->goal_anim_state = 1;

			break;

		case 3:
			scorpion->maximum_turn = 1456;

			if (info.distance < 0x1C639)
				item->goal_anim_state = 1;

			break;

		case 4:
		case 5:
			scorpion->maximum_turn = 0;

			if (abs(info.angle) < 1092)
				item->pos.y_rot += info.angle;
			else if (info.angle < 0)
				item->pos.y_rot -= 1092;
			else
				item->pos.y_rot += 1092;

			if (!scorpion->flags && item->touch_bits & 0x1B00100)
			{
				if (item->frame_number > anims[item->anim_number].frame_base + 20 && item->frame_number < anims[item->anim_number].frame_base + 32)
				{
					lara_item->hit_points -= 20;
					lara_item->hit_status = 1;

					if (item->current_anim_state == 5)
					{
						if (gfCurrentLevel > 3)
							lara.dpoisoned += 512;

						CreatureEffectT(item, &s_stinger, 3, item->pos.y_rot + 0x8000, DoBloodSplat);
					}
					else
						CreatureEffectT(item, &s_pincer, 3, item->pos.y_rot + 0x8000, DoBloodSplat);

					scorpion->flags = 1;
				}
			}

			break;
		}
	}

	CreatureAnimation(item_number, angle, 0);
}