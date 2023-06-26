#pragma once

void ControlMapper(short item_number);
void ControlLightningConductor(short item_number);
void BridgeFlatFloor(ItemInfo* item, long x, long y, long z, long* height);
void BridgeFlatCeiling(ItemInfo* item, long x, long y, long z, long* height);
long GetOffset(ItemInfo* item, long x, long z);
void BridgeTilt1Floor(ItemInfo* item, long x, long y, long z, long* height);
void BridgeTilt1Ceiling(ItemInfo* item, long x, long y, long z, long* height);
void BridgeTilt2Floor(ItemInfo* item, long x, long y, long z, long* height);
void BridgeTilt2Ceiling(ItemInfo* item, long x, long y, long z, long* height);
void StatuePlinthCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void TriggerRopeFlame(PHD_VECTOR* pos);
void ControlBurningRope(short item_number);
void BurningRopeCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void ControlWaterfall(short item_number);
void AnimateWaterfalls();
void ControlTriggerTriggerer(short item_number);
void PoleCollision(short item_num, ItemInfo* l, COLL_INFO* coll);
void ControlAnimatingSlots(short item_number);
void SmashObjectControl(short item_number);
void SmashObject(short item_number);
void EarthQuake(short item_number);

extern OBJECT_INFO objects[NUMBER_OBJECTS];
