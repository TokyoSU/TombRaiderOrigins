#pragma once

void InitialiseBike(short item_number);
void DrawBikeExtras(ItemInfo* item);
void TriggerBikeBeam(ItemInfo* item);
long GetOnBike(short item_number, COLL_INFO* coll);
void DrawBikeBeam(ItemInfo* item);
void BikeExplode(ItemInfo* item);
void AnimateBike(ItemInfo* item, long hitWall, long killed);
void BikeStart(ItemInfo* item, ItemInfo* l);
long TestHeight(ItemInfo* item, long z, long x, PHD_VECTOR* pos);
void BikeCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
long BikeBaddieCollision(ItemInfo* bike);
void BikeCollideStaticObjects(long x, long y, long z, short room_number, long height);
long BikeDynamics(ItemInfo* item);
void BikeControl(short item_number);
