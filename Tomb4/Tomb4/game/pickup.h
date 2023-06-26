#pragma once

void SarcophagusCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void KeyHoleCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void PuzzleDoneCollision(short item_num, ItemInfo* l, COLL_INFO* coll);
void PuzzleDone(ItemInfo* item, short item_number);
void AnimatingPickUp(short item_number);
short* FindPlinth(ItemInfo* item);
long KeyTrigger(short item_num);
long PickupTrigger(short item_num);
void RegeneratePickups();
void PickUpCollision(short item_number, ItemInfo* l, COLL_INFO* coll);
void PuzzleHoleCollision(short item_num, ItemInfo* l, COLL_INFO* coll);

extern uchar NumRPickups;
extern uchar RPickups[16];
extern char KeyTriggerActive;
