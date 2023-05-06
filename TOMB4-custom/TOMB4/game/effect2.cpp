#include "pch.h"
#include "effect2.h"
#include "control.h"
#include "objects.h"
#include "specific/function_stubs.h"
#include "tomb4fx.h"
#include "specific/3dmath.h"
#include "missile.h"
#include "items.h"
#include "effects.h"
#include "traps.h"
#include "seth.h"
#include "demigod.h"
#include "harpy.h"
#include "croc.h"
#include "draw.h"
#include "sound.h"
#include "lara.h"
#include "gameflow.h"

SPLASH_STRUCT splashes[4];
RIPPLE_STRUCT ripples[16];
SPLASH_SETUP splash_setup;
std::vector<DYNAMIC> Dynamics;
std::vector<SPARKS> Sparks;
long wibble = 0;
long SplashCount = 0;
long KillEverythingFlag = 0;
long SmokeCountL;
long SmokeCountR;
long SmokeWeapon;
long SmokeWindX;
long SmokeWindZ;

static long DeadlyBounds[6];
static long next_spark = 0;

void ControlSmokeEmitter(short item_number)
{
	ITEM_INFO* item;
	PHD_3DPOS pos;
	long size, dx, dz, normal;

	item = &items[item_number];
	normal = 0;

	if (!TriggerActive(item))
		return;

	if (item->object_number == STEAM_EMITTER && rooms[item->room_number].flags & ROOM_UNDERWATER)
	{
		if (item->item_flags[0] || !(GetRandomControl() & 0x1F) || item->trigger_flags == 1)
		{
			if (!(GetRandomControl() & 3) || item->item_flags[1])
			{
				pos.x_pos = (GetRandomControl() & 0x3F) + item->pos.x_pos - 32;
				pos.y_pos = item->pos.y_pos - (GetRandomControl() & 0x1F) - 16;
				pos.z_pos = (GetRandomControl() & 0x3F) + item->pos.z_pos - 32;

				if (item->trigger_flags == 1)
					CreateBubble(&pos, item->room_number, 15, 15);
				else
					CreateBubble(&pos, item->room_number, 8, 7);

				if (item->item_flags[0])
				{
					item->item_flags[0]--;

					if (!item->item_flags[0])
						item->item_flags[1] = 0;
				}
			}
		}
		else if (!(GetRandomControl() & 0x1F))
			item->item_flags[0] = (GetRandomControl() & 3) + 4;

		return;
	}

	if (item->object_number == STEAM_EMITTER && item->trigger_flags & 8)
	{
		if (item->item_flags[0])
		{
			item->item_flags[0]--;

			if (!item->item_flags[0])
				item->item_flags[1] = (GetRandomControl() & 0x3F) + 30;

			normal = 1;

			if (item->item_flags[2])
				item->item_flags[2] -= 256;
		}
		else if (item->item_flags[2] < 4096)
			item->item_flags[2] += 256;

		if (item->item_flags[2])
		{
			dx = lara_item->pos.x_pos - item->pos.x_pos;
			dz = lara_item->pos.z_pos - item->pos.z_pos;
			if (dx < -16384 || dx > 16384 || dz < -16384 || dz > 16384)
				return;

			SPARKS sptr;
			sptr.On = 1;
			sptr.sR = 96;
			sptr.sG = 96;
			sptr.sB = 96;
			sptr.dR = 48;
			sptr.dG = 48;
			sptr.dB = 48;
			sptr.FadeToBlack = 6;
			sptr.ColFadeSpeed = (GetRandomControl() & 3) + 6;
			sptr.TransType = 2;
			sptr.Life = (GetRandomControl() & 7) + 16;
			sptr.sLife = sptr.Life;
			sptr.x = (GetRandomControl() & 0x3F) + item->pos.x_pos - 32;
			sptr.y = (GetRandomControl() & 0x3F) + item->pos.y_pos - 32;
			sptr.z = (GetRandomControl() & 0x3F) + item->pos.z_pos - 32;
			size = item->item_flags[2];

			if (item->item_flags[2] == 4096)
				size = (GetRandomControl() & 0x7FF) + 2048;

			sptr.Xvel = (short)((size * phd_sin(item->pos.y_rot - 32768)) >> W2V_SHIFT);
			sptr.Yvel = -16 - (GetRandomControl() & 0xF);
			sptr.Zvel = (short)((size * phd_cos(item->pos.y_rot - 32768)) >> W2V_SHIFT);
			sptr.Friction = 4;
			sptr.Flags = 538;

			if (!(GlobalCounter & 3))
				sptr.Flags = 1562;

			sptr.RotAng = GetRandomControl() & 0xFFF;

			if (GetRandomControl() & 1)
				sptr.RotAdd = -8 - (GetRandomControl() & 7);
			else
				sptr.RotAdd = (GetRandomControl() & 7) + 8;

			sptr.Scalar = 2;
			sptr.Gravity = -8 - (GetRandomControl() & 0xF);
			sptr.MaxYvel = -8 - (GetRandomControl() & 7);
			size = (GetRandomControl() & 0x1F) + 128;
			sptr.dSize = (unsigned char)size;
			sptr.sSize = sptr.dSize >> 1;
			sptr.Size = sptr.sSize;

			if (item->item_flags[1])
				item->item_flags[1]--;
			else
				item->item_flags[0] = item->trigger_flags >> 4;

			Sparks.push_back(sptr);
		}

		if (!normal)
			return;
	}

	if (!(wibble & 0xF) && (item->object_number != STEAM_EMITTER || !(wibble & 0x1F)))
	{
		dx = lara_item->pos.x_pos - item->pos.x_pos;
		dz = lara_item->pos.z_pos - item->pos.z_pos;

		if (dx < -16384 || dx > 16384 || dz < -16384 || dz > 16384)
			return;

		SPARKS sptr;
		sptr.On = 1;
		sptr.sR = 0;
		sptr.sG = 0;
		sptr.sB = 0;
		
		if (item->object_number == SMOKE_EMITTER_BLACK)
		{
			sptr.dR = 96;
			sptr.dG = 96;
			sptr.dB = 96;
		}
		else
		{
			sptr.dR = 64;
			sptr.dG = 64;
			sptr.dB = 64;
		}

		sptr.FadeToBlack = 16;
		sptr.ColFadeSpeed = (GetRandomControl() & 3) + 8;
		sptr.Life = (GetRandomControl() & 7) + 28;
		sptr.sLife = sptr.Life;

		if (item->object_number == SMOKE_EMITTER_BLACK)
			sptr.TransType = 3;
		else
			sptr.TransType = 2;

		sptr.x = (GetRandomControl() & 0x3F) + item->pos.x_pos - 32;
		sptr.y = (GetRandomControl() & 0x3F) + item->pos.y_pos - 32;
		sptr.z = (GetRandomControl() & 0x3F) + item->pos.z_pos - 32;
		sptr.Xvel = (GetRandomControl() & 0xFF) - 128;
		sptr.Yvel = -16 - (GetRandomControl() & 0xF);
		sptr.Zvel = (GetRandomControl() & 0xFF) - 128;
		sptr.Friction = 3;
		sptr.Flags = 538;

		if (rooms[item->room_number].flags & ROOM_OUTSIDE)
			sptr.Flags = 794;

		sptr.RotAng = GetRandomControl() & 0xFFF;

		if (GetRandomControl() & 1)
			sptr.RotAdd = -8 - (GetRandomControl() & 7);
		else
			sptr.RotAdd = (GetRandomControl() & 7) + 8;

		sptr.Scalar = 2;
		sptr.Gravity = -8 - (GetRandomControl() & 0xF);
		sptr.MaxYvel = -8 - (GetRandomControl() & 7);
		size = (GetRandomControl() & 0x1F) + 128;
		sptr.dSize = (unsigned char)size;
		sptr.sSize = (unsigned char)(size >> 2);
		sptr.Size = (unsigned char)(size >> 2);

		if (item->object_number == STEAM_EMITTER)
		{
			sptr.Gravity >>= 1;
			sptr.Yvel >>= 1;
			sptr.MaxYvel >>= 1;
			sptr.Life += 16;
			sptr.sLife += 16;
			sptr.dR = 32;
			sptr.dG = 32;
			sptr.dB = 32;
		}

		Sparks.push_back(sptr);
	}
}

