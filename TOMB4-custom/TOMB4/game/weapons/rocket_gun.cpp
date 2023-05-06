#include "pch.h"
#include "rocket_gun.h"
#include "larafire.h"
#include "items.h"
#include "lara.h"
#include "control.h"
#include "objects.h"
#include "delstuff.h"
#include "sound.h"
#include "savegame.h"
#include "tomb4fx.h"
#include "specific/3dmath.h"
#include "specific/function_stubs.h"
#include "effect2.h"

constexpr auto ROCKET_BLAST_RADIUS = 512;

void FireRocket()
{
	short& ammo = get_current_ammo_pointer(WEAPON_ROCKET);
	if (ammo == 0)
		return;

	short item_number = CreateItem();
	if (item_number == NO_ITEM)
		return;

	lara.has_fired = 1;
	auto* item = &items[item_number];
	item->object_number = ROCKET;
	item->room_number = lara_item->room_number;
	item->shade = -0x3DF0;
	if (ammo != -1)
		ammo--;

	PHD_VECTOR vec;
	vec.x = 0;
	vec.y = ROCKET_YOFF;
	vec.z = ROCKET_ZOFF;
	GetLaraJointPos(&vec, 11);
	item->pos.x_pos = vec.x;
	item->pos.y_pos = vec.y;
	item->pos.z_pos = vec.z;
	PHD_VECTOR vec2;
	vec2.x = 0;
	vec2.y = ROCKET_YOFF+1024;
	vec2.z = ROCKET_ZOFF;
	GetLaraJointPos(&vec2, 11);

	for (int lp = 0; lp < 6; lp++)
		TriggerGunSmoke(vec.x, vec.y, vec.z, vec2.x - vec.x, vec2.y - vec.y, vec2.z - vec.z, 1, WEAPON_ROCKET, 32);

	InitialiseItem(item_number);
	item->pos.x_rot = lara.left_arm.x_rot + lara_item->pos.x_rot;
	item->pos.y_rot = lara.left_arm.y_rot + lara_item->pos.y_rot;
	item->pos.z_rot = 0;

	if (!lara.left_arm.lock)
	{
		item->pos.x_rot += lara.torso_x_rot;
		item->pos.y_rot += lara.torso_y_rot;
	}

	item->speed = ROCKET_SPEED;
	item->item_flags[0] = 0;
	AddActiveItem(item_number);

	phd_PushUnitMatrix();
	phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
	phd_PushMatrix();
	phd_TranslateRel(0, 0, -128);
	long wx = phd_mxptr[M03] >> W2V_SHIFT;
	long wy = phd_mxptr[M13] >> W2V_SHIFT;
	long wz = phd_mxptr[M23] >> W2V_SHIFT;
	phd_PopMatrix();

	for (int lp = 0; lp < 8; lp++)
	{
		phd_PushMatrix();
		phd_TranslateRel(0, 0, -(GetRandomControl() & 2047));
		long xv = phd_mxptr[M03] >> W2V_SHIFT;
		long yv = phd_mxptr[M13] >> W2V_SHIFT;
		long zv = phd_mxptr[M23] >> W2V_SHIFT;
		phd_PopMatrix();
		TriggerRocketFlameTR3(wx, wy, wz, xv - wx, yv - wy, zv - wz, item_number);
	}

	savegame.Game.AmmoUsed++;
}

void ControlRocket(short item_number)
{
	auto* item = &items[item_number];
	auto old_x = item->pos.x_pos;
	auto old_y = item->pos.y_pos;
	auto old_z = item->pos.z_pos;

	if (item->speed < ROCKET_SPEED)
		item->speed += (item->speed >> 3) + 2;

	item->shade = 0x2400;
	item->pos.z_rot += (((item->speed >> 2) + 7) * ONE_DEGREE);

	phd_PushUnitMatrix();
	phd_mxptr[M03] = 0;
	phd_mxptr[M13] = 0;
	phd_mxptr[M23] = 0;
	phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
	phd_PushMatrix();
	phd_TranslateRel(0, 0, -128);
	auto wx = phd_mxptr[M03] >> W2V_SHIFT;
	auto wy = phd_mxptr[M13] >> W2V_SHIFT;
	auto wz = phd_mxptr[M23] >> W2V_SHIFT;
	phd_PopMatrix();
	phd_TranslateRel(0, 0, -1024 - (GetRandomControl() & 511));
	auto xv = phd_mxptr[M03] >> W2V_SHIFT;
	auto yv = phd_mxptr[M13] >> W2V_SHIFT;
	auto zv = phd_mxptr[M23] >> W2V_SHIFT;
	phd_PopMatrix();
	if (wibble & 4)
		TriggerRocketFlameTR3(wx, wy, wz, xv - wx, yv - wy, zv - wz, item_number);
	TriggerRocketSmokeTR3(wx + item->pos.x_pos, wy + item->pos.y_pos, wz + item->pos.z_pos, -1);
	TriggerDynamic(wx + item->pos.x_pos + (GetRandomControl() & 15) - 8, wy + item->pos.y_pos + (GetRandomControl() & 15) - 8, wz + item->pos.z_pos + (GetRandomControl() & 15) - 8, 14, 28 + (GetRandomControl() & 3), 16 + (GetRandomControl() & 7), (GetRandomControl() & 7));

	auto speed = item->speed * phd_cos(item->pos.x_rot) >> W2V_SHIFT;
	item->pos.z_pos += speed * phd_cos(item->pos.y_rot) >> W2V_SHIFT;
	item->pos.x_pos += speed * phd_sin(item->pos.y_rot) >> W2V_SHIFT;
	item->pos.y_pos += item->fallspeed - (item->speed * phd_sin(item->pos.x_rot) >> W2V_SHIFT);

	auto room_number = item->room_number;
	auto* floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);
	item->floor = GetHeight(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);

	if (item->room_number != room_number)
		ItemNewRoom(item_number, room_number);

	int radius, explode;
	if (item->pos.y_pos >= item->floor || item->pos.y_pos <= GetCeiling(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos))
	{
		radius = ROCKET_BLAST_RADIUS;
		explode = 1;
	}
	else
		radius = explode = 0;

	if (explode)
	{
		auto* r = &rooms[item->room_number];
		TriggerExplosionSparks(old_x, old_y, old_z, 3, -2, r->flags & ROOM_UNDERWATER, item->room_number);
		for (int x = 0; x < 2; x++)
			TriggerExplosionSparks(old_x, old_y, old_z, 3, -1, r->flags & ROOM_UNDERWATER, item->room_number);
		Sound.PlayEffect(SFX_EXPLOSION2);
		KillItem(item_number);
	}
}
