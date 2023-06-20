#include "pch.h"
#include "delstuff.h"
#include "specificfx.h"
#include "3dmath.h"
#include "draw.h"
#include "output.h"
#include "hair.h"
#include "objects.h"
#include "control.h"
#include "lara_states.h"
#include "input.h"
#include "function_stubs.h"
#include "lara.h"
#include "gameflow.h"
#include "tomb4.h"

short* GLaraShadowframe;
MATRIX_FLT lara_matrices[NUM_LARA_MESHES];
MATRIX_FLT lara_joint_matrices[NUM_LARA_MESHES];
long LaraNodeAmbient[2];
long bLaraUnderWater;
uchar LaraNodeUnderwater[NUM_LARA_MESHES];
char SkinVertNums[40][12];
char ScratchVertNums[40][12];
char bLaraInWater;

char HairRotScratchVertNums[5][12] =
{
	{ 4, 5, 6, 7, -1, 0, 0, 0, 0, 0, 0, 0 },
	{ 5, 6, 7, 4, -1, 0, 0, 0, 0, 0, 0, 0 },
	{ 6, 7, 4, 5, -1, 0, 0, 0, 0, 0, 0, 0 },
	{ 7, 4, 5, 6, -1, 0, 0, 0, 0, 0, 0, 0 },
	{ 4, 5, 6, 7, -1, 0, 0, 0, 0, 0, 0, 0 }
};

static long lara_mesh_sweetness_table[NUM_LARA_MESHES] = { 0, 1, 2, 3, 4, 5, 6, 7, 14, 8, 9, 10, 11, 12, 13 };
static char lara_underwater_skin_sweetness_table[NUM_LARA_MESHES] = { 0, 2, 3, 0, 5, 6, 7, 9, 10, 11, 12, 13, 14, 8, 0 };

static char NodesToStashToScratch[NUM_LARA_MESHES][2] =
{
	{1, 3},
	{4, 5},
	{6, 7},
	{2, 8},
	{9, 10},
	{11, 12},
	{0, 13},
	{14, 17},
	{18, 19},
	{20, 21},
	{15, 22},
	{23, 24},
	{25, 26},
	{16, 27},
	{127, 127}
};

static char NodesToStashFromScratch[NUM_LARA_MESHES][4] =
{
	{0, 1, 2, -1},
	{3, 4, -1, 0},
	{5, 6, -1, 0},
	{7, -1, 0, 0},
	{8, 9, -1, 0},
	{10, 11, -1, 0},
	{12, -1, 0, 0},
	{13, 16, 14, 15},
	{27, 28, 34, -1},
	{17, 18, -1, 0},
	{19, 20, -1, 0},
	{21, -1, 0, 0},
	{22, 23, -1, 0},
	{24, 25, -1, 0},
	{26, -1, 0, 0}
};

static uchar SkinUseMatrix[NUM_LARA_MESHES][2] =
{
	{255, 255},
	{LM_LTHIGH, LM_LSHIN},
	{255, 255},
	{255, 255},
	{LM_RTHIGH, LM_RSHIN},
	{255, 255},
	{255, 255},
	{255, 255},
	{LM_ROUTARM, LM_RHAND},
	{255, 255},
	{255, 255},
	{LM_LOUTARM, LM_LHAND},
	{255, 255},
	{255, 255},
	{254, 254}
};

static long in_joints;