void TriggerExplosionSmokeEnd(long x, long y, long z, long uw)
{
	long dx = lara_item->pos.x_pos - x;
	long dz = lara_item->pos.z_pos - z;
	if (dx < -16384 || dx > 16384 || dz < -16384 || dz > 16384)
		return;

	SPARKS sptr;
	sptr.On = 1;
	if (uw)
	{
		sptr.sR = 0;
		sptr.sG = 0;
		sptr.sB = 0;
		sptr.dR = 192;
		sptr.dG = 192;
		sptr.dB = 208;
	}
	else
	{
		sptr.sR = 196;
		sptr.sG = 196;
		sptr.sB = 196;
		sptr.dR = 96;
		sptr.dG = 96;
		sptr.dB = 96;
	}

	sptr.ColFadeSpeed = 8;
	sptr.FadeToBlack = 64;
	sptr.Life = (GetRandomControl() & 0x1F) + 96;
	sptr.sLife = sptr.Life;

	if (uw)
		sptr.TransType = 2;
	else
		sptr.TransType = 3;

	sptr.x = (GetRandomControl() & 0x1F) + x - 16;
	sptr.y = (GetRandomControl() & 0x1F) + y - 16;
	sptr.z = (GetRandomControl() & 0x1F) + z - 16;
	sptr.Xvel = ((GetRandomControl() & 0xFFF) - 2048) >> 2;
	sptr.Yvel = (GetRandomControl() & 0xFF) - 128;
	sptr.Zvel = ((GetRandomControl() & 0xFFF) - 2048) >> 2;

	if (uw)
	{
		sptr.Friction = 20;
		sptr.Yvel >>= 4;
		sptr.y += 32;
	}
	else
		sptr.Friction = 6;

	sptr.Flags = 538;
	sptr.RotAng = GetRandomControl() & 0xFFF;

	if (GetRandomControl() & 1)
		sptr.RotAdd = -16 - (GetRandomControl() & 0xF);
	else
		sptr.RotAdd = (GetRandomControl() & 0xF) + 16;

	sptr.Scalar = 3;

	if (uw)
	{
		sptr.MaxYvel = 0;
		sptr.Gravity = 0;
	}
	else
	{
		sptr.Gravity = -3 - (GetRandomControl() & 3);
		sptr.MaxYvel = -4 - (GetRandomControl() & 3);
	}

	sptr.dSize = (GetRandomControl() & 0x1F) + 128;
	sptr.sSize = sptr.dSize >> 2;
	sptr.Size = sptr.sSize;

	Sparks.push_back(sptr);
}

void TriggerExplosionSmoke(long x, long y, long z, long uw)
{
	long dx = lara_item->pos.x_pos - x;
	long dz = lara_item->pos.z_pos - z;
	if (dx < -16384 || dx > 16384 || dz < -16384 || dz > 16384)
		return;

	SPARKS sptr;
	sptr.On = 1;

	if (!uw)
	{
		sptr.sR = 196;
		sptr.sG = 196;
		sptr.sB = 196;
		sptr.dR = 128;
		sptr.dG = 128;
		sptr.dB = 128;
	}
	else
	{
		sptr.sR = 144;
		sptr.sG = 144;
		sptr.sB = 144;
		sptr.dR = 64;
		sptr.dG = 64;
		sptr.dB = 64;
	}

	sptr.ColFadeSpeed = 2;
	sptr.FadeToBlack = 8;
	sptr.TransType = 3;
	sptr.Life = (GetRandomControl() & 3) + 10;
	sptr.sLife = sptr.Life;
	sptr.x = (GetRandomControl() & 0x1FF) + x - 256;
	sptr.y = (GetRandomControl() & 0x1FF) + y - 256;
	sptr.z = (GetRandomControl() & 0x1FF) + z - 256;
	sptr.Xvel = ((GetRandomControl() & 0xFFF) - 2048) >> 2;
	sptr.Yvel = (GetRandomControl() & 0xFF) - 128;
	sptr.Zvel = ((GetRandomControl() & 0xFFF) - 2048) >> 2;

	if (uw)
		sptr.Friction = 2;
	else
		sptr.Friction = 6;

	sptr.Flags = 538;
	sptr.RotAng = GetRandomControl() & 0xFFF;
	sptr.Scalar = 1;
	sptr.RotAdd = (GetRandomControl() & 0xF) + 16;
	sptr.Gravity = -3 - (GetRandomControl() & 3);
	sptr.MaxYvel = -4 - (GetRandomControl() & 3);
	sptr.dSize = (GetRandomControl() & 0x1F) + 128;
	sptr.sSize = sptr.dSize >> 2;
	sptr.Size = sptr.sSize >> 2;

	Sparks.push_back(sptr);
}

void TriggerFlareSparks(long x, long y, long z, long xvel, long yvel, long zvel, long smoke)
{
	long dx = lara_item->pos.x_pos - x;
	long dz = lara_item->pos.z_pos - z;
	if (dx < -0x4000 || dx > 0x4000 || dz < -0x4000 || dz > 0x4000)
		return;

	SPARKS sptr;
	sptr.On = 1;
	sptr.dR = -1;
	sptr.dG = (GetRandomControl() & 0x7F) + 64;
	sptr.dB = -64 - sptr.dG;
	sptr.sR = -1;
	sptr.sG = -1;
	sptr.sB = -1;
	sptr.Life = 10;
	sptr.sLife = 10;
	sptr.ColFadeSpeed = 3;
	sptr.FadeToBlack = 5;
	sptr.x = (GetRandomControl() & 7) + x - 3;
	sptr.y = ((GetRandomControl() >> 3) & 7) + y - 3;
	sptr.z = ((GetRandomControl() >> 6) & 7) + z - 3;
	sptr.Xvel = short(((GetRandomControl() >> 2) & 0xFF) + xvel - 128);
	sptr.Yvel = short(((GetRandomControl() >> 4) & 0xFF) + yvel - 128);
	sptr.Zvel = short(((GetRandomControl() >> 6) & 0xFF) + zvel - 128);
	sptr.TransType = 2;
	sptr.Friction = 34;
	sptr.Scalar = 1;
	sptr.Flags = 2;
	sptr.sSize = ((GetRandomControl() >> 9) & 3) + 4;
	sptr.Size = sptr.sSize;
	sptr.dSize = ((GetRandomControl() >> 12) & 1) + 1;
	sptr.MaxYvel = 0;
	sptr.Gravity = 0;
	Sparks.push_back(sptr);

	if (smoke)
	{
		SPARKS smokeSpark;
		smokeSpark.On = 1;
		smokeSpark.sR = sptr.dR >> 1;
		smokeSpark.sG = sptr.dG >> 1;
		smokeSpark.sB = sptr.dB >> 1;
		smokeSpark.dR = 32;
		smokeSpark.dG = 32;
		smokeSpark.dB = 32;
		smokeSpark.FadeToBlack = 4;
		smokeSpark.TransType = 2;
		smokeSpark.ColFadeSpeed = (GetRandomControl() & 3) + 8;
		smokeSpark.Life = ((GetRandomControl() >> 3) & 7) + 13;
		smokeSpark.sLife = smokeSpark.Life;
		smokeSpark.Friction = 4;
		smokeSpark.x = x + (xvel >> 5);
		smokeSpark.y = y + (yvel >> 5);
		smokeSpark.z = z + (zvel >> 5);
		smokeSpark.Xvel = short((GetRandomControl() & 0x3F) + xvel - 32);
		smokeSpark.Yvel = (short)yvel;
		smokeSpark.Zvel = short(((GetRandomControl() >> 6) & 0x3F) + zvel - 32);

		if (GetRandomControl() & 1)
		{
			smokeSpark.Flags = 538;
			smokeSpark.RotAng = short(GetRandomControl() >> 3);

			if (GetRandomControl() & 2)
				smokeSpark.RotAdd = -16 - (GetRandomControl() & 0xF);
			else
				smokeSpark.RotAdd = (GetRandomControl() & 0xF) + 16;
		}
		else
			smokeSpark.Flags = 522;

		smokeSpark.Gravity = -8 - ((GetRandomControl() >> 3) & 3);
		smokeSpark.Scalar = 2;
		smokeSpark.MaxYvel = -4 - ((GetRandomControl() >> 6) & 3);
		smokeSpark.dSize = ((GetRandomControl() >> 8) & 0xF) + 24;
		smokeSpark.sSize = smokeSpark.dSize >> 3;
		smokeSpark.Size = smokeSpark.dSize >> 3;
		Sparks.push_back(smokeSpark);
	}
}

void TriggerDynamic(long x, long y, long z, long falloff, long r, long g, long b)
{
	if (!falloff)
		return;

	DYNAMIC dl;
	dl.on = 1;
	dl.x = x;
	dl.y = y;
	dl.z = z;
	dl.falloff = unsigned short(falloff << 8);

	if (falloff < 8)
	{
		dl.r = unsigned char((r * falloff) >> 3);
		dl.g = unsigned char((g * falloff) >> 3);
		dl.b = unsigned char((b * falloff) >> 3);
	}
	else
	{
		dl.r = (unsigned char)r;
		dl.g = (unsigned char)g;
		dl.b = (unsigned char)b;
	}

	dl.FalloffScale = 0x200000 / (falloff << 8);
	Dynamics.push_back(dl);
}

