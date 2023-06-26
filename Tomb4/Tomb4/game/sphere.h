#pragma once

long GetSpheres(ItemInfo* item, SPHERE* ptr, long WorldSpace);
long TestCollision(ItemInfo* item, ItemInfo* l);
void InitInterpolate2(long frac, long rate);
void GetJointAbsPosition(ItemInfo* item, PHD_VECTOR* pos, long joint);

extern SPHERE Slist[34];
extern char GotLaraSpheres;
