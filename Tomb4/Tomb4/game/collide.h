#pragma once

void ShiftItem(ItemInfo* item, COLL_INFO* coll);
long GetCollidedObjects(ItemInfo* item, long rad, long noInvisible, ItemInfo** StoredItems, MESH_INFO** StoredStatics, long StoreLara);
void GenericDeadlyBoundingBoxCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void GenericSphereBoxCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void CreatureCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
long FindGridShift(long src, long dst);
short GetTiltType(FloorInfo* floor, long x, long y, long z);
long CollideStaticObjects(COLL_INFO* coll, long x, long y, long z, short room_number, long hite);
void UpdateLaraRoom(ItemInfo* item, long height);
void LaraBaddieCollision(ItemInfo* l, COLL_INFO* coll);
void ObjectCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void ObjectCollisionNoBigPush(short item_number, ItemInfo* l, COLL_INFO* coll);
void TrapCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
long ItemPushLara(ItemInfo* item, ItemInfo* l, COLL_INFO* coll, long spaz, long BigPush);
long TestBoundsCollide(ItemInfo* item, ItemInfo* l, long rad);
long TestBoundsCollideStatic(short* bounds, PHD_3DPOS* pos, long rad);
long ItemPushLaraStatic(ItemInfo* l, short* bounds, PHD_3DPOS* pos, COLL_INFO* coll);
long TestLaraPosition(short* bounds, ItemInfo* item, ItemInfo* l);
void AlignLaraPosition(PHD_VECTOR* pos, ItemInfo* item, ItemInfo* l);
long Move3DPosTo3DPos(PHD_3DPOS* pos, PHD_3DPOS* dest, long speed, short rotation);
long MoveLaraPosition(PHD_VECTOR* v, ItemInfo* item, ItemInfo* l);
long TestBoundsCollide2(ItemInfo* item, ItemInfo* l, long rad);
void StargateCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void CogCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void GetCollisionInfo(COLL_INFO* coll, long x, long y, long z, short room_number, long hite);

extern short GlobalCollisionBounds[6];
