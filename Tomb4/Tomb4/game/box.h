#pragma once

constexpr auto NOT_TARGETABLE = -16384; // NOTE: Enemies like baddy can't be targeted by lara anymore.

void CreatureDie(short item_number, long explode);
void InitialiseCreature(short item_number);
long CreatureActive(short item_number);
void CreatureAIInfo(ItemInfo* item, AIInfo* info);
long SearchLOT(LOT_INFO* LOT, long expansion);
long UpdateLOT(LOT_INFO* LOT, long expansion);
void TargetBox(LOT_INFO* LOT, short box_number);
long EscapeBox(ItemInfo* item, ItemInfo* enemy, short box_number);
long ValidBox(ItemInfo* item, short zone_number, short box_number);
long StalkBox(ItemInfo* item, ItemInfo* enemy, short box_number);
target_type CalculateTarget(PHD_VECTOR* target, ItemInfo* item, LOT_INFO* LOT);
void CreatureMood(ItemInfo* item, AIInfo* info, long violent);
void GetCreatureMood(ItemInfo* item, AIInfo* info, long violent);
long CreatureCreature(short item_number);
long BadFloor(long x, long y, long z, long box_height, long next_height, short room_number, LOT_INFO* LOT);
long CreatureAnimation(short item_number, short angle, short tilt);
short CreatureTurn(ItemInfo* item, short maximum_turn);
void CreatureTilt(ItemInfo* item, short angle);
void CreatureJoint(ItemInfo* item, short joint, short required);
void CreatureFloat(short item_number);
void CreatureUnderwater(ItemInfo* item, long depth);
short CreatureEffect(ItemInfo* item, BiteInfo* bite, short(*generate)(long x, long y, long z, short speed, short yrot, short room_number));
short CreatureEffectT(ItemInfo* item, BiteInfo* bite, short damage, short angle,
	short(*generate)(long x, long y, long z, short damage, short angle, short room_number));
long CreatureVault(short item_number, short angle, long vault, long shift);
void CreatureKill(ItemInfo* item, short kill_anim, short kill_state, short lara_anim);
void AlertAllGuards(short item_number);
void AlertNearbyGuards(ItemInfo* item);
short AIGuard(CreatureInfo* creature);
void FindAITargetObject(CreatureInfo* creature, short obj_num);
void GetAITarget(CreatureInfo* creature);
short SameZone(CreatureInfo* creature, ItemInfo* target_item);
void CreatureYRot(PHD_3DPOS* srcpos, short angle, short angadd);
long MoveCreature3DPos(PHD_3DPOS* srcpos, PHD_3DPOS* destpos, long velocity, short angdif, long angadd);
RotateBasedOnSlope RotateItemBasedOnSlope_Begin(ItemInfo* item, CreatureInfo* creature, long distance, long height_max = 512);
void RotateItemBasedOnSlope_End(ItemInfo* item, RotateBasedOnSlope& slope, long speedInClick = 256);

extern BOX_INFO* boxes;
extern ushort* overlap;
extern short* ground_zone[5][2];
extern long num_boxes;
