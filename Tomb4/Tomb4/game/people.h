#pragma once

short GunShot(long x, long y, long z, short speed, short yrot, short room_number);
short GunHit(long x, long y, long z, short speed, short yrot, short room_number);
short GunMiss(long x, long y, long z, short speed, short yrot, short room_number);
long TargetVisible(ItemInfo* item, AIInfo* info);
long Targetable(ItemInfo* item, AIInfo* info);
long ShotLara(ItemInfo* item, AIInfo* info, BiteInfo* gun, short extra_rotation, long damage);
