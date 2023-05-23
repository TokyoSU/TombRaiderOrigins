#include "pch.h"
#include "fish_emitter.h"
#include "function_stubs.h"
#include "3dmath.h"
#include "control.h"
#include "lara.h"
#include "effect2.h"
#include "effects.h"
#include "items.h"
#include "specificfx.h"
#include "dxshell.h"
#include "objects.h"
#include "tomb4fx.h"
#include "file.h"
#include "function_table.h"
#include "gamemain.h"
#include "output.h"
#include "winmain.h"
#include "savegame.h"

constexpr auto FISH_SIZE = 64;

static unsigned char jungle_fish_ranges[1][3] =
{
	{ 8, 20, 3 }
};

static unsigned char temple_fish_ranges[3][3] =
{
	{ 4, 4, 2 },
	{ 4, 16, 2 },
	{ 4, 28, 3 }
};

static unsigned char quadchase_fish_ranges[8][3] =
{
	{ 4, 12, 1 },
	{ 0, 12, 2 },
	{ 8, 4, 2 },
	{ 4, 8, 1 },
	{ 4, 16, 2 },
	{ 4, 24, 1 },
	{ 12, 4, 1 },
	{ 16, 4, 1 }
};

static unsigned char house_fish_ranges[7][3]
{
	{ 4, 4, 1 },
	{ 16, 8, 2 },
	{ 24, 8, 2 },
	{ 8, 16, 2 },
	{ 8, 12, 1 },
	{ 20, 8, 2 },
	{ 16, 8, 1 }
};

static unsigned char shore_fish_ranges[3][3] =
{
	{ 12, 12, 6 },
	{ 12, 20, 6 },
	{ 20, 4, 8 }
};

static unsigned char crash_fish_ranges[1][3]
{
	{ 20, 4, 6 }
};

static unsigned char rapids_fish_ranges[2][3]
{
	{ 16, 16, 8 },
	{ 4, 8, 5 }
};

enum FishOcb
{
	OCB_Jungle = 0x0,
	OCB_Temple = 0x1,
	OCB_Quadbike = 0x2,
	OCB_Gym = 0x4,
	OCB_Shore = 0x8,
	OCB_Crash = 0x10,
	OCB_Rapids = 0x20,

	OCB_PirahnaMode = 0x40, // Enable pirahna behaviour !

	OCB_AllRanges = OCB_Jungle | OCB_Temple | OCB_Quadbike | OCB_Gym | OCB_Shore | OCB_Crash | OCB_Crash | OCB_Rapids,
};

#define IS_OCB_VALID(trigger, x) trigger & x

static LEADER_INFO LeadInfo[MAX_FISHES_LEADER];
// Possible to have 32 fish emitter...
static FISH_INFO Fishes[(MAX_FISHES * 32) + MAX_FISHES_LEADER]; // +8 for the leader count.
static int nCurrentLeader = 0;

int GetFishRangeCount(ITEM_INFO* item)
{
	if (IS_OCB_VALID(item->ocb, OCB_Jungle))
		return 0;
	else if (IS_OCB_VALID(item->ocb, OCB_Temple))
		return 2;
	else if (IS_OCB_VALID(item->ocb, OCB_Quadbike))
		return 7;
	else if (IS_OCB_VALID(item->ocb, OCB_Gym))
		return 6;
	else if (IS_OCB_VALID(item->ocb, OCB_Shore))
		return 2;
	else if (IS_OCB_VALID(item->ocb, OCB_Crash))
		return 0;
	else if (IS_OCB_VALID(item->ocb, OCB_Rapids))
		return 1;
	return 0;
}