void TriggerDynamic_MIRROR(long x, long y, long z, long falloff, long r, long g, long b)
{
	for (int i = 0; i < 2; i++)
	{
		DYNAMIC dl;
		dl.on = 1;
		dl.x = x;
		dl.y = y;
		dl.z = z;
		dl.falloff = unsigned short(falloff << 8);

		if (falloff < 8)
		{
			dl.r = unsigned char((r * falloff) >> 3);
			dl.g = unsigned char((g * falloff) >> 3);
			dl.b = unsigned char((b * falloff) >> 3);
		}
		else
		{
			dl.r = (unsigned char)r;
			dl.g = (unsigned char)g;
			dl.b = (unsigned char)b;
		}

		dl.FalloffScale = 0x200000 / (falloff << 8);
		Dynamics.push_back(dl);

		z = 2 * gfMirrorZPlane - z;
	}
}

void ClearDynamics()
{
	Dynamics.clear();
}

void ControlEnemyMissile(short fx_number)
{
	FX_INFO* fx;
	FLOOR_INFO* floor;
	long speed, ox, oy, oz, h, c;
	short room_number, max_speed, max_turn;
	short angles[2];

	fx = &effects[fx_number];
	phd_GetVectorAngles(lara_item->pos.x_pos - fx->pos.x_pos, lara_item->pos.y_pos - fx->pos.y_pos - 256, lara_item->pos.z_pos - fx->pos.z_pos, angles);

	if (fx->flag1 == 1)
	{
		max_turn = 512;
		max_speed = 256;
	}
	else if (fx->flag1 == 6)
	{
		if (fx->counter)
			fx->counter--;

		max_turn = 768;
		max_speed = 192;
	}
	else
	{
		max_turn = 768;
		max_speed = 192;
	}

	if (fx->speed < max_speed)
	{
		if (fx->flag1 == 6)
			fx->speed++;
		else
			fx->speed += 3;

		oy = (unsigned short)angles[0] - (unsigned short)fx->pos.y_rot;

		if (abs(oy) > 0x8000)
			oy = (unsigned short)fx->pos.y_rot - (unsigned short)angles[0];

		ox = (unsigned short)angles[1] - (unsigned short)fx->pos.x_rot;

		if (abs(ox) > 0x8000)
			ox = (unsigned short)fx->pos.x_rot - (unsigned short)angles[1];

		oy >>= 3;
		ox >>= 3;

		if (oy > max_turn)
			oy = max_turn;
		else if (oy < -max_turn)
			oy = -max_turn;

		if (ox > max_turn)
			ox = max_turn;
		else if (ox < -max_turn)
			ox = -max_turn;

		fx->pos.x_rot += (short)ox;

		if (fx->flag1 != 4 && (fx->flag1 != 6 || !fx->counter))
			fx->pos.y_rot += (short)oy;
	}

	fx->pos.z_rot += fx->speed << 4;

	if (fx->flag1 == 6)
		fx->pos.z_rot += fx->speed << 4;

	ox = fx->pos.x_pos;
	oy = fx->pos.y_pos;
	oz = fx->pos.z_pos;
	speed = fx->speed * phd_cos(fx->pos.x_rot) >> W2V_SHIFT;
	fx->pos.x_pos += speed * phd_sin(fx->pos.y_rot) >> W2V_SHIFT;
	fx->pos.y_pos += fx->speed * phd_sin(-fx->pos.x_rot) >> W2V_SHIFT;
	fx->pos.z_pos += speed * phd_cos(fx->pos.y_rot) >> W2V_SHIFT;
	room_number = fx->room_number;
	floor = GetFloor(fx->pos.x_pos, fx->pos.y_pos, fx->pos.z_pos, &room_number);
	h = GetHeight(floor, fx->pos.x_pos, fx->pos.y_pos, fx->pos.z_pos);
	c = GetCeiling(floor, fx->pos.x_pos, fx->pos.y_pos, fx->pos.z_pos);

	if (fx->pos.y_pos >= h || fx->pos.y_pos <= c)
	{
		fx->pos.x_pos = ox;
		fx->pos.y_pos = oy;
		fx->pos.z_pos = oz;

		if (fx->flag1 != 6)
			ExplodeFX(fx, 0, -32);

		if (fx->flag1 == 1)
		{
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x18008040, (((~rooms[fx->room_number].flags & 0xFF) >> 4) & 2) << 16);	//decipher me
			TriggerExplosionSparks(ox, oy, oz, 3, -2, 2, fx->room_number);
		}
		else if (fx->flag1 == 0)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x10008040, 0);
		else if (fx->flag1 == 3 || fx->flag1 == 4)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x10004080, 0);
		else if (fx->flag1 == 5)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x10806000, 0);
		else if (fx->flag1 == 2)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x10808000, 0);
		else if (fx->flag1 == 6)
		{
			TriggerExplosionSparks(ox, oy, oz, 3, -2, 0, fx->room_number);
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xF00030, 64, 0x18806000, 0x20000);
			fx->pos.y_pos -= 128;
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xF00030, 48, 0x10807000, 0x20000);
			fx->pos.y_pos += 256;
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xF00030, 48, 0x10807000, 0x20000);
		}

		KillEffect(fx_number);
		return;
	}

	if (ItemNearLara(&fx->pos, 200))
	{
		lara_item->hit_status = 1;

		if (fx->flag1 != 6)
			ExplodeFX(fx, 0, -32);

		KillEffect(fx_number);

		if (fx->flag1 == 1)
		{
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xF00030, 64, 0x18008040, 0);
			TriggerExplosionSparks(ox, oy, oz, 3, -2, 2, fx->room_number);
			LaraBurn();
			lara.BurnGreen = 1;
		}
		else if (fx->flag1 == 0)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0x580018, 48, 0x10008040, (((~rooms[fx->room_number].flags & 0xFF) >> 4) & 2) << 16);
		else if (fx->flag1 == 3 || fx->flag1 == 4)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x10004080, 0x10000);
		else if (fx->flag1 == 5)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x10806000, 0x20000);
		else if (fx->flag1 == 2)
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xA00020, 64, 0x10808000, 0x20000);
		else if (fx->flag1 == 6)
		{
			TriggerExplosionSparks(ox, oy, oz, 3, -2, 0, fx->room_number);
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xF00030, 64, 0x18806000, 0);
			fx->pos.y_pos -= 128;
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xF00030, 48, 0x10807000, 0);
			fx->pos.y_pos += 256;
			TriggerShockwave((PHD_VECTOR*)&fx->pos, 0xF00030, 48, 0x10807000, 0);
			LaraBurn();
		}
	}
	else
	{
		if (room_number != fx->room_number)
			EffectNewRoom(fx_number, room_number);

		ox -= fx->pos.x_pos;
		oy -= fx->pos.y_pos;
		oz -= fx->pos.z_pos;

		if (wibble & 4 || fx->flag1 == 1 || fx->flag1 == 5 || fx->flag1 == 2)
		{
			if (fx->flag1 == 0)
				TriggerSethMissileFlame(fx_number, ox << 4, oy << 4, oz << 4);
			else if (fx->flag1 == 1)
				TriggerSethMissileFlame(fx_number, ox << 5, oy << 5, oz << 5);
			else if (fx->flag1 == 3 || fx->flag1 == 4 || fx->flag1 == 5)
				TriggerDemigodMissileFlame(fx_number, ox << 4, oy << 4, oz << 4);
			else if (fx->flag1 == 2)
				TriggerHarpyMissileFlame(fx_number, ox << 4, oy << 4, oz << 4);
			else if (fx->flag1 == 6)
				TriggerCrocgodMissileFlame(fx_number, ox << 4, oy << 4, oz << 4);
		}
	}
}

void SetupRipple(long x, long y, long z, long size, long flags)
{
	RIPPLE_STRUCT* ripple;
	long num;

	num = 0;

	while (ripples[num].flags & 1)
	{
		num++;

		if (num >= 16)
			return;
	}

	ripple = &ripples[num];
	ripple->flags = (char)flags | 1;
	ripple->size = (unsigned char)size;
	ripple->life = (GetRandomControl() & 0xF) + 48;
	ripple->init = 1;
	ripple->x = x;
	ripple->y = y;
	ripple->z = z;

	if (flags & 0x40)
	{
		ripple->x += (GetRandomControl() & 0x7F) - 64;
		ripple->z += (GetRandomControl() & 0x7F) - 64;
	}
}

void TriggerUnderwaterBlood(long x, long y, long z, long size)
{
	RIPPLE_STRUCT* ripple;
	long n;

	ripple = ripples;
	n = 0;

	while (ripple->flags & 1)
	{
		ripple++;
		n++;

		if (n >= 16)
			return;
	}

	ripple->flags = 49;
	ripple->init = 1;
	ripple->life = (GetRandomControl() & 7) - 16;
	ripple->size = (unsigned char)size;
	ripple->x = x + (GetRandomControl() & 0x3F) - 32;
	ripple->y = y;
	ripple->z = z + (GetRandomControl() & 0x3F) - 32;
}