void DrawLara(ITEM_INFO* item, long mirror)
{
	OBJECT_INFO* obj;
	FVECTOR v0;
	FVECTOR v1;
	short** meshpp;
	long* bone;
	short* rot;
	long top, bottom, left, right, dx, dy, dz, dist, stash, xRot;
	static long a = 255;
	
	top = phd_top;
	bottom = phd_bottom;
	left = phd_left;
	right = phd_right;
	phd_top = 0;
	phd_left = 0;
	phd_bottom = phd_winymax;
	phd_right = phd_winxmax;
	phd_PushMatrix();
	obj = &objects[item->object_number];

	if (lara.vehicle == NO_ITEM)
		S_PrintShadow(obj->shadow_size, GLaraShadowframe, item);

	if (tomb4.look_transparency)
	{
		if (input & IN_LOOK)
		{
			dx = lara_item->pos.x_pos - CamPos.x;
			dy = lara_item->pos.y_pos - CamPos.y - 512;
			dz = lara_item->pos.z_pos - CamPos.z;
			dist = phd_sqrt(SQUARE(dx) + SQUARE(dy) + SQUARE(dz));
			a = dist >> 2;

			if (a < 0)
				a = 0;

			if (a > 255)
				a = 255;

			GlobalAlpha = a << 24;
		}
		else
		{
			if (a < 255)
			{
				a += 8;

				if (a > 255)
					a = 255;
			}

			GlobalAlpha = a << 24;
		}
	}

	if (!mirror)
		CalculateObjectLightingLara();

	for (int i = 0; i < 15; i++) // Skin
	{
		mMXPtr->m00 = lara_matrices[i].m00;
		mMXPtr->m01 = lara_matrices[i].m01;
		mMXPtr->m02 = lara_matrices[i].m02;
		mMXPtr->m03 = lara_matrices[i].m03;
		mMXPtr->m10 = lara_matrices[i].m10;
		mMXPtr->m11 = lara_matrices[i].m11;
		mMXPtr->m12 = lara_matrices[i].m12;
		mMXPtr->m13 = lara_matrices[i].m13;
		mMXPtr->m20 = lara_matrices[i].m20;
		mMXPtr->m21 = lara_matrices[i].m21;
		mMXPtr->m22 = lara_matrices[i].m22;
		mMXPtr->m23 = lara_matrices[i].m23;

		if (LaraNodeUnderwater[i] != 0)
			bLaraUnderWater = i;
		else
			bLaraUnderWater = -1;

		phd_PutPolygons(lara.mesh_ptrs[lara_mesh_sweetness_table[i]], -1);	//no meshbits checks?

		for (int j = 0; j < 4; j++)
		{
			stash = (uchar)NodesToStashFromScratch[i][j];

			if (stash == 255)
				break;

			StashSkinVertices(stash);
		}
	}

	in_joints = 1;
	phd_PopMatrix();
	bLaraUnderWater = LaraNodeUnderwater[8] != 0 ? 8 : -1;
	DrawHair();
	phd_PushMatrix();
	obj = &objects[LARA_SKIN_JOINTS];
	meshpp = &meshes[obj->mesh_index];
	meshpp += 2;

	for (int i = 0; i < NUM_LARA_MESHES; i++) // Joints
	{
		if (NodesToStashToScratch[i][0] < 127) SkinVerticesToScratch(NodesToStashToScratch[i][0]);
		if (NodesToStashToScratch[i][1] < 127) SkinVerticesToScratch(NodesToStashToScratch[i][1]);

		if (LaraNodeUnderwater[lara_underwater_skin_sweetness_table[i]])
			bLaraUnderWater = lara_underwater_skin_sweetness_table[i];
		else
			bLaraUnderWater = -1;

		if (SkinUseMatrix[i][0] >= 255 || SkinUseMatrix[i][1] >= 255)
		{
			phd_PutPolygons(*meshpp, -1);
		}
		else if (SkinUseMatrix[i][0] < 254 || SkinUseMatrix[i][1] < 254)
		{
			mMXPtr->m00 = lara_matrices[SkinUseMatrix[i][1]].m00;
			mMXPtr->m01 = lara_matrices[SkinUseMatrix[i][1]].m01;
			mMXPtr->m02 = lara_matrices[SkinUseMatrix[i][1]].m02;
			mMXPtr->m03 = lara_matrices[SkinUseMatrix[i][1]].m03;
			mMXPtr->m10 = lara_matrices[SkinUseMatrix[i][1]].m10;
			mMXPtr->m11 = lara_matrices[SkinUseMatrix[i][1]].m11;
			mMXPtr->m12 = lara_matrices[SkinUseMatrix[i][1]].m12;
			mMXPtr->m13 = lara_matrices[SkinUseMatrix[i][1]].m13;
			mMXPtr->m20 = lara_matrices[SkinUseMatrix[i][1]].m20;
			mMXPtr->m21 = lara_matrices[SkinUseMatrix[i][1]].m21;
			mMXPtr->m22 = lara_matrices[SkinUseMatrix[i][1]].m22;
			mMXPtr->m23 = lara_matrices[SkinUseMatrix[i][1]].m23;
			phd_PushMatrix();
			v0.x = lara_matrices[SkinUseMatrix[i][0]].m01;
			v0.y = lara_matrices[SkinUseMatrix[i][0]].m11;
			v0.z = lara_matrices[SkinUseMatrix[i][0]].m21;
			v1.x = lara_matrices[SkinUseMatrix[i][1]].m01;
			v1.y = lara_matrices[SkinUseMatrix[i][1]].m11;
			v1.z = lara_matrices[SkinUseMatrix[i][1]].m21;
			xRot = long(32768.0F / (float)M_PI * acosf(v0.x * v1.x + v0.y * v1.y + v0.z * v1.z));
			phd_RotX(short(xRot >> 1));
			phd_PutPolygons(*meshpp, -1);
			phd_PopMatrix();
		}

		meshpp += 2;
	}

	in_joints = 0;
	bLaraUnderWater = (LaraNodeUnderwater[0] != 0) - 1;

	if (!(gfLevelFlags & GF_YOUNGLARA))
	{
		obj = &objects[lara.holster_l];
		meshpp = &meshes[obj->mesh_index];
		meshpp += 8;
		mMXPtr->m00 = lara_matrices[LM_LTHIGH].m00;
		mMXPtr->m01 = lara_matrices[LM_LTHIGH].m01;
		mMXPtr->m02 = lara_matrices[LM_LTHIGH].m02;
		mMXPtr->m03 = lara_matrices[LM_LTHIGH].m03;
		mMXPtr->m10 = lara_matrices[LM_LTHIGH].m10;
		mMXPtr->m11 = lara_matrices[LM_LTHIGH].m11;
		mMXPtr->m12 = lara_matrices[LM_LTHIGH].m12;
		mMXPtr->m13 = lara_matrices[LM_LTHIGH].m13;
		mMXPtr->m20 = lara_matrices[LM_LTHIGH].m20;
		mMXPtr->m21 = lara_matrices[LM_LTHIGH].m21;
		mMXPtr->m22 = lara_matrices[LM_LTHIGH].m22;
		mMXPtr->m23 = lara_matrices[LM_LTHIGH].m23;
		phd_PutPolygons(*meshpp, -1);

		obj = &objects[lara.holster_r];
		meshpp = &meshes[obj->mesh_index];
		meshpp += 16;
		mMXPtr->m00 = lara_matrices[LM_RTHIGH].m00;
		mMXPtr->m01 = lara_matrices[LM_RTHIGH].m01;
		mMXPtr->m02 = lara_matrices[LM_RTHIGH].m02;
		mMXPtr->m03 = lara_matrices[LM_RTHIGH].m03;
		mMXPtr->m10 = lara_matrices[LM_RTHIGH].m10;
		mMXPtr->m11 = lara_matrices[LM_RTHIGH].m11;
		mMXPtr->m12 = lara_matrices[LM_RTHIGH].m12;
		mMXPtr->m13 = lara_matrices[LM_RTHIGH].m13;
		mMXPtr->m20 = lara_matrices[LM_RTHIGH].m20;
		mMXPtr->m21 = lara_matrices[LM_RTHIGH].m21;
		mMXPtr->m22 = lara_matrices[LM_RTHIGH].m22;
		mMXPtr->m23 = lara_matrices[LM_RTHIGH].m23;
		phd_PutPolygons(*meshpp, -1);

		if (lara.back_gun)
		{
			phd_PushMatrix();
			mMXPtr->m00 = lara_matrices[LM_TORSO].m00;
			mMXPtr->m01 = lara_matrices[LM_TORSO].m01;
			mMXPtr->m02 = lara_matrices[LM_TORSO].m02;
			mMXPtr->m03 = lara_matrices[LM_TORSO].m03;
			mMXPtr->m10 = lara_matrices[LM_TORSO].m10;
			mMXPtr->m11 = lara_matrices[LM_TORSO].m11;
			mMXPtr->m12 = lara_matrices[LM_TORSO].m12;
			mMXPtr->m13 = lara_matrices[LM_TORSO].m13;
			mMXPtr->m20 = lara_matrices[LM_TORSO].m20;
			mMXPtr->m21 = lara_matrices[LM_TORSO].m21;
			mMXPtr->m22 = lara_matrices[LM_TORSO].m22;
			mMXPtr->m23 = lara_matrices[LM_TORSO].m23;
			obj = &objects[lara.back_gun];
			bone = &bones[obj->bone_index];
			meshpp = &meshes[obj->mesh_index];
			meshpp += 28;
			phd_TranslateRel(bone[53], bone[54], bone[55]);
			rot = objects[lara.back_gun].frame_base + 9;
			gar_RotYXZsuperpack(&rot, LM_HEAD);
			phd_PutPolygons(*meshpp, -1);
			phd_PopMatrix();
		}
	}

	bLaraUnderWater = 0;
	phd_PopMatrix();
	phd_top = top;
	phd_bottom = bottom;
	phd_left = left;
	phd_right = right;
	GlobalAlpha = 0xFF000000;
}