void SetupShoal(ITEM_INFO* item, LEADER_INFO* leader, short shoal_number)
{
	if (IS_OCB_VALID(item->ocb, OCB_Jungle))
	{
		leader->Xrange = (jungle_fish_ranges[shoal_number][0] + 2) << 8;
		leader->Yrange = jungle_fish_ranges[shoal_number][2] << 8;
		leader->Zrange = (jungle_fish_ranges[shoal_number][1] + 2) << 8;
	}
	else if (IS_OCB_VALID(item->ocb, OCB_Temple))
	{
		leader->Xrange = (temple_fish_ranges[shoal_number][0] + 2) << 8;
		leader->Yrange = temple_fish_ranges[shoal_number][2] << 8;
		leader->Zrange = (temple_fish_ranges[shoal_number][1] + 2) << 8;
	}
	else if (IS_OCB_VALID(item->ocb, OCB_Quadbike))
	{
		leader->Xrange = (quadchase_fish_ranges[shoal_number][0] + 2) << 8;
		leader->Yrange = quadchase_fish_ranges[shoal_number][2] << 8;
		leader->Zrange = (quadchase_fish_ranges[shoal_number][1] + 2) << 8;
	}
	else if (IS_OCB_VALID(item->ocb, OCB_Gym))
	{
		leader->Xrange = (house_fish_ranges[shoal_number][0] + 2) << 8;
		leader->Yrange = house_fish_ranges[shoal_number][2] << 8;
		leader->Zrange = (house_fish_ranges[shoal_number][1] + 2) << 8;
	}
	else if (IS_OCB_VALID(item->ocb, OCB_Shore))
	{
		leader->Xrange = (shore_fish_ranges[shoal_number][0] + 2) << 8;
		leader->Yrange = shore_fish_ranges[shoal_number][2] << 8;
		leader->Zrange = (shore_fish_ranges[shoal_number][1] + 2) << 8;
	}
	else if (IS_OCB_VALID(item->ocb, OCB_Crash))
	{
		leader->Xrange = (crash_fish_ranges[shoal_number][0] + 2) << 8;
		leader->Yrange = (crash_fish_ranges[shoal_number][2]) << 8;
		leader->Zrange = (crash_fish_ranges[shoal_number][1] + 2) << 8;
	}
	else if (IS_OCB_VALID(item->ocb, OCB_Rapids))
	{
		leader->Xrange = (rapids_fish_ranges[shoal_number][0] + 2) << 8;
		leader->Yrange = rapids_fish_ranges[shoal_number][2] << 8;
		leader->Zrange = (rapids_fish_ranges[shoal_number][1] + 2) << 8;
	}
	else
	{
		leader->Xrange = 256;
		leader->Zrange = 256;
		leader->Yrange = 256;
	}
}

void SetupFish(ITEM_INFO* item)
{
	LEADER_INFO* pLeader = &LeadInfo[item->hit_points];
	FISH_INFO* pFish = &Fishes[item->hit_points]; // the leader

	SetupShoal(item, pLeader, item->item_flags[0]);
	short x = pLeader->Xrange;
	short y = pLeader->Yrange;
	short z = pLeader->Zrange;

	pFish->x = 0;
	pFish->y = 0;
	pFish->z = 0;
	pFish->angle = 0;
	pFish->speed = (GetRandomControl() & 0x3F) + 8;
	pFish->swim = GetRandomControl() & 0x3F;

	// Every fish except leader.
	for (int i = 0; i < MAX_FISHES; i++)
	{
		pFish = &Fishes[(MAX_FISHES * item->hit_points) + MAX_FISHES_LEADER + i];
		pFish->x = GetRandomControl() % (x << 1) - x;
		pFish->y = GetRandomControl() % y;
		pFish->z = GetRandomControl() % (z << 1) - z;
		pFish->desty = GetRandomControl() % y;
		pFish->angle = GetRandomControl() & 0xFFF;
		pFish->speed = (GetRandomControl() & 0x1F) + 32;
		pFish->swim = GetRandomControl() & 0x3F;
		pFish->type = GetRandomControl() & 1;
	}

	pLeader->on = TRUE;
	pLeader->angle = 0;
	pLeader->speed = (GetRandomControl() & 0x7F) + 32;
	pLeader->angle_time = 0;
	pLeader->speed_time = 0;
	pLeader->carcass_item = NO_ITEM;
	pLeader->hit_wait = 0;
	pLeader->r = RGBA_GETRED(rooms[item->room_number].ambient);
	pLeader->g = RGBA_GETGREEN(rooms[item->room_number].ambient);
	pLeader->b = RGBA_GETBLUE(rooms[item->room_number].ambient);
}

