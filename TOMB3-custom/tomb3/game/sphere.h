#pragma once
#include "../global/types.h"

void InitInterpolate2(long frac, long rate);
long GetSpheres(ITEM_INFO* item, SPHERE* ptr, long WorldSpace);
long TestCollision(ITEM_INFO* item, ITEM_INFO* l);
void GetJointAbsPosition(ITEM_INFO* item, PHD_VECTOR* pos, long joint);
void GetJointAbsMatrix(ITEM_INFO* item, long* mptr, long joint);
void GetJointAbsPositionMatrix(ITEM_INFO* item, PHD_VECTOR* pos, long* mptr, long joint);