static long waterfallmist_offsets[4]{ 576,203,-203,-576 };
void TriggerWaterfallMist(long x, long y, long z, long ang)
{
	long ang2 = (ang + 1024) & 0xFFF;
	long ps = rcossin_tbl[ang2 << 2];
	long pc = rcossin_tbl[(ang2 << 2) + 1];
	long vs = rcossin_tbl[ang << 2];
	long vc = rcossin_tbl[(ang << 2) + 1];

	for (int i = 0; i < 4; i++)
	{
		SPARKS sptr;
		sptr.On = 1;
		sptr.sR = 128;
		sptr.sG = 128;
		sptr.sB = 128;
		sptr.dR = 192;
		sptr.dG = 192;
		sptr.dB = 192;
		sptr.ColFadeSpeed = 2;
		sptr.FadeToBlack = 4;
		sptr.TransType = 2;
		sptr.Life = (GetRandomControl() & 3) + 6;
		sptr.sLife = sptr.Life;
		long rad = (GetRandomControl() & 0x1F) + waterfallmist_offsets[i] - 16;
		sptr.x = ((rad * ps) >> 12) + x + (GetRandomControl() & 0xF) - 8;
		sptr.y = (GetRandomControl() & 0xF) + y - 8;
		sptr.z = ((rad * pc) >> 12) + z + (GetRandomControl() & 0xF) - 8;
		sptr.Xvel = vs >> 12;
		sptr.Yvel = 0;
		sptr.Zvel = vc >> 12;
		sptr.Friction = 3;
		if (GetRandomControl() & 1)
		{
			sptr.Flags = 538;
			sptr.RotAng = GetRandomControl() & 0xFFF;

			if (GetRandomControl() & 1)
				sptr.RotAdd = -16 - (GetRandomControl() & 0xF);
			else
				sptr.RotAdd = (GetRandomControl() & 0xF) + 16;
		}
		else
			sptr.Flags = 522;

		sptr.Scalar = 6;
		sptr.Gravity = 0;
		sptr.MaxYvel = 0;
		sptr.dSize = (GetRandomControl() & 7) + 12;
		sptr.sSize = sptr.dSize >> 1;
		sptr.Size = sptr.dSize >> 1;
		Sparks.push_back(sptr);
	}
}

void TriggerDartSmoke(long x, long y, long z, long xv, long zv, long hit)
{
	long dx = lara_item->pos.x_pos - x;
	long dz = lara_item->pos.z_pos - z;
	if (dx < -0x4000 || dx > 0x4000 || dz < -0x4000 || dz > 0x4000)
		return;

	SPARKS sptr;
	sptr.On = 1;
	sptr.sR = 16;
	sptr.sG = 8;
	sptr.sB = 4;
	sptr.dR = 64;
	sptr.dG = 48;
	sptr.dB = 32;
	sptr.ColFadeSpeed = 8;
	sptr.FadeToBlack = 4;
	sptr.Life = (GetRandomControl() & 3) + 32;
	sptr.sLife = sptr.Life;
	sptr.TransType = 2;
	sptr.x = (GetRandomControl() & 0x1F) + x - 16;
	sptr.y = (GetRandomControl() & 0x1F) + y - 16;
	sptr.z = (GetRandomControl() & 0x1F) + z - 16;

	if (hit)
	{
		sptr.Xvel = short((GetRandomControl() & 0xFF) - xv - 128);
		sptr.Yvel = -4 - (GetRandomControl() & 3);
		sptr.Zvel = short((GetRandomControl() & 0xFF) - zv - 128);
	}
	else
	{
		if (xv)
			sptr.Xvel = (short)-xv;
		else
			sptr.Xvel = (GetRandomControl() & 0xFF) - 128;

		sptr.Yvel = -4 - (GetRandomControl() & 3);

		if (zv)
			sptr.Zvel = (short)-zv;
		else
			sptr.Zvel = (GetRandomControl() & 0xFF) - 128;
	}

	sptr.Friction = 3;

	if (GetRandomControl() & 1)
	{
		sptr.Flags = 538;
		sptr.RotAng = GetRandomControl() & 0xFFF;

		if (GetRandomControl() & 1)
			sptr.RotAdd = -16 - (GetRandomControl() & 0xF);
		else
			sptr.RotAdd = (GetRandomControl() & 0xF) + 16;
	}
	else
		sptr.Flags = 522;

	sptr.Scalar = 1;

	unsigned char rand = (GetRandomControl() & 0x3F) + 72;
	if (hit)
	{
		sptr.MaxYvel = 0;
		sptr.Gravity = 0;
		sptr.Size = unsigned char(rand >> 3);
		sptr.sSize = sptr.Size;
		sptr.dSize = unsigned char(rand >> 1);
	}
	else
	{
		sptr.MaxYvel = -4 - (GetRandomControl() & 3);
		sptr.Gravity = -4 - (GetRandomControl() & 3);
		sptr.Size = unsigned char(rand >> 4);
		sptr.sSize = sptr.Size;
		sptr.dSize = (unsigned char)rand;
	}

	Sparks.push_back(sptr);
}

void KillAllCurrentItems(short item_number)
{
	KillEverythingFlag = 1;
}

void KillEverything()
{
	KillEverythingFlag = 0;
}

void TriggerExplosionBubble(long x, long y, long z, short room_number)
{
	long dx = lara_item->pos.x_pos - x;
	long dz = lara_item->pos.z_pos - z;
	if (dx < -0x4000 || dx > 0x4000 || dz < -0x4000 || dz > 0x4000)
		return;

	SPARKS sptr;
	sptr.On = 1;
	sptr.sR = 128;
	sptr.sG = 64;
	sptr.sB = 0;
	sptr.dR = 128;
	sptr.dG = 128;
	sptr.dB = 128;
	sptr.ColFadeSpeed = 8;
	sptr.FadeToBlack = 12;
	sptr.Life = 24;
	sptr.sLife = 24;
	sptr.TransType = 2;
	sptr.x = x;
	sptr.y = y;
	sptr.z = z;
	sptr.Xvel = 0;
	sptr.Yvel = 0;
	sptr.Zvel = 0;
	sptr.Friction = 0;
	sptr.Flags = 2058;
	sptr.Scalar = 3;
	sptr.Def = objects[DEFAULT_SPRITES].mesh_index + 13;
	sptr.Gravity = 0;
	sptr.MaxYvel = 0;

	unsigned char size = (GetRandomControl() & 7) + 63;
	sptr.Size = size >> 1;
	sptr.sSize = size >> 1;
	sptr.dSize = size << 1;
	Sparks.push_back(sptr);

	for (int i = 0; i < 7; i++)
	{
		PHD_3DPOS pos;
		pos.x_pos = (GetRandomControl() & 0x1FF) + x - 256;
		pos.y_pos = (GetRandomControl() & 0x7F) + y - 64;
		pos.z_pos = (GetRandomControl() & 0x1FF) + z - 256;
		CreateBubble(&pos, room_number, 6, 15);
	}
}

void ControlColouredLights(short item_number)
{
	static unsigned char colours[5][3] =
	{
		{ 255, 0, 0 },		//RED_LIGHT
		{ 0, 255, 0 },		//GREEN_LIGHT
		{ 0, 0, 255 },		//BLUE_LIGHT
		{ 255, 192, 0 },	//AMBER_LIGHT 
		{ 224, 224, 255 }	//WHITE_LIGHT
		                    //BLINKING_LIGHT
	};

	ITEM_INFO* item = &items[item_number];
	if (TriggerActive(item))
	{
		long objnum = item->object_number - RED_LIGHT;
		TriggerDynamic(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, 24, colours[objnum][0], colours[objnum][1], colours[objnum][2]);
	}
}

void DetatchSpark(long num, long type)
{
	if (Sparks.empty())
		return;

	for (auto& sptr : Sparks)
	{
		if (sptr.On && (sptr.Flags & type) && sptr.FxObj == num)
		{
			if (type == 64)
			{
				FX_INFO* fx = &effects[num];
				sptr.x += fx->pos.x_pos;
				sptr.y += fx->pos.y_pos;
				sptr.z += fx->pos.z_pos;
				sptr.Flags &= ~64;
			}
			else if (type == 128)
			{
				ITEM_INFO* item = &items[num];
				sptr.x += item->pos.x_pos;
				sptr.y += item->pos.y_pos;
				sptr.z += item->pos.z_pos;
				sptr.Flags &= ~128;
			}
		}
	}
}