void GetLaraJointPos(PHD_VECTOR* pos, long node)
{
	phd_PushMatrix();
	mMXPtr->m00 = lara_joint_matrices[node].m00;
	mMXPtr->m01 = lara_joint_matrices[node].m01;
	mMXPtr->m02 = lara_joint_matrices[node].m02;
	mMXPtr->m03 = lara_joint_matrices[node].m03;
	mMXPtr->m10 = lara_joint_matrices[node].m10;
	mMXPtr->m11 = lara_joint_matrices[node].m11;
	mMXPtr->m12 = lara_joint_matrices[node].m12;
	mMXPtr->m13 = lara_joint_matrices[node].m13;
	mMXPtr->m20 = lara_joint_matrices[node].m20;
	mMXPtr->m21 = lara_joint_matrices[node].m21;
	mMXPtr->m22 = lara_joint_matrices[node].m22;
	mMXPtr->m23 = lara_joint_matrices[node].m23;
	phd_TranslateRel(pos->x, pos->y, pos->z);
	pos->x = (long)mMXPtr->m03;
	pos->y = (long)mMXPtr->m13;
	pos->z = (long)mMXPtr->m23;
	pos->x += lara_item->pos.x_pos;
	pos->y += lara_item->pos.y_pos;
	pos->z += lara_item->pos.z_pos;
	phd_PopMatrix();
}