static long FishNearLara(PHD_3DPOS* pos, long dist, ITEM_INFO* item)
{
	long dx = pos->x_pos - item->pos.x_pos;
	long dy = abs(pos->y_pos - item->pos.y_pos);
	long dz = pos->z_pos - item->pos.z_pos;
	if (dx < -dist || dx > dist || dz < -dist || dz > dist || dy < -3072 || dy > 3072 || SQUARE(dz) + SQUARE(dx) > SQUARE(dist) || dy > dist)
		return 0;
	return 1;
}

void InitialiseFish(short item_number)
{
	auto* item = &items[item_number];

	// Get a fish leader for this item !
	if (nCurrentLeader >= MAX_FISHES_LEADER) // reset and replace the first leader !
		nCurrentLeader = 0;
	item->hit_points = nCurrentLeader++;

	// Get a fish range (define the range x/y/z limits)
	item->item_flags[0] = short(GetRandomControl() & GetFishRangeCount(item));

	SetupFish(item);
}

static short GetCarcassItemNumber(ITEM_INFO* pirahna)
{
	for (int i = 0; i < level_items; i++)
	{
		auto* item = &items[i];
		// need to be the fish_target AND in the same room !
		if (item->object_number == FISH_TARGET && pirahna->room_number == item->room_number && item->status == ITEM_ACTIVE)
			return item->index;
	}
	return NO_ITEM;
}