void UpdateSparks()
{
	if (Sparks.empty())
		return;

	short* bounds;
	long fade, uw, rad, rnd, x, y, z, r, g, b, falloff;

	bounds = GetBoundsAccurate(lara_item);
	DeadlyBounds[0] = lara_item->pos.x_pos + bounds[0];
	DeadlyBounds[1] = lara_item->pos.x_pos + bounds[1];
	DeadlyBounds[2] = lara_item->pos.y_pos + bounds[2];
	DeadlyBounds[3] = lara_item->pos.y_pos + bounds[3];
	DeadlyBounds[4] = lara_item->pos.z_pos + bounds[4];
	DeadlyBounds[5] = lara_item->pos.z_pos + bounds[5];

	for (auto& spark : Sparks)
	{
		if (!spark.On)
			continue;

		spark.Life--;
		if (!spark.Life)
		{
			spark.On = 0;
			continue;
		}

		if (spark.sLife - spark.Life < spark.ColFadeSpeed)
		{
			fade = ((spark.sLife - spark.Life) << 16) / spark.ColFadeSpeed;
			spark.R = unsigned char(spark.sR + ((fade * (spark.dR - spark.sR)) >> 16));
			spark.G = unsigned char(spark.sG + ((fade * (spark.dG - spark.sG)) >> 16));
			spark.B = unsigned char(spark.sB + ((fade * (spark.dB - spark.sB)) >> 16));
		}
		else if (spark.Life < spark.FadeToBlack)
		{
			fade = ((spark.Life - spark.FadeToBlack) << 16) / spark.FadeToBlack + 0x10000;
			spark.R = unsigned char((spark.dR * fade) >> 16);
			spark.G = unsigned char((spark.dG * fade) >> 16);
			spark.B = unsigned char((spark.dB * fade) >> 16);

			if (spark.R < 8 && spark.G < 8 && spark.B < 8)
			{
				spark.On = 0;
				continue;
			}
		}
		else
		{
			spark.R = spark.dR;
			spark.G = spark.dG;
			spark.B = spark.dB;
		}

		if (spark.Life == spark.FadeToBlack && spark.Flags & 0x800)
			spark.dSize >>= 2;

		if (spark.Flags & 0x10)
			spark.RotAng = (spark.RotAng + spark.RotAdd) & 0xFFF;

		if (spark.sLife - spark.Life == spark.extras >> 3 && spark.extras & 7)
		{
			if (spark.Flags & 0x800)
				uw = 1;
			else if (spark.Flags & 0x2000)
				uw = 2;
			else
				uw = 0;

			for (int j = 0; j < (spark.extras & 7); j++)
			{
				TriggerExplosionSparks(spark.x, spark.y, spark.z, (spark.extras & 7) - 1, spark.Dynamic, uw, spark.RoomNumber);
				spark.Dynamic = -1;
			}

			if (spark.Flags & 0x800)
				TriggerExplosionBubble(spark.x, spark.y, spark.z, spark.RoomNumber);

			spark.extras = 0;
		}

		fade = ((spark.sLife - spark.Life) << 16) / spark.sLife;
		spark.Yvel += spark.Gravity;

		if (spark.MaxYvel)
		{
			if (spark.Yvel < 0 && spark.Yvel < spark.MaxYvel << 5 || spark.Yvel > 0 && spark.Yvel > spark.MaxYvel << 5)
				spark.Yvel = spark.MaxYvel << 5;
		}

		if (spark.Friction & 0xF)
		{
			spark.Xvel -= spark.Xvel >> (spark.Friction & 0xF);
			spark.Zvel -= spark.Zvel >> (spark.Friction & 0xF);
		}

		if (spark.Friction & 0xF0)
			spark.Yvel -= spark.Yvel >> (spark.Friction >> 4);

		spark.x += spark.Xvel >> 5;
		spark.y += spark.Yvel >> 5;
		spark.z += spark.Zvel >> 5;

		if (spark.Flags & 0x100)
		{
			spark.x += SmokeWindX >> 1;
			spark.z += SmokeWindZ >> 1;
		}

		spark.Size = unsigned char(spark.sSize + ((fade * (spark.dSize - spark.sSize)) >> 16));

		if (spark.Flags & 1 && !lara.burn || spark.Flags & 0x400)
		{
			rad = spark.Size << spark.Scalar >> 1;

			if (spark.x + rad > DeadlyBounds[0] && spark.x - rad < DeadlyBounds[1] &&
				spark.y + rad > DeadlyBounds[2] && spark.y - rad < DeadlyBounds[3] &&
				spark.z + rad > DeadlyBounds[4] && spark.z - rad < DeadlyBounds[5])
			{
				if (spark.Flags & 1)
					LaraBurn();
				else
					lara_item->hit_points -= 2;
			}
		}
	}

	Sparks.erase(
		std::remove_if(
			Sparks.begin(),
			Sparks.end(),
			[](SPARKS& spr) {
				return spr.On == 0;
			}
		),
		Sparks.end()
	);

	for (auto& spark : Sparks)
	{
		if (spark.On == 0 || spark.Dynamic == -1)
			continue;
		for (auto& dyn : Dynamics)
		{
			if (dyn.flags & 3)
			{
				rnd = GetRandomControl();
				x = spark.x + 16 * (rnd & 0xF);
				y = spark.y + (rnd & 0xF0);
				z = spark.z + ((rnd >> 4) & 0xF0);
				falloff = spark.sLife - spark.Life - 1;

				if (falloff < 2)
				{
					if (dyn.falloff < 28)
						dyn.falloff += 6;

					r = 255 - (falloff << 3) - (rnd & 0x1F);
					g = 255 - (falloff << 4) - (rnd & 0x1F);
					b = 255 - (falloff << 6) - (rnd & 0x1F);
				}
				else if (falloff < 4)
				{
					if (dyn.falloff < 28)
						dyn.falloff += 6;

					r = 255 - (falloff << 3) - (rnd & 0x1F);
					g = 128 - (falloff << 3);
					b = 128 - (falloff << 5);

					if (b < 0)
						b = 0;
				}
				else
				{
					if (dyn.falloff > 0)
						dyn.falloff--;

					r = 224 + (rnd & 0x1F);
					g = 128 + ((rnd >> 4) & 0x1F);
					b = (rnd >> 8) & 0x3F;
				}

				if (spark.Flags & 0x2000)
					TriggerDynamic(x, y, z, dyn.falloff > 31 ? 31 : dyn.falloff, b, r, g);
				else
					TriggerDynamic(x, y, z, dyn.falloff > 31 ? 31 : dyn.falloff, r, g, b);
			}
		}
	}
}

