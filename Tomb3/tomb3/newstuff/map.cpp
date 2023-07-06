#include "../tomb3/pch.h"
#include "map.h"
#include "../specific/input.h"
#include "../3dsystem/3d_gen.h"
#include "../specific/output.h"
#include "../game/draw.h"
#include "LaraDraw.h"
#include "../specific/draweffects.h"
#include "../game/gameflow.h"
#include "../specific/litesrc.h"
#include "../3dsystem/hwinsert.h"
#include "../game/inventry.h"
#include "../game/objects.h"
#include "../game/control.h"
#include "../game/lara.h"
#include "../game/effects.h"
#include "../specific/winmain.h"

#define CAM_SPEED	128
#define VIEW_DIST	(40 * WALL_SIZE)

static long ViewAngles[15] = { -0x4000, 0x200, 0, -0x4000, 0x3800, 0, -0x8000, 0x3800, 0, 0x4000, 0x3800, 0, 0, 0x3800, 0 };
static long MapXMin, MapXMax, MapYMin, MapYMax, MapZMin, MapZMax;
static bool DrawBoxes;
char RoomVisited[255];

static void phd_ScaleMatrix(long scalar)
{
	phd_mxptr[M00] <<= W2V_SHIFT + 2;
	phd_mxptr[M00] /= scalar;

	phd_mxptr[M01] <<= W2V_SHIFT + 2;
	phd_mxptr[M01] /= scalar;

	phd_mxptr[M02] <<= W2V_SHIFT + 2;
	phd_mxptr[M02] /= scalar;

	phd_mxptr[M10] <<= W2V_SHIFT + 2;
	phd_mxptr[M10] /= scalar;

	phd_mxptr[M11] <<= W2V_SHIFT + 2;
	phd_mxptr[M11] /= scalar;

	phd_mxptr[M12] <<= W2V_SHIFT + 2;
	phd_mxptr[M12] /= scalar;

	phd_mxptr[M20] <<= W2V_SHIFT + 2;
	phd_mxptr[M20] /= scalar;

	phd_mxptr[M21] <<= W2V_SHIFT + 2;
	phd_mxptr[M21] /= scalar;

	phd_mxptr[M22] <<= W2V_SHIFT + 2;
	phd_mxptr[M22] /= scalar;
}

static long GetRoomList(long x, long y, long z, long xSize, long ySize, long zSize, short* rList)
{
	ROOM_INFO* r;
	long lp, n, right, left, top, bottom, front, back;
	long xMin, yMin, zMin, xMax, yMax, zMax;
	
	left = x - (xSize >> 1);
	right = x + (xSize >> 1);
	top = y - (ySize >> 1);
	bottom = y + (ySize >> 1);
	back = z - (zSize >> 1);
	front = z + (zSize >> 1);
	MapXMax = 0x80000000;
	MapYMax = 0x80000000;
	MapZMax = 0x80000000;
	MapXMin = 0x7FFFFFFF;
	MapYMin = 0x7FFFFFFF;
	MapZMin = 0x7FFFFFFF;
	n = 0;

	for (lp = 0; lp < number_rooms; lp++)
	{
		r = &room[lp];

		if (!RoomVisited[lp])
			continue;

		xMin = r->x;
		xMax = xMin + (r->y_size << WALL_SHIFT);
		yMin = r->maxceiling;
		yMax = r->minfloor;
		zMin = r->z;
		zMax = zMin + (r->x_size << WALL_SHIFT);

		if (xMin < MapXMin)
			MapXMin = xMin;

		if (xMax > MapXMax)
			MapXMax = xMax;

		if (yMin < MapYMin)
			MapYMin = yMin;

		if (yMax > MapYMax)
			MapYMax = yMax;

		if (zMin < MapZMin)
			MapZMin = zMin;

		if (zMax > MapZMax)
			MapZMax = zMax;

		//Don't check Y to help with abrupt room disappearance
		if (xMin <= right || xMax >= left ||/* yMin <= bottom || yMax >= top ||*/ zMin <= front || zMax >= back)
		{
			*rList++ = (short)lp;
			n++;
		}
	}

	return n;
}

static long InterpolateAngle(long s, long e, long speed)
{
	long d;

	d = e - s;

	if (s > 0 && e > 0)
		d = -d;

	if (d > 0x8000)
		d = 0x8000 - d;
	else if (d < -0x8000)
		d = -d - 0x8000;

	return d / speed;
}

