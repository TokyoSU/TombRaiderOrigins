#pragma once
#include "global/types.h"

struct SNOWMOBILEINFO
{
	short track_mesh;
	int skidoo_turn;
	int left_fallspeed, right_fallspeed;
	short momentum_angle, extra_rotation;
	int pitch;
	bool armed;
};

extern void InitialiseSkidoo(short item_number);
extern int SkidooCheckGetOn(short item_number, COLL_INFO* coll);
extern void SkidooCollision(short item_number, ITEM_INFO* litem, COLL_INFO* coll);
extern void SkidooBaddieCollision(ITEM_INFO* skidoo);
extern int SkidooTestHeight(ITEM_INFO* item, int z_off, int x_off, PHD_VECTOR* pos);
extern int SkidooDoShift(ITEM_INFO* skidoo, PHD_VECTOR* pos, PHD_VECTOR* old);
extern int SkidooDoDynamics(long height, int fallspeed, long* y);
extern int SkidooGetCollisionAnim(ITEM_INFO* skidoo, PHD_VECTOR* moved);
extern void SkidooDoSnowEffect(ITEM_INFO* skidoo);
extern int SkidooDynamics(ITEM_INFO* skidoo);
extern int SkidooUserControl(ITEM_INFO* skidoo, int height, int* pitch);
extern int SkidooCheckGetOffOK(int direction);
extern void SkidooAnimation(ITEM_INFO* skidoo, int collide, int dead);
extern void SkidooExplode(ITEM_INFO* skidoo);
extern int SkidooCheckGetOff();
extern void SkidooGuns();
extern void SnowmobileStart(ITEM_INFO* skidoo, ITEM_INFO* laraitem);
extern void SkidooControl(short item_number);
extern void DrawSkidoo(ITEM_INFO* item);
extern void DrawSkidooExtras(ITEM_INFO* item);
