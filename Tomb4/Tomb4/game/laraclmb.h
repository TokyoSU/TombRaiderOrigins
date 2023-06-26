#pragma once

void lara_as_climbstnc(ItemInfo* item, COLL_INFO* coll);
void lara_as_climbleft(ItemInfo* item, COLL_INFO* coll);
void lara_col_climbleft(ItemInfo* item, COLL_INFO* coll);
void lara_as_climbright(ItemInfo* item, COLL_INFO* coll);
void lara_col_climbright(ItemInfo* item, COLL_INFO* coll);
void lara_as_climbing(ItemInfo* item, COLL_INFO* coll);
void lara_as_climbdown(ItemInfo* item, COLL_INFO* coll);
void lara_as_climbend(ItemInfo* item, COLL_INFO* coll);
void lara_col_climbstnc(ItemInfo* item, COLL_INFO* coll);
void lara_col_climbing(ItemInfo* item, COLL_INFO* coll);
void lara_col_climbdown(ItemInfo* item, COLL_INFO* coll);
short GetClimbTrigger(long x, long y, long z, short room_number);
long LaraTestClimb(long x, long y, long z, long xfront, long zfront, long item_height, short item_room, long* shift);
long LaraTestClimbPos(ItemInfo* item, long front, long right, long origin, long height, long* shift);
long LaraTestClimbUpPos(ItemInfo* item, long front, long right, long* shift, long* ledge);
long LaraCheckForLetGo(ItemInfo* item, COLL_INFO* coll);
long LaraClimbLeftCornerTest(ItemInfo* item, COLL_INFO* coll);
long LaraClimbRightCornerTest(ItemInfo* item, COLL_INFO* coll);
void LaraDoClimbLeftRight(ItemInfo* item, COLL_INFO* coll, long result, long shift);