static void MoveCamPos(PHD_VECTOR* pos, long dir)
{
	switch (dir)
	{
	case 0:

		if (input & IN_LEFT)
			pos->z -= CAM_SPEED;

		if (input & IN_RIGHT)
			pos->z += CAM_SPEED;

		if (input & IN_FORWARD && !(input & IN_JUMP))
			pos->x -= CAM_SPEED;

		if (input & IN_BACK && !(input & IN_JUMP))
			pos->x += CAM_SPEED;

		if (input & IN_JUMP && input & IN_FORWARD)
			pos->y += CAM_SPEED;

		if (input & IN_JUMP && input & IN_BACK)
			pos->y -= CAM_SPEED;

		break;

	case 1:

		if (input & IN_LEFT)
			pos->z -= CAM_SPEED;

		if (input & IN_RIGHT)
			pos->z += CAM_SPEED;

		if (input & IN_FORWARD && !(input & IN_JUMP))
			pos->y -= CAM_SPEED;

		if (input & IN_BACK && !(input & IN_JUMP))
			pos->y += CAM_SPEED;

		if (input & IN_JUMP && input & IN_FORWARD)
			pos->x -= CAM_SPEED;

		if (input & IN_JUMP && input & IN_BACK)
			pos->x += CAM_SPEED;

		break;

	case 2:

		if (input & IN_LEFT)
			pos->x += CAM_SPEED;

		if (input & IN_RIGHT)
			pos->x -= CAM_SPEED;

		if (input & IN_FORWARD && !(input & IN_JUMP))
			pos->y -= CAM_SPEED;

		if (input & IN_BACK && !(input & IN_JUMP))
			pos->y += CAM_SPEED;

		if (input & IN_JUMP && input & IN_FORWARD)
			pos->z -= CAM_SPEED;

		if (input & IN_JUMP && input & IN_BACK)
			pos->z += CAM_SPEED;

		break;

	case 3:

		if (input & IN_LEFT)
			pos->z += CAM_SPEED;

		if (input & IN_RIGHT)
			pos->z -= CAM_SPEED;

		if (input & IN_FORWARD && !(input & IN_JUMP))
			pos->y -= CAM_SPEED;

		if (input & IN_BACK && !(input & IN_JUMP))
			pos->y += CAM_SPEED;

		if (input & IN_JUMP && input & IN_FORWARD)
			pos->x += CAM_SPEED;

		if (input & IN_JUMP && input & IN_BACK)
			pos->x -= CAM_SPEED;

		break;

	case 4:

		if (input & IN_LEFT)
			pos->x -= CAM_SPEED;

		if (input & IN_RIGHT)
			pos->x += CAM_SPEED;

		if (input & IN_FORWARD && !(input & IN_JUMP))
			pos->y -= CAM_SPEED;

		if (input & IN_BACK && !(input & IN_JUMP))
			pos->y += CAM_SPEED;

		if (input & IN_JUMP && input & IN_FORWARD)
			pos->z += CAM_SPEED;

		if (input & IN_JUMP && input & IN_BACK)
			pos->z -= CAM_SPEED;

		break;
	}
}

static void PrintMapObjects(short room_number)
{
	CurrentRoom = room_number;
	auto* r = &room[room_number];

	if (r->flags & ROOM_UNDERWATER)
		S_SetupBelowWater(camera_underwater);
	else
		S_SetupAboveWater(camera_underwater);

	phd_PushMatrix();
	phd_TranslateAbs(r->x, r->y, r->z);

	nPolyType = PT_STATICS;
	for (int i = 0; i < r->num_meshes; i++)
	{
		auto* mesh = &r->mesh[i];
		CurrentMesh = mesh;

		if (static_objects[mesh->static_number].flags & 2)
		{
			phd_PushMatrix();
			phd_TranslateAbs(mesh->x, mesh->y, mesh->z);
			phd_RotY(mesh->y_rot);
			auto clip = S_GetObjectBounds(&static_objects[mesh->static_number].x_minp);

			if (clip)
			{
				S_CalculateStaticMeshLight(mesh->x, mesh->y, mesh->z, mesh->shade, mesh->shadeB, r);
				phd_PutPolygons(meshes[static_objects[mesh->static_number].mesh_number], clip);
			}

			phd_PopMatrix();
		}
	}

	nPolyType = PT_OBJECTS;
	for (auto item_num = r->item_number; item_num != NO_ITEM;)
	{
		auto* item = &items[item_num];
		if (item->status != ITEM_INVISIBLE)
			objects[item->object_number].draw_routine(item);
		item_num = item->next_item;
	}

	nPolyType = PT_EFFECT;
	for (int i = r->fx_number; i != NO_ITEM; i = effects[i].next_fx)
		DrawEffect(i);

	phd_PopMatrix();
}