void ControlFish(short item_number)
{
	ITEM_INFO* item;
	ITEM_INFO* enemy;
	LEADER_INFO* pLeader;
	FISH_INFO* pFish;
	PHD_3DPOS pos;
	long pirahna_attack, diff, x, z, dx, dz;

	item = &items[item_number];
	if (!TriggerActive(item))
		return;
	enemy = item;
	pLeader = &LeadInfo[item->hit_points];
	pLeader->carcass_item = GetCarcassItemNumber(item);

	if ((item->ocb & OCB_PirahnaMode))
	{
		if (pLeader->carcass_item != NO_ITEM)
			pirahna_attack = 2;
		else
			pirahna_attack = item->room_number == lara_item->room_number;
	}
	else
		pirahna_attack = 0;

	if (pLeader->hit_wait)
		pLeader->hit_wait--;

	pFish = &Fishes[item->hit_points];
	if (pirahna_attack != 0)
	{
		if (pirahna_attack != 1 && pLeader->carcass_item != NO_ITEM)
			enemy = &items[pLeader->carcass_item];
		else
			enemy = lara_item;
		pFish->angle = ((-(long)mGetAngle(pFish->x + item->pos.x_pos, pFish->z + item->pos.z_pos, enemy->pos.x_pos, enemy->pos.z_pos) - 16384) >> 4) & 0xFFF;
		pLeader->angle = pFish->angle;
		pLeader->speed = (GetRandomControl() & 0x3F) - 64;
	}

	diff = pFish->angle - pLeader->angle;
	if (diff > 2048)
		diff -= 4096;
	else if (diff < -2048)
		diff += 4096;

	if (diff > 128)
	{
		pFish->angadd -= 4;
		if (pFish->angadd < -120)
			pFish->angadd = -120;
	}
	else if (diff < -128)
	{
		pFish->angadd += 4;
		if (pFish->angadd > 120)
			pFish->angadd = 120;
	}
	else
	{
		pFish->angadd -= pFish->angadd >> 2;
		if (abs(pFish->angadd) < 4)
			pFish->angadd = 0;
	}

	pFish->angle += pFish->angadd;
	if (diff > 1024)
		pFish->angle += pFish->angadd >> 2;
	pFish->angle &= 0xFFF;
	diff = pFish->speed - pLeader->speed;

	if (diff < -4)
	{
		diff = pFish->speed + (GetRandomControl() & 3) + 1;
		if (diff < 0)
			diff = 0;
		pFish->speed = (unsigned char)diff;
	}
	else if (diff > 4)
	{
		diff = pFish->speed - (GetRandomControl() & 3) - 1;
		if (diff > 255)
			diff = 255;
		pFish->speed = (unsigned char)diff;
	}

	pFish->swim = (pFish->swim + (pFish->speed >> 4)) & 0x3F;
	x = pFish->x - ((pFish->speed * rcossin_tbl[2 * pFish->angle]) >> 13);
	z = pFish->z + ((pFish->speed * rcossin_tbl[2 * pFish->angle + 1]) >> 13);

	if (!pirahna_attack)
	{
		if (z < -pLeader->Zrange)
		{
			z = -pLeader->Zrange;

			if (pFish->angle < 2048)
				pLeader->angle = pFish->angle - (GetRandomControl() & 0x7F) - 128;
			else
				pLeader->angle = pFish->angle + (GetRandomControl() & 0x7F) + 128;

			pLeader->angle_time = (GetRandomControl() & 0xF) + 8;
			pLeader->speed_time = 0;
		}
		else if (z > pLeader->Zrange)
		{
			z = pLeader->Zrange;

			if (pFish->angle > 3072)
				pLeader->angle = pFish->angle - (GetRandomControl() & 0x7F) - 128;
			else
				pLeader->angle = pFish->angle + (GetRandomControl() & 0x7F) + 128;

			pLeader->angle_time = (GetRandomControl() & 0xF) + 8;
			pLeader->speed_time = 0;
		}

		if (x < -pLeader->Xrange)
		{
			x = -pLeader->Xrange;

			if (pFish->angle < 1024)
				pLeader->angle = pFish->angle - (GetRandomControl() & 0x7F) - 128;
			else
				pLeader->angle = pFish->angle + (GetRandomControl() & 0x7F) + 128;

			pLeader->angle_time = (GetRandomControl() & 0xF) + 8;
			pLeader->speed_time = 0;
		}
		else if (x > pLeader->Xrange)
		{
			x = pLeader->Xrange;

			if (pFish->angle < 3072)
				pLeader->angle = pFish->angle - (GetRandomControl() & 0x7F) - 128;
			else
				pLeader->angle = pFish->angle + (GetRandomControl() & 0x7F) + 128;

			pLeader->angle_time = (GetRandomControl() & 0xF) + 8;
			pLeader->speed_time = 0;
		}

		if (!(GetRandomControl() & 0xF))
			pLeader->angle_time = 0;

		if (pLeader->angle_time)
			pLeader->angle_time--;
		else
		{
			pLeader->angle_time = (GetRandomControl() & 0xF) + 8;
			diff = (GetRandomControl() & 0x3F) - 24;

			if (!(GetRandomControl() & 3))
				diff <<= 5;

			pLeader->angle = (pLeader->angle + diff) & 0xFFF;
		}

		if (pLeader->speed_time)
			pLeader->speed_time--;
		else
		{
			pLeader->speed_time = (GetRandomControl() & 0x1F) + 32;

			if (!(GetRandomControl() & 7))
				pLeader->speed = (GetRandomControl() & 0x7F) + 128;
			else if (!(GetRandomControl() & 3))
				pLeader->speed += (GetRandomControl() & 0x7F) + 32;
			else if (pLeader->speed > 140)
				pLeader->speed += 208 - (GetRandomControl() & 0x1F);
			else
			{
				pLeader->speed_time = (GetRandomControl() & 3) + 4;
				pLeader->speed += (GetRandomControl() & 0x1F) - 15;
			}
		}
	}

	pFish->x = (short)x;
	pFish->z = (short)z;

	for (int i = 0; i < MAX_FISHES; i++)
	{
		pFish = &Fishes[(MAX_FISHES * item->hit_points) + MAX_FISHES_LEADER + i];

		if (item->ocb & OCB_PirahnaMode)
		{
			pos.x_pos = item->pos.x_pos + pFish->x;
			pos.y_pos = item->pos.y_pos + pFish->y;
			pos.z_pos = item->pos.z_pos + pFish->z;

			if (FishNearLara(&pos, 256, pirahna_attack >= 2 ? enemy : lara_item))
			{
				if (pLeader->hit_wait == 0)
				{
					DoBloodSplat(item->pos.x_pos + pFish->x, item->pos.y_pos + pFish->y, item->pos.z_pos + pFish->z, 0, 0, pirahna_attack < 2 ? lara_item->room_number : enemy->room_number);
					pLeader->hit_wait = 8;
				}

				if (pirahna_attack != 2)
					lara_item->hit_points -= 4;
			}
		}

		dx = SQUARE(pFish->x - x - 128 * i + 3072);
		dz = SQUARE(pFish->z - z + 128 * i - 3072);
		diff = pFish->angle - (((-(long)mGetAngle(pFish->x, pFish->z, x, z) - 16384) >> 4) & 0xFFF);

		if (diff > 2048)
			diff -= 4096;
		else if (diff < -2048)
			diff += 4096;

		if (diff > 128)
		{
			pFish->angadd -= 4;
			if (pFish->angadd < -(i >> 1) - 92)
				pFish->angadd = -(i >> 1) - 92;
		}
		else if (diff < -128)
		{
			pFish->angadd += 4;
			if (pFish->angadd > (i >> 1) + 92)
				pFish->angadd = (i >> 1) + 92;
		}
		else
		{
			pFish->angadd -= pFish->angadd >> 2;
			if (abs(pFish->angadd) < 4)
				pFish->angadd = 0;
		}

		pFish->angle += pFish->angadd;
		if (diff > 1024)
			pFish->angle += pFish->angadd >> 2;
		pFish->angle &= 0xFFF;

		if (dx + dz < 16384 * SQUARE(i) + 1048576)
		{
			if (pFish->speed > 2 * i + 32)
				pFish->speed -= pFish->speed >> 5;
		}
		else
		{
			if (pFish->speed < (i >> 1) + 160)
				pFish->speed += unsigned char((i >> 1) + (GetRandomControl() & 3) + 1);
			if (pFish->speed > (i >> 1) - 4 * i + 160)
				pFish->speed = (i >> 1) - 4 * i - 96;
		}

		if (GetRandomControl() & 1)
			pFish->speed -= GetRandomControl() & 1;
		else
			pFish->speed += GetRandomControl() & 1;

		if (pFish->speed < 32)
			pFish->speed = 32;
		else if (pFish->speed > 200)
			pFish->speed = 200;

		pFish->swim = (pFish->swim + (pFish->speed >> 4) + (pFish->speed >> 5)) & 0x3F;
		dx = pFish->x - ((pFish->speed * rcossin_tbl[2 * pFish->angle]) >> 13);
		dz = pFish->z + ((pFish->speed * rcossin_tbl[2 * pFish->angle + 1]) >> 13);

		if (dz < -32000)
			dz = -32000;
		else if (dz > 32000)
			dz = 32000;

		if (dx < -32000)
			dx = -32000;
		else if (dx > 32000)
			dx = 32000;

		pFish->x = (short)dx;
		pFish->z = (short)dz;

		if (pirahna_attack == 0 && abs(pFish->y - pFish->desty) < 16)
			pFish->desty = GetRandomControl() % pLeader->Yrange;
		else if (abs(pFish->y - pFish->desty) < 16)
			pFish->desty = short(enemy->pos.y_pos - item->pos.y_pos + (GetRandomControl() & 0xFF));

		pFish->y += (pFish->desty - pFish->y) >> 4;
	}
}

