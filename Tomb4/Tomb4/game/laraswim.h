#pragma once

void lara_as_swim(ItemInfo* item, COLL_INFO* coll);
void LaraUnderWater(ItemInfo* item, COLL_INFO* coll);
void lara_col_swim(ItemInfo* item, COLL_INFO* coll);
void lara_col_glide(ItemInfo* item, COLL_INFO* coll);
void lara_col_tread(ItemInfo* item, COLL_INFO* coll);
void lara_col_dive(ItemInfo* item, COLL_INFO* coll);
void lara_col_waterroll(ItemInfo* item, COLL_INFO* coll);
void lara_as_glide(ItemInfo* item, COLL_INFO* coll);
void lara_as_tread(ItemInfo* item, COLL_INFO* coll);
void lara_as_dive(ItemInfo* item, COLL_INFO* coll);
void lara_as_uwdeath(ItemInfo* item, COLL_INFO* coll);
void lara_as_waterroll(ItemInfo* item, COLL_INFO* coll);
void lara_col_uwdeath(ItemInfo* item, COLL_INFO* coll);
long GetWaterDepth(long x, long y, long z, short room_number);
void SwimTurn(ItemInfo* item);
void LaraTestWaterDepth(ItemInfo* item, COLL_INFO* coll);
void LaraSwimCollision(ItemInfo* item, COLL_INFO* coll);
void LaraWaterCurrent(COLL_INFO* coll);
