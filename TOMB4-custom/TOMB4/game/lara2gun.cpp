#include "pch.h"
#include "lara2gun.h"
#include "objects.h"
#include "larafire.h"
#include "sound.h"
#include "delstuff.h"
#include "tomb4fx.h"
#include "specific/function_stubs.h"
#include "effect2.h"
#include "camera.h"
#include "specific/input.h"
#include "lara.h"
#include "savegame.h"
#include "gameflow.h"
#include "control.h"

static PISTOL_DEF PistolTable[NUM_WEAPONS] =
{
	{ LARA, 0, 0, 0, 0 }, // NONE
	{ FLARE_ANIM, 0, 0, 0, 0 }, // FLARE
	{ TORCH_ANIM, 0, 0, 0, 0 }, // TORCH
	{ PISTOLS_ANIM, 4, 5, 13, 24 }, // PISTOLS
	{ PISTOLS_ANIM, 4, 5, 13, 24 }, // UZIS
	{ PISTOLS_ANIM, 4, 5, 13, 24 }, // MAGNUMS
	{ PISTOLS_ANIM, 4, 5, 13, 24 }, // AUTOPISTOLS
	{ DESERTEAGLE_ANIM, 7, 8, 15, 29 }, // DESERTEAGLE
	{ REVOLVER_ANIM, 7, 8, 15, 29 }, // REVOLVER
	{ SHOTGUN_ANIM, 0, 0, 0, 0 }, // SHOTGUN
	{ M16_ANIM, 0, 0, 0, 0 }, // M16
	{ MP5_ANIM, 0, 0, 0, 0 }, // MP5
	{ CROSSBOW_ANIM, 0, 0, 0, 0 }, // CROSSBOW
	{ HARPOON_GUN_ANIM, 0, 0, 0, 0 }, // HARPOONGUN
	{ GRENADE_GUN_ANIM, 0, 0, 0, 0 }, // GRENADEGUN
	{ ROCKET_GUN_ANIM, 0, 0, 0, 0 }, // ROCKETGUN
	{ GRAPPLING_GUN_ANIM, 0, 0, 0, 0 }, // GRAPPLINGGUN
	{ 0, 0, 0, 0, 0 }, // SNOWMOBILEGUN
};

void undraw_pistol_mesh_left(long weapon_type)
{
	if (weapon_type != WEAPON_REVOLVER && weapon_type != WEAPON_DESERTEAGLE)
	{
		lara.mesh_ptrs[LM_LHAND] = meshes[objects[LARA].mesh_index + LM_LHAND * 2];
		lara.holster_left = (short)GetWeaponHolsterObject(weapon_type);
	}
}

void undraw_pistol_mesh_right(long weapon_type)
{
	lara.mesh_ptrs[LM_RHAND] = meshes[objects[LARA].mesh_index + LM_RHAND * 2];
	lara.holster_right = (short)GetWeaponHolsterObject(weapon_type);
}

static void set_arm_info(LARA_ARM* arm, long frame)
{
	PISTOL_DEF* p;
	long anim_base;

	p = &PistolTable[lara.gun_type];
	anim_base = objects[p->ObjectNum].anim_index;

	if (frame >= p->Draw1Anim)
	{
		if (frame >= p->Draw2Anim)
		{
			if (frame < p->RecoilAnim)
				anim_base += 2;
			else
				anim_base += 3;
		}
		else
			anim_base++;
	}

	arm->anim_number = (short)anim_base;
	arm->frame_number = (short)frame;
	arm->frame_base = anims[anim_base].frame_ptr;
}

void ready_pistols(long weapon_type)
{
	lara.gun_status = LG_READY;
	lara.left_arm.x_rot = 0;
	lara.left_arm.y_rot = 0;
	lara.left_arm.z_rot = 0;
	lara.right_arm.x_rot = 0;
	lara.right_arm.y_rot = 0;
	lara.right_arm.z_rot = 0;
	lara.right_arm.frame_number = 0;
	lara.left_arm.frame_number = 0;
	lara.target = 0;
	lara.right_arm.lock = 0;
	lara.left_arm.lock = 0;
	lara.right_arm.frame_base = objects[GetWeaponAnimObject(weapon_type)].frame_base;
	lara.left_arm.frame_base = lara.right_arm.frame_base;
}