static void ClipCheckPoint(D3DTLVERTEX* v, long x, long y, long z, short* clip)
{
	char clipFlag = 0;

	if (x < phd_winxmin)
		clipFlag++;
	else if (x > phd_winxmax)
		clipFlag += 2;

	if (y < phd_winymin)
		clipFlag += 4;
	else if (y > phd_winymax)
		clipFlag += 8;

	*clip = clipFlag;
	v->sx = (float)x;
	v->sy = (float)y;
	v->sz = (float)z;
	v->rhw = f_persp / (float)z * f_oneopersp;
}

static void setXYZ3(D3DTLVERTEX* v,
	long x1, long y1, long z1,
	long x2, long y2, long z2,
	long x3, long y3, long z3, short* clip)

{
	ClipCheckPoint(&v[0], x1, y1, z1, &clip[0]);
	ClipCheckPoint(&v[1], x2, y2, z2, &clip[1]);
	ClipCheckPoint(&v[2], x3, y3, z3, &clip[2]);
}

void S_DrawFish(ITEM_INFO* item)
{
	if (!item->active)
		return;
	auto* pLeader = &LeadInfo[item->hit_points];
	if (!pLeader->on)
		return;

	DXDISPLAYMODE* dm = &G_dxinfo->DDInfo[G_dxinfo->nDD].D3DDevices[G_dxinfo->nD3D].DisplayModes[G_dxinfo->nDisplayMode];
	long w = dm->w;
	long h = dm->h;
	long sx = phd_winxmin + phd_winxmax;
	long sy = phd_winymax + phd_winymin;

	D3DTLVERTEX v[3];
	TEXTURESTRUCT tex;

	auto* pFish = &Fishes[(MAX_FISHES * item->hit_points) + MAX_FISHES_LEADER];
	long x1, y1, z1, x2, y2, z2, x3, y3, z3, size;
	long XYZ[3][3]{};
	long point[3];

	for (int i = 0; i < MAX_FISHES; i++, pFish++)
	{
		long x = item->pos.x_pos + pFish->x;
		long y = item->pos.y_pos + pFish->y;
		long z = item->pos.z_pos + pFish->z;
		short ang = ((rcossin_tbl[pFish->swim << 7] >> 5) + pFish->angle - 2048) & 0xFFF;

		mCalcPoint(x, y, z, point);
		ProjectPCoord(point[0], point[1], point[2], XYZ[0], w >> 1, h >> 1, phd_persp);

		size = (FISH_SIZE * rcossin_tbl[i << 7] >> 12) + 192;
		x -= (size * rcossin_tbl[ang << 1]) >> 12;
		y -= size;
		z += (size * rcossin_tbl[(ang << 1) + 1]) >> 12;

		mCalcPoint(x, y, z, point);
		ProjectPCoord(point[0], point[1], point[2], XYZ[1], w >> 1, h >> 1, phd_persp);

		y += size << 1;

		mCalcPoint(x, y, z, point);
		ProjectPCoord(point[0], point[1], point[2], XYZ[2], w >> 1, h >> 1, phd_persp);

		x1 = XYZ[0][0];
		y1 = XYZ[0][1];
		z1 = XYZ[0][2];
		x2 = XYZ[1][0];
		y2 = XYZ[1][1];
		z2 = XYZ[1][2];
		x3 = XYZ[2][0];
		y3 = XYZ[2][1];
		z3 = XYZ[2][2];

		if (z1 > 0x5000 || z1 < 32 || z2 < 32 || z3 < 32)
			continue;
		if ((x1 < phd_winxmin && x2 < phd_winxmin && x3 < phd_winxmin) || (x1 >= sx && x2 >= sx && x3 >= sx))
			continue;
		if ((y1 < phd_winymin && y2 < phd_winymin && y3 < phd_winymin) || (y1 >= sy && y2 >= sy && y3 >= sy))
			continue;

		if (ang < 1024)
			ang -= 512;
		else if (ang < 2048)
			ang -= 1536;
		else if (ang < 3072)
			ang -= 2560;
		else
			ang -= 3584;

		if (ang > 512 || ang < 0)
			ang = 0;
		else if (ang < 256)
			ang >>= 2;
		else
			ang = (512 - ang) >> 2;

		ang += i;
		if (ang > 128)
			ang = 128;
		ang += 80;

		z1 <<= W2V_SHIFT;
		z2 <<= W2V_SHIFT;
		z3 <<= W2V_SHIFT;

		setXYZ3(v, x1, y1, z1, x2, y2, z2, x3, y3, z3, clipflags);

		v[0].color = RGBA_MAKE(pLeader->r, pLeader->g, pLeader->b, 255);
		v[1].color = RGBA_MAKE(pLeader->r, pLeader->g, pLeader->b, 255);
		v[2].color = RGBA_MAKE(pLeader->r, pLeader->g, pLeader->b, 255);
		v[0].specular = RGBA_MAKE(0, 0, 0, 255);
		v[1].specular = RGBA_MAKE(0, 0, 0, 255);
		v[2].specular = RGBA_MAKE(0, 0, 0, 255);

		SPRITESTRUCT* sprite;
		if ((item->ocb & OCB_PirahnaMode)) // Pirahna
			sprite = &spriteinfo[objects[FISH_SPRITES].mesh_index + 0];
		else
			sprite = &spriteinfo[objects[FISH_SPRITES].mesh_index + 1];

		tex.drawtype = 1;
		tex.tpage = sprite->tpage;

		if (pFish->type == 0)
		{
			tex.u1 = sprite->x1;
			tex.v1 = sprite->y1;
			tex.u2 = sprite->x2;
			tex.v2 = sprite->y1;
			tex.u3 = sprite->x1;
			tex.v3 = sprite->y2;
		}
		else
		{
			tex.u1 = sprite->x2;
			tex.v1 = sprite->y2;
			tex.u2 = sprite->x1;
			tex.v2 = sprite->y2;
			tex.u3 = sprite->x2;
			tex.v3 = sprite->y1;
		}

		tex.flag = 0;
		AddTriSorted(v, 0, 1, 2, &tex, 1);
	}
}