void SetLaraUnderwaterNodes()
{
	PHD_VECTOR pos{};
	long bit;
	short room_num;

	pos.x = lara_item->pos.x_pos;
	pos.y = lara_item->pos.y_pos;
	pos.z = lara_item->pos.z_pos;
	room_num = lara_item->room_number;
	GetFloor(pos.x, pos.y, pos.z, &room_num);
	bLaraInWater = room[room_num].flags & ROOM_UNDERWATER ? 1 : 0;
	bit = 0;

	for (int i = 14; i >= 0; i--)
	{
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		GetLaraJointPos(&pos, i);

		if (lara_mesh_sweetness_table[i] == 7)
			pos.y -= 120;

		if (lara_mesh_sweetness_table[i] == 14)
			pos.y -= 60;

		room_num = lara_item->room_number;
		GetFloor(pos.x, pos.y, pos.z, &room_num);
		LaraNodeUnderwater[i] = room[room_num].flags & ROOM_UNDERWATER;

		if (room[room_num].flags & ROOM_UNDERWATER)
		{
			lara.wet[i] = 252;

			if (!(bit & 1))
			{
				LaraNodeAmbient[1] = room[room_num].ambient;
				bit |= 1;
			}
		}
		else if (!(bit & 2))
		{
			LaraNodeAmbient[0] = room[room_num].ambient;
			bit |= 2;
		}
	}
}