void draw_pistol_meshes(long weapon_type)
{
	long mesh_index = objects[GetWeaponMeshObject(weapon_type)].mesh_index;

	lara.mesh_ptrs[LM_RHAND] = meshes[mesh_index + LM_RHAND * 2];
	lara.holster_right = LARA_HOLSTERS;

	if (weapon_type != WEAPON_REVOLVER && weapon_type != WEAPON_DESERTEAGLE)
	{
		lara.mesh_ptrs[LM_LHAND] = meshes[mesh_index + LM_LHAND * 2];
		lara.holster_left = LARA_HOLSTERS;
	}
}

void draw_pistols(long weapon_type)
{
	PISTOL_DEF* p;
	short ani;

	ani = lara.left_arm.frame_number + 1;
	p = &PistolTable[lara.gun_type];

	if (ani < p->Draw1Anim || ani > p->RecoilAnim - 1)
		ani = p->Draw1Anim;
	else if (ani == p->Draw2Anim)
	{
		draw_pistol_meshes(weapon_type);
		Sound.PlayEffect(SFX_LARA_DRAW, &lara_item->pos);
	}
	else if (ani == p->RecoilAnim - 1)
	{
		ready_pistols(weapon_type);
		ani = 0;
	}

	set_arm_info(&lara.right_arm, ani);
	set_arm_info(&lara.left_arm, ani);
}

void undraw_pistols(long weapon_type)
{
	PISTOL_DEF* p;
	short anil, anir;

	p = &PistolTable[lara.gun_type];
	anil = lara.left_arm.frame_number;

	if (lara.left_arm.frame_number >= p->RecoilAnim)
		anil = p->Draw1Anim2;
	else if (lara.left_arm.frame_number > 0 && lara.left_arm.frame_number < p->Draw1Anim)
	{
		lara.left_arm.x_rot -= lara.left_arm.x_rot / anil;
		lara.left_arm.y_rot -= lara.left_arm.y_rot / anil;
		anil--;
	}
	else if (!lara.left_arm.frame_number)
	{
		lara.left_arm.x_rot = 0;
		lara.left_arm.y_rot = 0;
		lara.left_arm.z_rot = 0;
		anil = p->RecoilAnim - 1;
	}
	else if (lara.left_arm.frame_number > p->Draw1Anim)
	{
		anil--;

		if (anil == p->Draw2Anim - 1)
		{
			undraw_pistol_mesh_left(weapon_type);
			Sound.PlayEffect(SFX_LARA_HOLSTER, &lara_item->pos);
		}
	}

	set_arm_info(&lara.left_arm, anil);
	anir = lara.right_arm.frame_number;

	if (lara.right_arm.frame_number >= p->RecoilAnim)
		anir = p->Draw1Anim2;
	else if (lara.right_arm.frame_number > 0 && lara.right_arm.frame_number < p->Draw1Anim)
	{
		lara.right_arm.x_rot -= lara.right_arm.x_rot / anir;
		lara.right_arm.y_rot -= lara.right_arm.y_rot / anir;
		anir--;
	}
	else if (!lara.right_arm.frame_number)
	{
		lara.right_arm.z_rot = 0;
		lara.right_arm.y_rot = 0;
		lara.right_arm.x_rot = 0;
		anir = p->RecoilAnim - 1;
	}
	else if (lara.right_arm.frame_number > p->Draw1Anim)
	{
		anir--;

		if (anir == p->Draw2Anim - 1)
		{
			undraw_pistol_mesh_right(weapon_type);
			Sound.PlayEffect(SFX_LARA_HOLSTER, &lara_item->pos);
		}
	}

	set_arm_info(&lara.right_arm, anir);

	if (anil == p->Draw1Anim && anir == p->Draw1Anim)
	{
		lara.gun_status = LG_NO_ARMS;
		lara.left_arm.frame_number = 0;
		lara.right_arm.frame_number = 0;
		lara.target = 0;
		lara.right_arm.lock = 0;
		lara.left_arm.lock = 0;
	}

	if (!(input & IN_LOOK))
	{
		lara.head_x_rot = (lara.left_arm.x_rot + lara.right_arm.x_rot) >> 2;
		lara.torso_x_rot = lara.head_x_rot;
		lara.head_y_rot = (lara.left_arm.y_rot + lara.right_arm.y_rot) >> 2;
		lara.torso_y_rot = lara.head_y_rot;
	}
}