void TriggerRicochetSpark(GAME_VECTOR* pos, long ang, long num, long smoke_only)
{
	if (!smoke_only)
	{
		for (int i = 0; i < num; i++)
		{
			SPARKS sptr;
			sptr.On = 1;
			sptr.sR = 128;
			sptr.sG = (GetRandomControl() & 0xF) + 16;
			sptr.sB = 0;
			sptr.dR = 96;
			sptr.dG = ((GetRandomControl() >> 4) & 0x1F) + 48;
			sptr.dB = 0;
			sptr.ColFadeSpeed = 2;
			sptr.FadeToBlack = 4;
			sptr.Life = 9;
			sptr.sLife = 9;
			sptr.TransType = 2;
			sptr.x = pos->x;
			sptr.y = pos->y;
			sptr.z = pos->z;
			sptr.Yvel = (GetRandomControl() & 0xFFF) - 2048;
			sptr.Gravity = (GetRandomControl() >> 7) & 0x1F;
			int rnd = (((GetRandomControl() >> 3) & 0x7FF) + ang - 1024) & 0xFFF;
			sptr.Zvel = rcossin_tbl[(rnd << 1) + 1] >> 2;
			sptr.Xvel = -rcossin_tbl[rnd << 1] >> 2;
			sptr.Friction = 34;
			sptr.Flags = 0;
			sptr.MaxYvel = 0;
			Sparks.push_back(sptr);
		}

		SPARKS sptr;
		sptr.On = 1;
		sptr.sR = 48;
		sptr.sG = (GetRandomControl() & 0xF) + 32;
		sptr.sB = 0;
		sptr.dR = 0;
		sptr.dG = 0;
		sptr.dB = 0;
		sptr.ColFadeSpeed = 4;
		sptr.FadeToBlack = 0;
		sptr.Life = 4;
		sptr.sLife = 4;
		sptr.TransType = 2;
		sptr.x = pos->x;
		sptr.y = pos->y;
		sptr.z = pos->z;
		sptr.Xvel = 0;
		sptr.Yvel = 0;
		sptr.Zvel = 0;
		sptr.Flags = 26;
		sptr.RotAng = (GetRandomControl() >> 2) & 0xFFF;

		if (GetRandomControl() & 1)
			sptr.RotAdd = -64 - ((GetRandomControl() >> 1) & 0x3F);
		else
			sptr.RotAdd = ((GetRandomControl() >> 1) & 0x3F) + 64;

		sptr.Scalar = 3;
		sptr.Def = objects[DEFAULT_SPRITES].mesh_index + 12;
		sptr.Size = ((GetRandomControl() >> 10) & 7) + 8;
		sptr.sSize = sptr.Size;
		sptr.dSize = 1;
		sptr.MaxYvel = 0;
		sptr.Gravity = 0;
		Sparks.push_back(sptr);
	}

	for (int i = 0; i < 1 - smoke_only; i++)
	{
		SPARKS sptr;
		sptr.On = 1;
		sptr.sR = 0;
		sptr.sG = 0;
		sptr.sB = 0;
		sptr.dR = 40;
		sptr.dG = 40;
		sptr.dB = 48;
		sptr.ColFadeSpeed = (GetRandomControl() & 3) + 4;
		sptr.FadeToBlack = 8;
		sptr.Life = ((GetRandomControl() >> 2) & 7) + 16;
		sptr.sLife = sptr.Life;
		sptr.x = pos->x;
		sptr.y = pos->y;
		sptr.z = pos->z;

		if (smoke_only)
		{
			sptr.ColFadeSpeed >>= 1;
			sptr.FadeToBlack = 4;
			sptr.Life >>= 1;
			sptr.sLife >>= 1;
			sptr.Xvel = (GetRandomControl() & 0x1FF) - 256;
			sptr.Yvel = ((GetRandomControl() >> 2) & 0x1FF) - 256;
			sptr.Zvel = ((GetRandomControl() >> 4) & 0x1FF) - 256;
		}
		else
		{
			sptr.Yvel = 0;
			sptr.Xvel = 0;
			sptr.Zvel = 0;
		}

		sptr.TransType = 2;
		sptr.Friction = 0;
		sptr.Flags = 26;
		sptr.RotAng = short(GetRandomControl() >> 3);

		if (GetRandomControl() & 1)
			sptr.RotAdd = -16 - (GetRandomControl() & 0xF);
		else
			sptr.RotAdd = (GetRandomControl() & 0xF) + 16;

		sptr.Scalar = 2;
		sptr.Gravity = -4 - ((GetRandomControl() >> 9) & 3);
		sptr.MaxYvel = -4 - ((GetRandomControl() >> 6) & 3);
		sptr.Size = ((GetRandomControl() >> 5) & 7) + 4;
		sptr.sSize = sptr.Size;
		sptr.dSize = sptr.Size << 2;
		Sparks.push_back(sptr);
	}
}

void TriggerRocketFlameTR3(long x, long y, long z, long xv, long yv, long zv, long item_no)
{
	SPARKS sptr;
	sptr.On = 1;
	sptr.sR = 48 + (GetRandomControl() & 31);
	sptr.sG = sptr.sR;
	sptr.sB = 192 + (GetRandomControl() & 63);
	sptr.dR = 192 + (GetRandomControl() & 63);
	sptr.dG = 128 + (GetRandomControl() & 63);
	sptr.dB = 32;
	sptr.ColFadeSpeed = 12 + (GetRandomControl() & 3);
	sptr.FadeToBlack = 12;
	sptr.sLife = sptr.Life = (GetRandomControl() & 3) + 28;
	sptr.TransType = COLADD;
	sptr.extras = 0;
	sptr.Dynamic = -1;

	sptr.x = x + ((GetRandomControl() & 31) - 16);
	sptr.y = y;
	sptr.z = z + ((GetRandomControl() & 31) - 16);

	sptr.Xvel = xv;
	sptr.Yvel = yv;
	sptr.Zvel = zv;
	sptr.Friction = 3 | (3 << 4);

	if (GetRandomControl() & 1)
	{
		sptr.Flags = SP_SCALE | SP_DEF | SP_ROTATE | SP_ITEM | SP_EXPDEF;
		sptr.FxObj = item_no;
		sptr.RotAng = GetRandomControl() & 4095;
		if (GetRandomControl() & 1)
			sptr.RotAdd = -(GetRandomControl() & 15) - 16;
		else
			sptr.RotAdd = (GetRandomControl() & 15) + 16;
	}
	else
	{
		sptr.Flags = SP_SCALE | SP_DEF | SP_ITEM | SP_EXPDEF;
		sptr.FxObj = item_no;
	}

	sptr.Gravity = 0;
	sptr.MaxYvel = 0;
	sptr.Def = objects[DEFAULT_SPRITES].mesh_index;
	sptr.Scalar = 2;
	long size = (GetRandomControl() & 7) + 32;
	sptr.Size = sptr.sSize = size;
	sptr.dSize = 2;
	Sparks.push_back(sptr);
}

void TriggerRocketSmokeTR3(long x, long y, long z, long body_part)
{
	SPARKS sptr;
	sptr.On = 1;
	sptr.sR = 0;
	sptr.sG = 0;
	sptr.sB = 0;
	sptr.dR = 64 + body_part;
	sptr.dG = 64 + body_part;
	sptr.dB = 64 + body_part;

	sptr.ColFadeSpeed = 4 + (GetRandomControl() & 3);
	sptr.FadeToBlack = 12;
	sptr.sLife = sptr.Life = (GetRandomControl() & 3) + 20;
	sptr.TransType = COLADD;
	sptr.extras = 0;
	sptr.Dynamic = -1;
	sptr.x = x + ((GetRandomControl() & 15) - 8);
	sptr.y = y + ((GetRandomControl() & 15) - 8);
	sptr.z = z + ((GetRandomControl() & 15) - 8);
	sptr.Xvel = ((GetRandomControl() & 255) - 128);
	sptr.Yvel = -(GetRandomControl() & 3) - 4;
	sptr.Zvel = ((GetRandomControl() & 255) - 128);
	sptr.Friction = 4;

	if (GetRandomControl() & 1)
	{
		sptr.Flags = SP_SCALE | SP_DEF | SP_ROTATE | SP_EXPDEF;
		sptr.RotAng = GetRandomControl() & 4095;
		if (GetRandomControl() & 1)
			sptr.RotAdd = -(GetRandomControl() & 15) - 16;
		else
			sptr.RotAdd = (GetRandomControl() & 15) + 16;
	}
	else
		sptr.Flags = SP_SCALE | SP_DEF | SP_EXPDEF;

	sptr.Def = objects[DEFAULT_SPRITES].mesh_index;
	sptr.Scalar = 3;
	sptr.Gravity = -(GetRandomControl() & 3) - 4;
	sptr.MaxYvel = -(GetRandomControl() & 3) - 4;
	long size = (GetRandomControl() & 7) + 32;
	sptr.Size = sptr.sSize = size >> 2;
	sptr.dSize = size;
	Sparks.push_back(sptr);
}