static void DrawMapRooms(long nList, short* rList)
{
	ROOM_INFO* r;
	OBJECT_INFO* obj;
	short bounds[6];

	CalcLaraMatrices(0);
	phd_PushUnitMatrix();
	phd_SetTrans(0, 0, 0);
	CalcLaraMatrices(1);
	phd_PopMatrix();
	SetLaraUnderwaterNodes();
	outside = 0;

	obj = &objects[0];
	if (obj->loaded && !(lara_item->flags & IFL_INVISIBLE))
	{
		nPolyType = PT_LARA;
		NewDrawLara(lara_item);
		if (DrawBoxes)
			SuperDrawBox(lara_item->pos.x_pos, lara_item->pos.y_pos, lara_item->pos.z_pos, GetBoundsAccurate(lara_item), 0xFF0000FF);
	}

	phd_left = 0;
	phd_top = 0;
	phd_right = phd_winxmax;
	phd_bottom = phd_winymax;

	nPolyType = PT_ROOM;
	for (int i = 0; i < nList; i++)
	{
		r = &room[rList[i]];
		camera_underwater = r->flags & ROOM_UNDERWATER;
		phd_TranslateAbs(r->x, r->y, r->z);
		S_InsertRoom(r->data, 0);

		if (DrawBoxes)
		{
			bounds[0] = WALL_SIZE;
			bounds[1] = (r->y_size - 1) << WALL_SHIFT;
			bounds[2] = short(r->maxceiling - r->y);
			bounds[3] = short(r->minfloor - r->y);
			bounds[4] = WALL_SIZE;
			bounds[5] = (r->x_size - 1) << WALL_SHIFT;
			SuperDrawBox(r->x, r->y, r->z, bounds, 0xFF007F00);
		}
	}

	for (int i = 0; i < nList; i++)
		PrintMapObjects(rList[i]);

	nPolyType = PT_EFFECT;
	S_DrawSparks();
	S_DrawSplashes();
	S_DrawBat();

	if (GF_Snow)
		DoSnow();
	if (GF_Rain)
		DoRain();

	S_DrawFootPrints();

	if (GF_WaterParts)
		DoUwEffect();
}