void AnimatePistols(long weapon_type)
{
	PISTOL_DEF* p;
	WEAPON_INFO* winfo;
	PHD_VECTOR pos;
	static long uzi_left;
	static long uzi_right;
	short angles[2];
	short anil, anir, sound_already = 0;

	if (lara_item->mesh_bits)
	{
		if (SmokeCountL)
		{
			switch (SmokeWeapon)
			{
			case WEAPON_PISTOLS:
			case WEAPON_MAGNUMS:
			case WEAPON_AUTOPISTOLS:
				pos.x = 4;
				pos.y = 128;
				pos.z = 40;
				break;

			case WEAPON_REVOLVER:
			case WEAPON_DESERTEAGLE:
				pos.x = 16;
				pos.y = 160;
				pos.z = 56;
				break;

			case WEAPON_UZI:
				pos.x = 8;
				pos.y = 140;
				pos.z = 48;
				break;
			}

			GetLaraJointPos(&pos, 14);
			TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, SmokeWeapon, SmokeCountL);
		}

		if (SmokeCountR)
		{
			switch (SmokeWeapon)
			{
			case WEAPON_PISTOLS:
			case WEAPON_MAGNUMS:
			case WEAPON_AUTOPISTOLS:
				pos.x = -16;
				pos.y = 128;
				pos.z = 40;
				break;

			case WEAPON_REVOLVER:
			case WEAPON_DESERTEAGLE:
				pos.x = -32;
				pos.y = 160;
				pos.z = 56;
				break;

			case WEAPON_UZI:
				pos.x = -16;
				pos.y = 140;
				pos.z = 48;
				break;
			}

			GetLaraJointPos(&pos, 11);
			TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, SmokeWeapon, SmokeCountR);
		}
	}

	p = &PistolTable[lara.gun_type];
	winfo = &weapons[weapon_type];
	anir = lara.right_arm.frame_number;

	if (lara.right_arm.lock || input & IN_ACTION && !lara.target)
	{
		if (lara.right_arm.frame_number >= 0 && lara.right_arm.frame_number < p->Draw1Anim2)
			anir++;
		else if (lara.right_arm.frame_number == p->Draw1Anim2)
		{
			if (input & IN_ACTION)
			{
				if (weapon_type != WEAPON_REVOLVER && weapon_type != WEAPON_DESERTEAGLE)
				{
					angles[0] = lara.right_arm.y_rot + lara_item->pos.y_rot;
					angles[1] = lara.right_arm.x_rot;

					if (FireWeapon(weapon_type, lara.target, lara_item, angles))
					{
						SmokeCountR = 28;
						SmokeWeapon = weapon_type;
						TriggerGunShell(1, GUNSHELL, weapon_type);
						lara.right_arm.flash_gun = winfo->flash_time;
						Sound.PlayEffect(SFX_EXPLOSION1, &lara_item->pos);
						Sound.PlayEffect(winfo->sample_num, &lara_item->pos);
						sound_already = 1;
						savegame.Game.AmmoUsed++;
					}
				}

				anir = p->RecoilAnim;
			}
		}
		else if (lara.right_arm.frame_number >= p->RecoilAnim)
		{
			anir++;
			if (anir == p->RecoilAnim + winfo->recoil_frame)
				anir = p->Draw1Anim2;
		}
	}
	else
	{
		if (lara.right_arm.frame_number >= p->RecoilAnim)
			anir = p->Draw1Anim2;
		else if (lara.right_arm.frame_number > 0 && lara.right_arm.frame_number <= p->Draw1Anim2)
			anir--;
	}

	set_arm_info(&lara.right_arm, anir);
	anil = lara.left_arm.frame_number;

	if (lara.left_arm.lock || input & IN_ACTION && !lara.target)
	{
		if (lara.left_arm.frame_number >= 0 && lara.left_arm.frame_number < p->Draw1Anim2)
			anil++;
		else if (lara.left_arm.frame_number == p->Draw1Anim2)
		{
			if (input & IN_ACTION)
			{
				angles[0] = lara.left_arm.y_rot + lara_item->pos.y_rot;
				angles[1] = lara.left_arm.x_rot;

				if (FireWeapon(weapon_type, lara.target, lara_item, angles))
				{
					switch (weapon_type)
					{
					case WEAPON_REVOLVER:
						SmokeCountR = 28;
						SmokeWeapon = WEAPON_REVOLVER;
						lara.right_arm.flash_gun = winfo->flash_time;
						break;
					case WEAPON_DESERTEAGLE:
						SmokeCountR = 28;
						SmokeWeapon = WEAPON_DESERTEAGLE;
						lara.right_arm.flash_gun = winfo->flash_time;
						break;
					default:
						SmokeCountL = 28;
						SmokeWeapon = weapon_type;
						TriggerGunShell(0, GUNSHELL, weapon_type);
						lara.left_arm.flash_gun = winfo->flash_time;
						break;
					}

					if (!sound_already)
					{
						Sound.PlayEffect(SFX_EXPLOSION1, &lara_item->pos);
						Sound.PlayEffect(winfo->sample_num, &lara_item->pos, SFXO_LAND);
					}

					savegame.Game.AmmoUsed++;
				}

				anil = p->RecoilAnim;
			}
		}
		else if (lara.left_arm.frame_number >= p->RecoilAnim)
		{
			anil++;
			if (anil == p->RecoilAnim + winfo->recoil_frame)
				anil = p->Draw1Anim2;
		}
	}
	else
	{
		if (lara.left_arm.frame_number >= p->RecoilAnim)
			anil = p->Draw1Anim2;
		else if (lara.left_arm.frame_number > 0 && lara.left_arm.frame_number <= p->Draw1Anim2)
			anil--;
	}

	set_arm_info(&lara.left_arm, anil);
}