static unsigned char explosion_extra_tables[4]{ 0, 4, 7, 10 };
void TriggerExplosionSparks(long x, long y, long z, long extras, long dynamic, long uw, short room_number)
{
	long dx = lara_item->pos.x_pos - x;
	long dz = lara_item->pos.z_pos - z;
	long scalar = 0;
	long mirror = 0;
	if (dx < -0x4000 || dx > 0x4000 || dz < -0x4000 || dz > 0x4000)
		return;

	if (room_number < 0)
	{
		room_number = -room_number;
		scalar = 1;
	}

	if (room_number == gfMirrorRoom && gfLevelFlags & GF_MIRROR)
		mirror = 1;

	unsigned char r, g, b;
	do
	{
		SPARKS sptr;
		sptr.On = TRUE;
		sptr.sR = 255;

		if (uw == 1)
		{
			sptr.sG = (GetRandomControl() & 0x3F) + 128;
			sptr.sB = 32;
			sptr.dR = 192;
			sptr.dG = (GetRandomControl() & 0x1F) + 64;
			sptr.dB = 0;
			sptr.ColFadeSpeed = 7;
			sptr.FadeToBlack = 8;
			sptr.TransType = COLADD;
			sptr.Life = (GetRandomControl() & 7) + 16;
			sptr.sLife = sptr.Life;
			sptr.RoomNumber = (unsigned char)room_number;
		}
		else
		{
			sptr.sG = (GetRandomControl() & 0xF) + 32;
			sptr.sB = 0;
			sptr.dR = (GetRandomControl() & 0x3F) + 192;
			sptr.dG = (GetRandomControl() & 0x3F) + 128;
			sptr.dB = 32;
			sptr.ColFadeSpeed = 8;
			sptr.FadeToBlack = 16;
			sptr.TransType = COLADD;
			sptr.Life = (GetRandomControl() & 7) + 24;
			sptr.sLife = sptr.Life;
		}

		sptr.extras = unsigned char(extras | ((explosion_extra_tables[extras] + (GetRandomControl() & 7) + 28) << 3));
		sptr.Dynamic = (char)dynamic;

		if (dynamic == -2)
		{
			DYNAMIC dyn;
			dyn.on = 1;
			dyn.falloff = 4;
			if (uw == 1)
				dyn.flags = 2;
			else
				dyn.flags = 1;
			Dynamics.push_back(dyn);
			sptr.Dynamic = Dynamics.size()-1;
		}

		sptr.x = (GetRandomControl() & 0x1F) + x - 16;
		sptr.y = (GetRandomControl() & 0x1F) + y - 16;
		sptr.z = (GetRandomControl() & 0x1F) + z - 16;
		sptr.Xvel = (GetRandomControl() & 0xFFF) - 2048;
		sptr.Yvel = (GetRandomControl() & 0xFFF) - 2048;
		sptr.Zvel = (GetRandomControl() & 0xFFF) - 2048;

		if (dynamic != -2 || uw == 1)
		{
			sptr.x = (GetRandomControl() & 0x1F) + x - 16;
			sptr.y = (GetRandomControl() & 0x1F) + y - 16;
			sptr.z = (GetRandomControl() & 0x1F) + z - 16;
		}
		else
		{
			sptr.x = (GetRandomControl() & 0x1FF) + x - 256;
			sptr.y = (GetRandomControl() & 0x1FF) + y - 256;
			sptr.z = (GetRandomControl() & 0x1FF) + z - 256;
		}

		if (uw == 1)
			sptr.Friction = 17;
		else
			sptr.Friction = 51;

		if (GetRandomControl() & 1)
		{
			if (uw == 1)
				sptr.Flags = 2586;
			else
				sptr.Flags = 538;

			sptr.RotAng = GetRandomControl() & 0xFFF;
			sptr.RotAdd = (GetRandomControl() & 0xFF) + 128;
		}
		else if (uw == 1)
			sptr.Flags = 2570;
		else
			sptr.Flags = 522;

		sptr.Scalar = 3;
		sptr.Gravity = 0;
		sptr.Size = (GetRandomControl() & 0xF) + 40;
		sptr.sSize = sptr.Size << scalar;
		sptr.dSize = sptr.Size << (scalar + 1);
		sptr.Size <<= scalar;
		GetRandomControl();
		sptr.MaxYvel = 0;

		if (uw == 2)
		{
			r = sptr.sR;
			g = sptr.sG;
			b = sptr.sB;
			sptr.sR = b;
			sptr.sG = r;
			sptr.sB = g;

			r = sptr.dR;
			g = sptr.dG;
			b = sptr.dB;
			sptr.dR = b;
			sptr.dG = r;
			sptr.dB = g;

			sptr.Flags |= 0x2000;
		}
		else if (extras)
			TriggerExplosionSmoke(x, y, z, uw);
		else
			TriggerExplosionSmokeEnd(x, y, z, uw);

		Sparks.push_back(sptr);
		z = 2 * gfMirrorZPlane - z;
		mirror--;
	}
	while (mirror >= 0);
}

void TriggerFireFlame(long x, long y, long z, long body_part, long type)
{
	long dx = lara_item->pos.x_pos - x;
	long dz = lara_item->pos.z_pos - z;
	if (dx < -0x4000 || dx > 0x4000 || dz < -0x4000 || dz > 0x4000)
		return;

	SPARKS sptr;
	sptr.On = 1;

	if (type == 2)
	{
		sptr.sR = (GetRandomControl() & 0x1F) + 48;
		sptr.sG = sptr.sR;
		sptr.sB = (GetRandomControl() & 0x3F) + 192;
	}
	else if (type == 254)
	{
		sptr.sR = 48;
		sptr.sG = 255;
		sptr.sB = (GetRandomControl() & 0x1F) + 48;
		sptr.dR = 32;
		sptr.dG = (GetRandomControl() & 0x3F) + 192;
		sptr.dB = (GetRandomControl() & 0x3F) + 128;
	}
	else
	{
		sptr.sR = 255;
		sptr.sG = (GetRandomControl() & 0x1F) + 48;
		sptr.sB = 48;
	}

	if (type != 254)
	{
		sptr.dR = (GetRandomControl() & 0x3F) + 192;
		sptr.dG = (GetRandomControl() & 0x3F) + 128;
		sptr.dB = 32;
	}

	if (body_part == -1)
	{
		sptr.FadeToBlack = 16;
		sptr.ColFadeSpeed = (GetRandomControl() & 3) + 8;
		sptr.Life = (GetRandomControl() & 3) + 28;
	}
	else if (type == 2 || type == 255 || type == 254)
	{
		sptr.FadeToBlack = 6;
		sptr.ColFadeSpeed = (GetRandomControl() & 3) + 5;
		sptr.Life = (type < 254 ? 0 : 8) + (GetRandomControl() & 3) + 16;
	}
	else
	{
		sptr.FadeToBlack = 8;
		sptr.ColFadeSpeed = (GetRandomControl() & 3) + 20;
		sptr.Life = (GetRandomControl() & 7) + 40;
	}

	sptr.sLife = sptr.Life;
	sptr.TransType = 2;

	if (body_part != -1)
	{
		sptr.x = (GetRandomControl() & 0x1F) - 16;
		sptr.y = 0;
		sptr.z = (GetRandomControl() & 0x1F) - 16;
	}
	else if (!type || type == 1)
	{
		sptr.x = (GetRandomControl() & 0x1F) + x - 16;
		sptr.y = y;
		sptr.z = (GetRandomControl() & 0x1F) + z - 16;
	}
	else if (type < 254)
	{
		sptr.x = (GetRandomControl() & 0xF) + x - 8;
		sptr.y = y;
		sptr.z = (GetRandomControl() & 0xF) + z - 8;
	}
	else
	{
		sptr.x = (GetRandomControl() & 0x3F) + x - 32;
		sptr.y = y;
		sptr.z = (GetRandomControl() & 0x3F) + z - 32;
	}

	if (type == 2)
	{
		sptr.Xvel = (GetRandomControl() & 0x1F) - 16;
		sptr.Yvel = -1024 - (GetRandomControl() & 0x1FF);
		sptr.Zvel = (GetRandomControl() & 0x1F) - 16;
		sptr.Friction = 68;
	}
	else
	{
		sptr.Xvel = (GetRandomControl() & 0xFF) - 128;
		sptr.Yvel = -16 - (GetRandomControl() & 0xF);
		sptr.Zvel = (GetRandomControl() & 0xFF) - 128;

		if (type == 1)
			sptr.Friction = 51;
		else
			sptr.Friction = 5;
	}

	if (GetRandomControl() & 1)
	{
		if (body_part == -1)
		{
			sptr.Gravity = -16 - (GetRandomControl() & 0x1F);
			sptr.Flags = 538;
			sptr.MaxYvel = -16 - (GetRandomControl() & 7);
		}
		else
		{
			sptr.Flags = 602;
			sptr.FxObj = (unsigned char)body_part;
			sptr.Gravity = -32 - (GetRandomControl() & 0x3F);
			sptr.MaxYvel = -24 - (GetRandomControl() & 7);
		}

		sptr.RotAng = GetRandomControl() & 0xFFF;

		if (GetRandomControl() & 1)
			sptr.RotAdd = -16 - (GetRandomControl() & 0xF);
		else
			sptr.RotAdd = (GetRandomControl() & 0xF) + 16;
	}
	else
	{
		if (body_part == -1)
		{
			sptr.Flags = 522;
			sptr.Gravity = -16 - (GetRandomControl() & 0x1F);
			sptr.MaxYvel = -16 - (GetRandomControl() & 7);
		}
		else
		{
			sptr.Flags = 586;
			sptr.FxObj = (unsigned char)body_part;
			sptr.Gravity = -32 - (GetRandomControl() & 0x3F);
			sptr.MaxYvel = -24 - (GetRandomControl() & 7);
		}
	}

	sptr.Scalar = 2;

	unsigned char size = 0;
	if (!type)
		size = (GetRandomControl() & 0x1F) + 128;
	else if (type == 1)
		size = (GetRandomControl() & 0x1F) + 64;
	else if (type < 254)
	{
		sptr.MaxYvel = 0;
		sptr.Gravity = 0;
		size = (GetRandomControl() & 0x1F) + 32;
	}
	else
		size = (GetRandomControl() & 0xF) + 48;

	sptr.Size = size;
	sptr.sSize = sptr.Size;

	if (type == 2)
		sptr.dSize = sptr.Size >> 2;
	else
	{
		sptr.dSize = sptr.Size >> 4;

		if (type == 7)
		{
			sptr.ColFadeSpeed >>= 2;
			sptr.FadeToBlack >>= 2;
			sptr.Life >>= 2;
			sptr.sLife >>= 2;
		}
	}

	Sparks.push_back(sptr);
}