void do_map_option()
{
	PHD_VECTOR cam;
	PHD_3DPOS viewPos;
	long nList, num, dist, dir, nFrames, lp, sx, sy, sz;
	long xAdd, yAdd, zAdd, xRot, yRot, zRot, fog;
	short rList[255];
	short rots[3];
	static short cubeX, cubeY, cubeZ;
	short cube[6];

	dist = 10240;
	num = 0;
	dir = 0;
	nFrames = 2;
	nList = GetRoomList(lara_item->pos.x_pos, lara_item->pos.y_pos, lara_item->pos.z_pos, VIEW_DIST, VIEW_DIST, VIEW_DIST, rList);
	cam.x = lara_item->pos.x_pos;
	cam.y = lara_item->pos.y_pos - 768;
	cam.z = lara_item->pos.z_pos;
	rots[0] = (short)ViewAngles[0];
	rots[1] = (short)ViewAngles[1];
	rots[2] = (short)ViewAngles[2];
	viewPos.x_pos = 0;
	viewPos.y_pos = 0;
	viewPos.z_pos = 0;
	viewPos.x_rot = 0;
	viewPos.y_rot = 0;
	viewPos.z_rot = 0;
	fog = distanceFogValue;

	while (!(input & IN_OPTION))
	{
		S_UpdateInput();
		distanceFogValue = MAXINT32 >> W2V_SHIFT;

		for (int i = 0; i < nFrames; i++)
		{
			inputDB = GetDebouncedInput(input);
			cubeX += 60;
			cubeY += 182;
			cubeZ += 26;

			if (inputDB & IN_WALK)
				DrawBoxes = !DrawBoxes;

			if (num || !input)
				continue;

			if (input & IN_LOOK && input & IN_FORWARD)
			{
				dist -= CAM_SPEED;

				if (dist < WALL_SIZE)
					dist = WALL_SIZE;
			}

			if (input & IN_LOOK && input & IN_BACK)
			{
				dist += CAM_SPEED;

				if (dist > 25 * WALL_SIZE)
					dist = 25 * WALL_SIZE;
			}

			if (!(input & IN_ACTION) && !(input & IN_LOOK))
				MoveCamPos(&cam, dir);

			if (input & IN_DRAW)
			{
				cam.x = lara_item->pos.x_pos;
				cam.y = lara_item->pos.y_pos - 768;
				cam.z = lara_item->pos.z_pos;
				rots[0] = (short)ViewAngles[0];
				rots[1] = (short)ViewAngles[1];
				rots[2] = (short)ViewAngles[2];
				dir = 0;
			}

			if (cam.x < MapXMin)
				cam.x = MapXMin;
			else if (cam.x > MapXMax)
				cam.x = MapXMax;

			if (cam.y < MapYMin)
				cam.y = MapYMin;
			else if (cam.y > MapYMax)
				cam.y = MapYMax;

			if (cam.z < MapZMin)
				cam.z = MapZMin;
			else if (cam.z > MapZMax)
				cam.z = MapZMax;

			if (input & IN_ACTION && input & IN_LEFT)
			{
				dir--;

				if (dir < 1)
					dir = 4;

				num = 32;

				xRot = ViewAngles[3 * dir + 0];
				yRot = ViewAngles[3 * dir + 1];
				zRot = ViewAngles[3 * dir + 2];
				xAdd = InterpolateAngle(rots[0], xRot, num);
				yAdd = -InterpolateAngle(rots[1], yRot, num);
				zAdd = InterpolateAngle(rots[2], zRot, num);
			}

			if (input & IN_ACTION && input & IN_RIGHT)
			{
				dir++;

				if (dir > 4)
					dir = 1;

				num = 32;

				xRot = ViewAngles[3 * dir + 0];
				yRot = ViewAngles[3 * dir + 1];
				zRot = ViewAngles[3 * dir + 2];
				xAdd = InterpolateAngle(rots[0], xRot, num);
				yAdd = -InterpolateAngle(rots[1], yRot, num);
				zAdd = InterpolateAngle(rots[2], zRot, num);
			}

			if (input & IN_ACTION && input & IN_FORWARD && dir)
			{
				dir = 0;
				num = 32;

				xRot = ViewAngles[0];
				yRot = ViewAngles[1];
				zRot = ViewAngles[2];
				xAdd = InterpolateAngle(rots[0], xRot, num);
				yAdd = -InterpolateAngle(rots[1], yRot, num);
				zAdd = InterpolateAngle(rots[2], zRot, num);
			}

			if (input & IN_ACTION && input & IN_BACK && !dir)
			{
				dir = 1;
				num = 32;

				xRot = ViewAngles[3];
				yRot = ViewAngles[4];
				zRot = ViewAngles[5];
				xAdd = InterpolateAngle(rots[0], xRot, num);
				yAdd = -InterpolateAngle(rots[1], yRot, num);
				zAdd = InterpolateAngle(rots[2], zRot, num);
			}

			nList = GetRoomList(cam.x, cam.y, cam.z, VIEW_DIST, VIEW_DIST, VIEW_DIST, rList);
		}

		if (num)
		{
			for (lp = 0; lp < nFrames; lp++)
			{
				rots[0] += (short)xAdd;
				rots[1] += (short)yAdd;
				rots[2] += (short)zAdd;
				num--;

				if (!num)
				{
					rots[0] = (short)xRot;
					rots[1] = (short)yRot;
					rots[2] = (short)zRot;
					break;
				}
			}
		}

		phd_GenerateW2V(&viewPos);
		phd_RotYXZ(rots[0], rots[1], 0);
		sx = cam.x + (-dist * phd_mxptr[M01] >> W2V_SHIFT);
		sy = cam.y + (-dist * phd_mxptr[M11] >> W2V_SHIFT);
		sz = cam.z + (-dist * phd_mxptr[M21] >> W2V_SHIFT);
		phd_LookAt(sx, sy, sz, cam.x, cam.y, cam.z, rots[2]);
		S_InitialisePolyList(1);
		phd_ScaleMatrix(2 << (W2V_SHIFT + 2));

		if (DrawBoxes)
		{
			//Cube at view target
			cube[0] = -256;
			cube[1] = 256;
			cube[2] = -256;
			cube[3] = 256;
			cube[4] = -256;
			cube[5] = 256;
			phd_PushMatrix();
			phd_TranslateAbs(cam.x, cam.y, cam.z);
			phd_RotYXZ(cubeY, cubeX, cubeZ);
			SuperDrawBox(cube, 0xFFFF0000);
			phd_PopMatrix();
		}

		DrawMapRooms(nList, rList);
		S_AnimateTextures(nFrames);
		S_OutputPolyList();
		nFrames = S_DumpScreen();
	}

	distanceFogValue = fog;
}