void FishTargetControl(short item_number)
{
	ITEM_INFO* item;
	FLOOR_INFO* floor;
	long h;
	short old_room, room_number, maxfs;

	item = &items[item_number];
	if (item->status != ITEM_ACTIVE)
		return;

	item->pos.y_pos += item->fallspeed;
	old_room = item->room_number;
	room_number = item->room_number;
	floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);
	h = GetHeight(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
	if (item->room_number != room_number)
		ItemNewRoom(item_number, room_number);

	h -= 64;
	if (item->pos.y_pos >= h)
	{
		item->pos.y_pos = h;
		item->fallspeed = 0;
		item->pos.z_rot = 0x6000;
		return;
	}

	if (rooms[room_number].flags & ROOM_UNDERWATER)
		item->pos.z_rot += item->fallspeed << 3;
	else
		item->pos.z_rot += item->fallspeed << 1;

	if (item->pos.z_rot > 0x6000)
		item->pos.z_rot = 0x6000;

	item->fallspeed += rooms[room_number].flags & ROOM_UNDERWATER ? 1 : 8;
	maxfs = rooms[old_room].flags & ROOM_UNDERWATER ? 64 : 512;

	if (item->fallspeed > maxfs)
		item->fallspeed = maxfs;

	AnimateItem(item);

	if (rooms[room_number].flags & ROOM_UNDERWATER && !(rooms[old_room].flags & ROOM_UNDERWATER))
	{
		splash_setup.x = item->pos.x_pos;
		splash_setup.y = rooms[room_number].maxceiling;
		splash_setup.z = item->pos.z_pos;
		splash_setup.InnerRad = 16;
		splash_setup.InnerSize = 12;
		splash_setup.InnerRadVel = 160;
		splash_setup.InnerYVel = -72 * item->fallspeed;
		splash_setup.pad1 = 24;
		splash_setup.MiddleRad = 24;
		splash_setup.MiddleSize = 224;
		splash_setup.MiddleRadVel = -36 * item->fallspeed;
		splash_setup.MiddleYVel = 32;
		splash_setup.pad2 = 32;
		splash_setup.OuterRad = 272;
		SetupSplash(&splash_setup);
		SplashCount = 16;

		item->fallspeed = 16;
		item->pos.y_pos = rooms[room_number].maxceiling + 1;
	}
}
