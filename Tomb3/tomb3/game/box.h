#pragma once
#include "../global/types.h"

constexpr auto NO_BOX = 2047;
constexpr auto BLOCKED_SEARCH = 0x8000;
constexpr auto BLOCK_SEARCH = 0x4000;
constexpr auto FLY_SEARCH = 0x2000;

void AlertNearbyGuards(ITEM_INFO* item);
CREATURE_INFO* GetCreatureInfo(ITEM_INFO* item);
void InitialiseCreature(short item_number);
void InitialiseCreature(ITEM_INFO* item);
long CreatureActive(short item_number);
void CreatureAIInfo(ITEM_INFO* item, AI_INFO* info);
long SearchLOT(LOT_INFO* LOT, long expansion);
long UpdateLOT(LOT_INFO* LOT, long expansion);
void TargetBox(LOT_INFO* LOT, short box_number);
long EscapeBox(ITEM_INFO* item, ITEM_INFO* enemy, short box_number);
long ValidBox(ITEM_INFO* item, short zone_number, short box_number);
long StalkBox(ITEM_INFO* item, ITEM_INFO* enemy, short box_number);
target_type CalculateTarget(PHD_VECTOR* target, ITEM_INFO* item, LOT_INFO* LOT);
void CreatureMood(ITEM_INFO* item, AI_INFO* info, long violent);
void GetCreatureMood(ITEM_INFO* item, AI_INFO* info, long violent);
bool BadFloor(long x, long y, long z, long box_height, long next_height, short room_number, LOT_INFO* LOT);
long CreatureCreature(short item_number);
void CreatureDie(short item_number, long explode);
short CreatureTurn(ITEM_INFO* item, short maximum_turn);
long CreatureAnimation(short item_number, short angle, short tilt);
void CreatureTilt(ITEM_INFO* item, short angle);
void CreatureJoint(ITEM_INFO* item, short joint, short required, short maxAngleUp = ANGLE(70), short maxAngleDown = ANGLE(70));
void CreatureFloat(short item_number);
void CreatureUnderwater(ITEM_INFO* item, long depth);
short CreatureEffect(ITEM_INFO* item, const BiteInfo* bite, short(*generate)(long x, long y, long z, short speed, short yrot, short room_number));
long CreatureVault(short item_number, short angle, long vault, long shift);
void CreatureKill(ITEM_INFO* item, long kill_anim, long kill_state, long lara_kill_state);
short AIGuard(CREATURE_INFO* creature);
void AlertAllGuards(short item_number);
bool SameZone(CREATURE_INFO* creature, ITEM_INFO* target_item);
void GetAITarget(CREATURE_INFO* creature);
void AdjustStopperFlag(ITEM_INFO* item, long dir, long set);

extern BOX_INFO* boxes;
extern long number_boxes;
extern short* overlap;

extern short* ground_zone[5][2];