void TriggerSuperJetFlame(ITEM_INFO* item, long yvel, long deadly)
{
	long dx = lara_item->pos.x_pos - item->pos.x_pos;
	long dz = lara_item->pos.z_pos - item->pos.z_pos;
	if (dx < -0x4000 || dx > 0x4000 || dz < -0x4000 || dz > 0x4000)
		return;

	long dy = (GetRandomControl() & 0x1FF) - yvel;
	if (dy < 512)
		dy = 512;

	SPARKS sptr;
	sptr.On = 1;
	sptr.sR = (GetRandomControl() & 0x1F) + 48;
	sptr.sG = sptr.sR;
	sptr.sB = (GetRandomControl() & 0x3F) + 192;
	sptr.dR = (GetRandomControl() & 0x3F) + 192;
	sptr.dG = (GetRandomControl() & 0x3F) + 128;
	sptr.dB = 32;
	sptr.ColFadeSpeed = 8;
	sptr.FadeToBlack = 8;
	sptr.TransType = 2;
	sptr.Life = unsigned char((dy >> 9) + (GetRandomControl() & 7) + 16);
	sptr.sLife = sptr.Life;
	sptr.x = (GetRandomControl() & 0x1F) + item->pos.x_pos - 16;
	sptr.y = (GetRandomControl() & 0x1F) + item->pos.y_pos - 16;
	sptr.z = (GetRandomControl() & 0x1F) + item->pos.z_pos - 16;
	sptr.Friction = 51;
	sptr.MaxYvel = 0;

	if (deadly)
		sptr.Flags = 539;
	else
		sptr.Flags = 538;

	sptr.Scalar = 2;
	sptr.dSize = unsigned char((GetRandomControl() & 0xF) + (dy >> 6) + 16);
	sptr.sSize = sptr.dSize >> 1;
	sptr.Size = sptr.dSize >> 1;

	if ((-item->trigger_flags & 7) == 1)
	{
		sptr.Gravity = -16 - (GetRandomControl() & 0x1F);
		sptr.Xvel = (GetRandomControl() & 0xFF) - 128;
		sptr.Yvel = (short)-dy;
		sptr.Zvel = (GetRandomControl() & 0xFF) - 128;
		sptr.dSize += sptr.dSize >> 2;
		return;
	}

	sptr.y -= 64;
	sptr.Gravity = short(-((dy >> 9) + GetRandomControl() % (dy >> 8)));
	sptr.Xvel = (GetRandomControl() & 0xFF) - 128;
	sptr.Yvel = (GetRandomControl() & 0xFF) - 128;
	sptr.Zvel = (GetRandomControl() & 0xFF) - 128;
	dy -= dy >> 2;

	if (!item->pos.y_rot)
		sptr.Zvel = (short)-dy;
	else if (item->pos.y_rot == 0x4000)
		sptr.Xvel = (short)-dy;
	else if (item->pos.y_rot == -0x8000)
		sptr.Zvel = (short)dy;
	else
		sptr.Xvel = (short)dy;

	Sparks.push_back(sptr);
}

void TriggerRocketSmokeTR4(long x, long y, long z, long col)
{
	SPARKS sptr;
	sptr.On = 1;
	sptr.sR = 0;
	sptr.sG = 0;
	sptr.sB = 0;
	sptr.dR = unsigned char(col + 64);
	sptr.dG = unsigned char(col + 64);
	sptr.dB = unsigned char(col + 64);
	sptr.FadeToBlack = 12;
	sptr.ColFadeSpeed = (GetRandomControl() & 3) + 4;
	sptr.TransType = 2;
	sptr.Life = (GetRandomControl() & 3) + 20;
	sptr.sLife = sptr.Life;
	sptr.x = (GetRandomControl() & 0xF) + x - 8;
	sptr.y = (GetRandomControl() & 0xF) + y - 8;
	sptr.z = (GetRandomControl() & 0xF) + z - 8;
	sptr.Xvel = (GetRandomControl() & 0xFF) - 128;
	sptr.Yvel = -4 - (GetRandomControl() & 3);
	sptr.Zvel = (GetRandomControl() & 0xFF) - 128;
	sptr.Friction = 4;

	if (GetRandomControl() & 1)
	{
		sptr.Flags = 538;
		sptr.RotAng = GetRandomControl() & 0xFFF;

		if (GetRandomControl() & 1)
			sptr.RotAdd = -16 - (GetRandomControl() & 0xF);
		else
			sptr.RotAdd = (GetRandomControl() & 0xF) + 16;
	}
	else
		sptr.Flags = 522;

	sptr.Scalar = 3;
	sptr.Gravity = -4 - (GetRandomControl() & 3);
	sptr.MaxYvel = -4 - (GetRandomControl() & 3);
	sptr.dSize = (GetRandomControl() & 7) + 32;
	sptr.sSize = sptr.dSize >> 2;
	sptr.Size = sptr.dSize >> 2;

	Sparks.push_back(sptr);
}

void SetupSplash(SPLASH_SETUP* setup)
{
	SPLASH_STRUCT* splash;
	long n;

	splash = splashes;
	n = 0;

	while (splash->flags & 1)
	{
		splash++;
		n++;
		if (n >= 4)
		{
			Sound.PlayEffect(SFX_LARA_SPLASH, (PHD_3DPOS*)setup);
			return;
		}
	}

	splash->flags = 1;
	splash->x = setup->x;
	splash->y = setup->y;
	splash->z = setup->z;
	splash->life = 62;
	splash->InnerRad = setup->InnerRad;
	splash->InnerSize = setup->InnerSize;
	splash->InnerRadVel = setup->InnerRadVel;
	splash->InnerYVel = setup->InnerYVel;
	splash->InnerY = setup->InnerYVel >> 2;
	splash->MiddleRad = setup->pad1;
	splash->MiddleSize = setup->MiddleRad;
	splash->MiddleRadVel = setup->MiddleSize;
	splash->MiddleYVel = setup->MiddleRadVel;
	splash->MiddleY = setup->MiddleRadVel >> 2;
	splash->OuterRad = setup->MiddleYVel;
	splash->OuterSize = setup->pad2;
	splash->OuterRadVel = setup->OuterRad;

	Sound.PlayEffect(SFX_LARA_SPLASH, (PHD_3DPOS*)setup);
}

void UpdateSplashes()	//(and ripples)
{
	SPLASH_STRUCT* splash;
	RIPPLE_STRUCT* ripple;

	for (int i = 0; i < 4; i++)
	{
		splash = &splashes[i];

		if (!(splash->flags & 1))
			continue;

		splash->InnerRad += splash->InnerRadVel >> 5;
		splash->InnerSize += splash->InnerRadVel >> 6;
		splash->InnerRadVel -= splash->InnerRadVel >> 6;
		splash->MiddleRad += splash->MiddleRadVel >> 5;
		splash->MiddleSize += splash->MiddleRadVel >> 6;
		splash->MiddleRadVel -= splash->MiddleRadVel >> 6;
		splash->OuterRad += splash->OuterRadVel >> 5;
		splash->OuterSize += splash->OuterRadVel >> 6;
		splash->OuterRadVel -= splash->OuterRadVel >> 6;
		splash->InnerY += splash->InnerYVel >> 4;
		splash->InnerYVel += 0x400;

		if (splash->InnerYVel > 0x4000)
			splash->InnerYVel = 0x4000;

		if (splash->InnerY < 0)
		{
			if (splash->InnerY < -0x7000)
				splash->InnerY = -0x7000;
		}
		else
		{
			splash->InnerY = 0;
			splash->flags |= 4;
			splash->life -= 2;

			if (!splash->life)
				splash->flags = 0;
		}

		splash->MiddleY += splash->MiddleYVel >> 4;
		splash->MiddleYVel += 0x380;

		if (splash->MiddleYVel > 0x4000)
			splash->MiddleYVel = 0x4000;

		if (splash->MiddleY < 0)
		{
			if (splash->MiddleY < -0x7000)
				splash->MiddleY = -0x7000;
		}
		else
		{
			splash->MiddleY = 0;
			splash->flags |= 8;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		ripple = &ripples[i];

		if (!(ripple->flags & 1))
			continue;

		if (ripple->size < 252)
		{
			if (ripple->flags & 2)
				ripple->size += 2;
			else
				ripple->size += 4;
		}

		if (ripple->init)
		{
			if (ripple->init < ripple->life)
			{
				if (ripple->flags & 2)
					ripple->init += 8;
				else
					ripple->init += 4;

				if (ripple->init >= ripple->life)
					ripple->init = 0;
			}
		}
		else
		{
			ripple->life -= 3;

			if (ripple->life > 250)
				ripple->flags = 0;
		}
	}
}
