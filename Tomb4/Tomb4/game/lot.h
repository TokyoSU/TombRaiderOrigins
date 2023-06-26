#pragma once

void InitialiseLOTarray(long allocmem);
void DisableBaddieAI(short item_number);
void ClearLOT(LOT_INFO* lot);
void CreateZone(ItemInfo* item);
void InitialiseSlot(short item_number, long slot);
long EnableBaddieAI(short item_number, long Always);

extern CreatureInfo* baddie_slots;