void PistolHandler(long weapon_type)
{
	WEAPON_INFO* winfo;
	PHD_VECTOR pos;
	long r, g, b;

	winfo = &weapons[weapon_type];
	LaraGetNewTarget(winfo);

	if (input & IN_ACTION)
		LaraTargetInfo(winfo);

	AimWeapon(winfo, &lara.left_arm);
	AimWeapon(winfo, &lara.right_arm);

	if (lara.left_arm.lock && lara.right_arm.lock)
	{
		lara.torso_y_rot = (lara.left_arm.y_rot + lara.right_arm.y_rot) >> 2;
		lara.torso_x_rot = (lara.left_arm.x_rot + lara.right_arm.x_rot) >> 2;

		if (camera.old_type != LOOK_CAMERA)
		{
			lara.head_y_rot = lara.torso_y_rot;
			lara.head_x_rot = lara.torso_x_rot;
		}
	}
	else if (lara.left_arm.lock && !lara.right_arm.lock)
	{
		lara.torso_y_rot = lara.left_arm.y_rot >> 1;
		lara.torso_x_rot = lara.left_arm.x_rot >> 1;

		if (camera.old_type != LOOK_CAMERA)
		{
			lara.head_y_rot = lara.torso_y_rot;
			lara.head_x_rot = lara.torso_x_rot;
		}
	}
	else if (!lara.left_arm.lock && lara.right_arm.lock)
	{
		lara.torso_y_rot = lara.right_arm.y_rot >> 1;
		lara.torso_x_rot = lara.right_arm.x_rot >> 1;

		if (camera.old_type != LOOK_CAMERA)
		{
			lara.head_y_rot = lara.torso_y_rot;
			lara.head_x_rot = lara.torso_x_rot;
		}
	}

	AnimatePistols(weapon_type);

	if (lara.left_arm.flash_gun || lara.right_arm.flash_gun)
	{
		pos.x = (GetRandomControl() & 0xFF) - 128;
		pos.y = (GetRandomControl() & 0x7F) - 63;
		pos.z = (GetRandomControl() & 0xFF) - 128;

		if (lara.left_arm.flash_gun)
			GetLaraJointPos(&pos, 13);
		else
			GetLaraJointPos(&pos, 14);

		r = (GetRandomControl() & 0x3F) + 192;
		g = (GetRandomControl() & 0x1F) + 128;
		b = GetRandomControl() & 0x3F;

		if (gfLevelFlags & GF_MIRROR && lara_item->room_number == gfMirrorRoom)
			TriggerDynamic_MIRROR(pos.x, pos.y, pos.z, 10, r, g, b);
		else
			TriggerDynamic(pos.x, pos.y, pos.z, 10, r, g, b);
	}
}
