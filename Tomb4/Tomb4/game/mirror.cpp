#include "pch.h"
#include "mirror.h"
#include "delstuff.h"
#include "function_table.h"
#include "tomb4fx.h"
#include "3dmath.h"
#include "hair.h"
#include "lara.h"
#include "gameflow.h"

void Draw_Mirror_Lara()
{
	HAIR_STRUCT* hair = &hairs[0][0];
	GUNSHELL_STRUCT* gunshell;
	long zplane = gfMirrorZPlane << 1;

	for (int i = 0; i < 6; i++, hair++)
	{
		hair->pos.z_pos = zplane - hair->pos.z_pos;
		hair->pos.y_rot = 0x8000 - hair->pos.y_rot;
	}

	for (int i = 0; i < 24; i++)
	{
		gunshell = &Gunshells[i];
		if (gunshell->counter > 0)
			gunshell->pos.z_pos = zplane - gunshell->pos.z_pos;
	}

	lara_item->pos.z_pos = zplane - lara_item->pos.z_pos;
	lara_item->pos.x_rot = -lara_item->pos.x_rot;
	lara_item->pos.y_rot = -lara_item->pos.y_rot;
	lara_item->pos.z_rot += 0x8000;
	CalcLaraMatrices(2);
	SetCullCW();
	DrawLara(lara_item, 1);
	DrawGunshells();
	SetCullCCW();
	lara_item->pos.z_pos = zplane - lara_item->pos.z_pos;
	lara_item->pos.x_rot = -lara_item->pos.x_rot;
	lara_item->pos.y_rot = -lara_item->pos.y_rot;
	lara_item->pos.z_rot += 0x8000;
	phd_PushMatrix();

	if (lara.right_arm.flash_gun)
	{
		mMXPtr->m00 = lara_matrices[LM_RHAND + 1].m00;
		mMXPtr->m01 = lara_matrices[LM_RHAND + 1].m01;
		mMXPtr->m02 = lara_matrices[LM_RHAND + 1].m02;
		mMXPtr->m03 = lara_matrices[LM_RHAND + 1].m03;
		mMXPtr->m10 = lara_matrices[LM_RHAND + 1].m10;
		mMXPtr->m11 = lara_matrices[LM_RHAND + 1].m11;
		mMXPtr->m12 = lara_matrices[LM_RHAND + 1].m12;
		mMXPtr->m13 = lara_matrices[LM_RHAND + 1].m13;
		mMXPtr->m20 = lara_matrices[LM_RHAND + 1].m20;
		mMXPtr->m21 = lara_matrices[LM_RHAND + 1].m21;
		mMXPtr->m22 = lara_matrices[LM_RHAND + 1].m22;
		mMXPtr->m23 = lara_matrices[LM_RHAND + 1].m23;
		SetGunFlash(lara.gun_type);
	}

	if (lara.left_arm.flash_gun)
	{
		mMXPtr->m00 = lara_matrices[LM_LHAND + 1].m00;
		mMXPtr->m01 = lara_matrices[LM_LHAND + 1].m01;
		mMXPtr->m02 = lara_matrices[LM_LHAND + 1].m02;
		mMXPtr->m03 = lara_matrices[LM_LHAND + 1].m03;
		mMXPtr->m10 = lara_matrices[LM_LHAND + 1].m10;
		mMXPtr->m11 = lara_matrices[LM_LHAND + 1].m11;
		mMXPtr->m12 = lara_matrices[LM_LHAND + 1].m12;
		mMXPtr->m13 = lara_matrices[LM_LHAND + 1].m13;
		mMXPtr->m20 = lara_matrices[LM_LHAND + 1].m20;
		mMXPtr->m21 = lara_matrices[LM_LHAND + 1].m21;
		mMXPtr->m22 = lara_matrices[LM_LHAND + 1].m22;
		mMXPtr->m23 = lara_matrices[LM_LHAND + 1].m23;
		SetGunFlash(lara.gun_type);
	}

	phd_PopMatrix();

	for (int i = 0; i < 24; i++)
	{
		gunshell = &Gunshells[i];
		if (gunshell->counter > 0)
			gunshell->pos.z_pos = zplane - gunshell->pos.z_pos;
	}

	hair = &hairs[0][0];
	for (int i = 0; i < 6; i++, hair++)
	{
		hair->pos.z_pos = zplane - hair->pos.z_pos;
		hair->pos.y_rot = 0x8000 - hair->pos.y_rot;
	}
}
