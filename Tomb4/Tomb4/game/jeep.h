#pragma once

void InitialiseJeep(short item_number);
void DrawJeepExtras(ItemInfo* item);
void JeepExplode(ItemInfo* item);
void JeepCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
long GetCollisionAnim(ItemInfo* item, PHD_VECTOR* pos, BIKEINFO* vehicle);
long DoShift(ItemInfo* item, PHD_VECTOR* newPos, PHD_VECTOR* oldPos);
void JeepBaddieCollision(ItemInfo* item);
void JeepCollideStaticObjects(long x, long y, long z, short room_number, long height);
long JeepDynamics(ItemInfo* item);
void JeepControl(short item_number);
void JeepStart(ItemInfo* item, ItemInfo* l);
void JeepFireGrenade(ItemInfo* item);
void InitialiseEnemyJeep(short item_number);
void EnemyJeepControl(short item_number);