void Rich_CalcLaraMatrices_Normal(short* frame, long* bone, long flag)
{
	PHD_VECTOR vec;
	MATRIX_FLT* matrix;
	short* rot;
	short* rot2;
	short gun;

	if (flag == 1)
		matrix = lara_joint_matrices;
	else
		matrix = lara_matrices;

	phd_PushMatrix();

	if (!flag || flag == 2)
		phd_TranslateAbs(lara_item->pos.x_pos, lara_item->pos.y_pos, lara_item->pos.z_pos);
	else
		phd_SetTrans(0, 0, 0);

	phd_RotYXZ(lara_item->pos.y_rot, lara_item->pos.x_rot, lara_item->pos.z_rot);
	if (flag == 2)
	{
		vec.x = -16384;
		vec.y = -16384;
		vec.z = -16384;
		ScaleCurrentMatrix(&vec);
	}

	phd_PushMatrix();
	rot = &frame[9];
	phd_TranslateRel(frame[6], frame[7], frame[8]);
	gar_RotYXZsuperpack(&rot, 0);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

	phd_PushMatrix();
	phd_TranslateRel(bone[1], bone[2], bone[3]);
	gar_RotYXZsuperpack(&rot, 0);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

	phd_TranslateRel(bone[5], bone[6], bone[7]);
	gar_RotYXZsuperpack(&rot, 0);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

	phd_TranslateRel(bone[9], bone[10], bone[11]);
	gar_RotYXZsuperpack(&rot, 0);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_PushMatrix();
	phd_TranslateRel(bone[13], bone[14], bone[15]);
	gar_RotYXZsuperpack(&rot, 0);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

	phd_TranslateRel(bone[17], bone[18], bone[19]);
	gar_RotYXZsuperpack(&rot, 0);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

	phd_TranslateRel(bone[21], bone[22], bone[23]);
	gar_RotYXZsuperpack(&rot, 0);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_TranslateRel(bone[25], bone[26], bone[27]);
	gar_RotYXZsuperpack(&rot, 0);
	phd_RotYXZ(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

	phd_PushMatrix();
	phd_TranslateRel(bone[53], bone[54], bone[55]);
	rot2 = rot;
	gar_RotYXZsuperpack(&rot2, 6);
	phd_RotYXZ(lara.head_y_rot, lara.head_x_rot, lara.head_z_rot);
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	gun = WEAPON_NONE;

	if (lara.gun_status == LG_READY || lara.gun_status == LG_FLARE || lara.gun_status == LG_DRAW_GUNS || lara.gun_status == LG_UNDRAW_GUNS)
		gun = lara.gun_type;

	switch (gun)
	{
	case WEAPON_NONE:
	case WEAPON_FLARE:
	case WEAPON_TORCH:
		phd_PushMatrix();
		phd_TranslateRel(bone[29], bone[30], bone[31]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_PushMatrix();
		phd_TranslateRel(bone[41], bone[42], bone[43]);

		if (lara.flare_control_left)
		{
			rot = &lara.left_arm.frame_base[(anims[lara.left_arm.anim_number].interpolation >> 8) * (lara.left_arm.frame_number - anims[lara.left_arm.anim_number].frame_base) + 9];
			gar_RotYXZsuperpack(&rot, 11);
		}
		else
			gar_RotYXZsuperpack(&rot, 0);

		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		break;

	case WEAPON_PISTOLS:
	case WEAPON_UZI:
		phd_PushMatrix();
		phd_TranslateRel(bone[29], bone[30], bone[31]);

		mMXPtr->m00 = mMXPtr[-2].m00;
		mMXPtr->m01 = mMXPtr[-2].m01;
		mMXPtr->m02 = mMXPtr[-2].m02;
		mMXPtr->m10 = mMXPtr[-2].m10;
		mMXPtr->m11 = mMXPtr[-2].m11;
		mMXPtr->m12 = mMXPtr[-2].m12;
		mMXPtr->m20 = mMXPtr[-2].m20;
		mMXPtr->m21 = mMXPtr[-2].m21;
		mMXPtr->m22 = mMXPtr[-2].m22;

		phd_RotYXZ(lara.right_arm.y_rot, lara.right_arm.x_rot, lara.right_arm.z_rot);
		rot = &lara.right_arm.frame_base[(anims[lara.right_arm.anim_number].interpolation >> 8) * (lara.right_arm.frame_number - anims[lara.right_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 8);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_PushMatrix();
		phd_TranslateRel(bone[41], bone[42], bone[43]);

		mMXPtr->m00 = mMXPtr[-2].m00;
		mMXPtr->m01 = mMXPtr[-2].m01;
		mMXPtr->m02 = mMXPtr[-2].m02;
		mMXPtr->m10 = mMXPtr[-2].m10;
		mMXPtr->m11 = mMXPtr[-2].m11;
		mMXPtr->m12 = mMXPtr[-2].m12;
		mMXPtr->m20 = mMXPtr[-2].m20;
		mMXPtr->m21 = mMXPtr[-2].m21;
		mMXPtr->m22 = mMXPtr[-2].m22;

		phd_RotYXZ(lara.left_arm.y_rot, lara.left_arm.x_rot, lara.left_arm.z_rot);
		rot = &lara.left_arm.frame_base[(anims[lara.left_arm.anim_number].interpolation >> 8) * (lara.left_arm.frame_number - anims[lara.left_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 11);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		break;

	case WEAPON_REVOLVER:
		phd_PushMatrix();
		phd_TranslateRel(bone[29], bone[30], bone[31]);

		mMXPtr->m00 = mMXPtr[-2].m00;
		mMXPtr->m01 = mMXPtr[-2].m01;
		mMXPtr->m02 = mMXPtr[-2].m02;
		mMXPtr->m10 = mMXPtr[-2].m10;
		mMXPtr->m11 = mMXPtr[-2].m11;
		mMXPtr->m12 = mMXPtr[-2].m12;
		mMXPtr->m20 = mMXPtr[-2].m20;
		mMXPtr->m21 = mMXPtr[-2].m21;
		mMXPtr->m22 = mMXPtr[-2].m22;

		phd_RotYXZ(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
		rot = &lara.right_arm.frame_base[(anims[lara.right_arm.anim_number].interpolation >> 8) *
			(lara.right_arm.frame_number - anims[lara.right_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 8);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_PushMatrix();
		phd_TranslateRel(bone[41], bone[42], bone[43]);

		mMXPtr->m00 = mMXPtr[-2].m00;
		mMXPtr->m01 = mMXPtr[-2].m01;
		mMXPtr->m02 = mMXPtr[-2].m02;
		mMXPtr->m10 = mMXPtr[-2].m10;
		mMXPtr->m11 = mMXPtr[-2].m11;
		mMXPtr->m12 = mMXPtr[-2].m12;
		mMXPtr->m20 = mMXPtr[-2].m20;
		mMXPtr->m21 = mMXPtr[-2].m21;
		mMXPtr->m22 = mMXPtr[-2].m22;

		phd_RotYXZ(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
		rot = &lara.left_arm.frame_base[(anims[lara.left_arm.anim_number].interpolation >> 8) * (lara.left_arm.frame_number - anims[lara.left_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 11);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		break;

	case WEAPON_SHOTGUN:
	case WEAPON_GRENADE:
	case WEAPON_CROSSBOW:
		phd_PushMatrix();
		phd_TranslateRel(bone[29], bone[30], bone[31]);
		rot = &lara.right_arm.frame_base[lara.right_arm.frame_number * (anims[lara.right_arm.anim_number].interpolation >> 8) + 9];
		gar_RotYXZsuperpack(&rot, 8);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_PushMatrix();
		phd_TranslateRel(bone[41], bone[42], bone[43]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		break;
	}

	phd_PopMatrix();
	phd_PopMatrix();
	phd_PopMatrix();
	GLaraShadowframe = frame;
}

void Rich_CalcLaraMatrices_Interpolated(short* frame1, short* frame2, long frac, long rate, long* bone, long flag)
{
	PHD_VECTOR vec{};
	MATRIX_FLT* matrix;
	MATRIX_FLT* arms;
	short* rot;
	short* rot2;
	short* rotcopy;
	short* rot2copy;
	short gun;

	if (flag == 1)
		matrix = lara_joint_matrices;
	else
		matrix = lara_matrices;

	phd_PushMatrix();

	if (flag == 0 || flag == 2)
		phd_TranslateAbs(lara_item->pos.x_pos, lara_item->pos.y_pos, lara_item->pos.z_pos);
	else
		phd_SetTrans(0, 0, 0);

	phd_RotYXZ(lara_item->pos.y_rot, lara_item->pos.x_rot, lara_item->pos.z_rot);
	if (flag == 2)
	{
		vec.x = -16384;
		vec.y = -16384;
		vec.z = -16384;
		ScaleCurrentMatrix(&vec);
	}

	arms = mMXPtr;
	phd_PushMatrix();
	rot = frame1 + 9;
	rot2 = frame2 + 9;
	InitInterpolate(frac, rate);
	phd_TranslateRel_ID(frame1[6], frame1[7], frame1[8], frame2[6], frame2[7], frame2[8]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);

	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_PushMatrix_I();
	phd_TranslateRel_I(bone[1], bone[2], bone[3]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_TranslateRel_I(bone[5], bone[6], bone[7]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_TranslateRel_I(bone[9], bone[10], bone[11]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();
	phd_PopMatrix_I();

	phd_PushMatrix_I();
	phd_TranslateRel_I(bone[13], bone[14], bone[15]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_TranslateRel_I(bone[17], bone[18], bone[19]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_TranslateRel_I(bone[21], bone[22], bone[23]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();
	phd_PopMatrix_I();

	phd_TranslateRel_I(bone[25], bone[26], bone[27]);
	gar_RotYXZsuperpack_I(&rot, &rot2, 0);
	phd_RotYXZ_I(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();

	phd_PushMatrix_I();
	phd_TranslateRel_I(bone[53], bone[54], bone[55]);
	rotcopy = rot;
	rot2copy = rot2;
	gar_RotYXZsuperpack_I(&rotcopy, &rot2copy, 6);
	phd_RotYXZ_I(lara.head_y_rot, lara.head_x_rot, lara.head_z_rot);
	phd_PushMatrix();
	mInterpolateMatrix();
	memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
	phd_PopMatrix();
	phd_PopMatrix_I();

	if (lara.gun_status == LG_READY || lara.gun_status == LG_FLARE || lara.gun_status == LG_DRAW_GUNS || lara.gun_status == LG_UNDRAW_GUNS)
		gun = lara.gun_type;
	else
		gun = WEAPON_NONE;

	switch (gun)
	{
	case WEAPON_NONE:
	case WEAPON_FLARE:
	case WEAPON_TORCH:
		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[29], bone[30], bone[31]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();
		phd_PopMatrix_I();

		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[41], bone[42], bone[43]);

		if (lara.flare_control_left)
		{
			rot2 = &lara.left_arm.frame_base[(anims[lara.left_arm.anim_number].interpolation >> 8) * (lara.left_arm.frame_number - anims[lara.left_arm.anim_number].frame_base) + 9];
			rot = rot2;
			gar_RotYXZsuperpack_I(&rot, &rot2, 11);
		}
		else
			gar_RotYXZsuperpack_I(&rot, &rot2, 0);

		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();
		break;

	case WEAPON_PISTOLS:
	case WEAPON_UZI:
		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[29], bone[30], bone[31]);
		mInterpolateArmMatrix(arms);
		phd_RotYXZ(lara.right_arm.y_rot, lara.right_arm.x_rot, lara.right_arm.z_rot);
		rot = &lara.right_arm.frame_base[(anims[lara.right_arm.anim_number].interpolation >> 8) * (lara.right_arm.frame_number - anims[lara.right_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 8);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		
		phd_TranslateRel(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		
		phd_TranslateRel(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix_I();

		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[41], bone[42], bone[43]);
		mInterpolateArmMatrix(arms);
		phd_RotYXZ(lara.left_arm.y_rot, lara.left_arm.x_rot, lara.left_arm.z_rot);
		rot = &lara.left_arm.frame_base[(anims[lara.left_arm.anim_number].interpolation >> 8) * (lara.left_arm.frame_number - anims[lara.left_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 11);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		break;

	case WEAPON_REVOLVER:
		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[29], bone[30], bone[31]);
		mInterpolateArmMatrix(arms);
		phd_RotYXZ_I(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
		rot = &lara.right_arm.frame_base[(anims[lara.right_arm.anim_number].interpolation >> 8) * (lara.right_arm.frame_number - anims[lara.right_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 8);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix_I();

		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[41], bone[42], bone[43]);
		mInterpolateArmMatrix(arms);
		phd_RotYXZ_I(lara.torso_y_rot, lara.torso_x_rot, lara.torso_z_rot);
		rot = &lara.left_arm.frame_base[(anims[lara.left_arm.anim_number].interpolation >> 8) * (lara.left_arm.frame_number - anims[lara.left_arm.anim_number].frame_base) + 9];
		gar_RotYXZsuperpack(&rot, 11);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));

		phd_TranslateRel(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack(&rot, 0);
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		break;

	case WEAPON_SHOTGUN:
	case WEAPON_GRENADE:
	case WEAPON_CROSSBOW:
		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[29], bone[30], bone[31]);
		rot2 = &lara.right_arm.frame_base[lara.right_arm.frame_number * (anims[lara.right_arm.anim_number].interpolation >> 8) + 9];
		rot = rot2;
		gar_RotYXZsuperpack_I(&rot, &rot2, 8);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[33], bone[34], bone[35]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[37], bone[38], bone[39]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();
		phd_PopMatrix_I();

		phd_PushMatrix_I();
		phd_TranslateRel_I(bone[41], bone[42], bone[43]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[45], bone[46], bone[47]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();

		phd_TranslateRel_I(bone[49], bone[50], bone[51]);
		gar_RotYXZsuperpack_I(&rot, &rot2, 0);
		phd_PushMatrix();
		mInterpolateMatrix();
		memcpy(matrix++, mMXPtr, sizeof(MATRIX_FLT));
		phd_PopMatrix();
		break;
	}

	phd_PopMatrix();
	phd_PopMatrix();
	phd_PopMatrix();
}

void CalcLaraMatrices(long flag)
{
	long* bone;
	short* frame;
	short* frmptr[2];
	long rate, frac;
	short spaz;

	bone = &bones[objects[lara_item->object_number].bone_index];
	frac = GetFrames(lara_item, frmptr, &rate);

	if (lara.hit_direction < 0)
	{
		if (frac)
		{
			GLaraShadowframe = GetBoundsAccurate(lara_item);
			Rich_CalcLaraMatrices_Interpolated(frmptr[0], frmptr[1], frac, rate, bone, flag);
			return;
		}
	}

	if (lara.hit_direction < 0)
		frame = *frmptr;
	else
	{
		if (!lara.hit_direction)
			spaz = lara.IsDucked ? ANIM_SPAZ_DUCKF : ANIM_SPAZ_FORWARD;
		else if (lara.hit_direction == 1)
			spaz = lara.IsDucked ? ANIM_SPAZ_DUCKR : ANIM_SPAZ_RIGHT;
		else if (lara.hit_direction == 2)
			spaz = lara.IsDucked ? ANIM_SPAZ_DUCKB : ANIM_SPAZ_BACK;
		else
			spaz = lara.IsDucked ? ANIM_SPAZ_DUCKL : ANIM_SPAZ_LEFT;

		frame = &anims[spaz].frame_ptr[lara.hit_frame * (anims[spaz].interpolation >> 8)];
	}

	Rich_CalcLaraMatrices_Normal(frame, bone, flag);
}
