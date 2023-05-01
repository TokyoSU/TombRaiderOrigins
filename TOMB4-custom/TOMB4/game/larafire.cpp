#include "../tomb4/pch.h"
#include "larafire.h"
#include "lara_states.h"
#include "objects.h"
#include "lara2gun.h"
#include "lara1gun.h"
#include "laraflar.h"
#include "delstuff.h"
#include "draw.h"
#include "../specific/3dmath.h"
#include "control.h"
#include "../specific/function_stubs.h"
#include "sphere.h"
#include "effect2.h"
#include "sound.h"
#include "effects.h"
#include "items.h"
#include "flmtorch.h"
#include "camera.h"
#include "../specific/input.h"
#include "lara.h"
#include "savegame.h"
#include "lot.h"
#include "gameflow.h"

std::vector<WEAPON_INFO> weapons;

static short HoldStates[] =
{
	AS_WALK,
	AS_STOP,
	AS_POSE,
	AS_TURN_R,
	AS_TURN_L,
	AS_BACK,
	AS_FASTTURN,
	AS_STEPLEFT,
	AS_STEPRIGHT,
	AS_WADE,
	AS_DUCK,
	AS_DUCKROTL,
	AS_DUCKROTR,
	-1
};

static PHD_3DPOS bum_view;
static GAME_VECTOR bum_vdest;
static GAME_VECTOR bum_vsrc;
static ITEM_INFO* TargetList[8];
static ITEM_INFO* LastTargets[8];

static long CheckForHoldingState(long state)
{
	short* holds;

	holds = HoldStates;

	while (*holds >= 0)
	{
		if (state == *holds)
			return 1;

		holds++;
	}

	return 0;
}

constexpr short ANGLE(short value) { return value * 182; }
constexpr short DISTANCE(int value) { return value * 1024; }

void InitialiseWeaponArray()
{
	WEAPON_INFO none; // WEAPON_NONE
	none.setLockAngle(0, 0, 0, 0);
	none.setLeftAngle(0, 0, 0, 0);
	none.setRightAngle(0, 0, 0, 0);
	none.setAimSpeed(0);
	none.setShotAccuracy(0);
	none.setGunHeight(0);
	none.setTargetDistance(0);
	none.setDamage(0);
	none.setAlternateDamage(0);
	none.setRecoilFrame(0);
	none.setFlashTime(0);
	none.setDrawFrame(0);
	none.setShotSample(0);
	weapons.push_back(none);

	WEAPON_INFO flare; // WEAPON_FLARE
	flare.setLockAngle(0, 0, 0, 0);
	flare.setLeftAngle(0, 0, 0, 0);
	flare.setRightAngle(0, 0, 0, 0);
	flare.setAimSpeed(0);
	flare.setShotAccuracy(0);
	flare.setGunHeight(0);
	flare.setTargetDistance(0);
	flare.setDamage(0);
	flare.setAlternateDamage(0);
	flare.setRecoilFrame(0);
	flare.setFlashTime(0);
	flare.setDrawFrame(0);
	flare.setShotSample(0);
	weapons.push_back(flare);

	WEAPON_INFO torch; // WEAPON_TORCH
	torch.setLockAngle(0, 0, 0, 0);
	torch.setLeftAngle(0, 0, 0, 0);
	torch.setRightAngle(0, 0, 0, 0);
	torch.setAimSpeed(0);
	torch.setShotAccuracy(0);
	torch.setGunHeight(0);
	torch.setTargetDistance(0);
	torch.setDamage(0);
	torch.setAlternateDamage(0);
	torch.setRecoilFrame(0);
	torch.setFlashTime(0);
	torch.setDrawFrame(0);
	torch.setShotSample(0);
	weapons.push_back(torch);

	WEAPON_INFO pistols; // WEAPON_PISTOLS
	pistols.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	pistols.setLeftAngle(-ANGLE(170), ANGLE(60), -ANGLE(80), ANGLE(80));
	pistols.setRightAngle(-ANGLE(60), ANGLE(170), -ANGLE(80), ANGLE(80));
	pistols.setAimSpeed(ANGLE(10));
	pistols.setShotAccuracy(ANGLE(8));
	pistols.setGunHeight(650);
	pistols.setTargetDistance(DISTANCE(8));
	pistols.setDamage(1);
	pistols.setAlternateDamage(0);
	pistols.setRecoilFrame(9);
	pistols.setFlashTime(3);
	pistols.setDrawFrame(0);
	pistols.setShotSample(SFX_LARA_FIRE);
	weapons.push_back(pistols);

	WEAPON_INFO uzi; // WEAPON_UZI
	uzi.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	uzi.setLeftAngle(-ANGLE(170), ANGLE(60), -ANGLE(80), ANGLE(80));
	uzi.setRightAngle(-ANGLE(60), ANGLE(170), -ANGLE(80), ANGLE(80));
	uzi.setAimSpeed(ANGLE(10));
	uzi.setShotAccuracy(ANGLE(8));
	uzi.setGunHeight(650);
	uzi.setTargetDistance(DISTANCE(8));
	uzi.setDamage(1);
	uzi.setAlternateDamage(0);
	uzi.setRecoilFrame(2);
	uzi.setFlashTime(3);
	uzi.setDrawFrame(0);
	uzi.setShotSample(SFX_LARA_UZI_FIRE);
	weapons.push_back(uzi);

	WEAPON_INFO magnum; // WEAPON_MAGNUMS
	magnum.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	magnum.setLeftAngle(-ANGLE(170), ANGLE(60), -ANGLE(80), ANGLE(80));
	magnum.setRightAngle(-ANGLE(60), ANGLE(170), -ANGLE(80), ANGLE(80));
	magnum.setAimSpeed(ANGLE(10));
	magnum.setShotAccuracy(ANGLE(8));
	magnum.setGunHeight(650);
	magnum.setTargetDistance(DISTANCE(8));
	magnum.setDamage(3);
	magnum.setAlternateDamage(0);
	magnum.setRecoilFrame(9);
	magnum.setFlashTime(3);
	magnum.setDrawFrame(0);
	magnum.setShotSample(SFX_LARA_FIRE);
	weapons.push_back(magnum);

	WEAPON_INFO autopistols; // WEAPON_AUTOPISTOLS
	autopistols.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	autopistols.setLeftAngle(-ANGLE(170), ANGLE(60), -ANGLE(80), ANGLE(80));
	autopistols.setRightAngle(-ANGLE(60), ANGLE(170), -ANGLE(80), ANGLE(80));
	autopistols.setAimSpeed(ANGLE(10));
	autopistols.setShotAccuracy(ANGLE(8));
	autopistols.setGunHeight(650);
	autopistols.setTargetDistance(DISTANCE(8));
	autopistols.setDamage(2);
	autopistols.setAlternateDamage(0);
	autopistols.setRecoilFrame(7);
	autopistols.setFlashTime(3);
	autopistols.setDrawFrame(0);
	autopistols.setShotSample(SFX_LARA_FIRE);
	weapons.push_back(autopistols);

	WEAPON_INFO deserteagle; // WEAPON_DESERTEAGLE
	deserteagle.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	deserteagle.setLeftAngle(-ANGLE(10), ANGLE(10), -ANGLE(80), ANGLE(80));
	deserteagle.setRightAngle(0, 0, 0, 0);
	deserteagle.setAimSpeed(ANGLE(10));
	deserteagle.setShotAccuracy(ANGLE(4));
	deserteagle.setGunHeight(650);
	deserteagle.setTargetDistance(DISTANCE(8));
	deserteagle.setDamage(30);
	deserteagle.setAlternateDamage(0);
	deserteagle.setRecoilFrame(18);
	deserteagle.setFlashTime(3);
	deserteagle.setDrawFrame(0);
	deserteagle.setShotSample(SFX_REVOLVER_FIRE);
	weapons.push_back(deserteagle);

	WEAPON_INFO revolver; // WEAPON_REVOLVER
	revolver.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	revolver.setLeftAngle(-ANGLE(10), ANGLE(10), -ANGLE(80), ANGLE(80));
	revolver.setRightAngle(0, 0, 0, 0);
	revolver.setAimSpeed(ANGLE(10));
	revolver.setShotAccuracy(ANGLE(4));
	revolver.setGunHeight(650);
	revolver.setTargetDistance(DISTANCE(8));
	revolver.setDamage(15);
	revolver.setAlternateDamage(0);
	revolver.setRecoilFrame(11);
	revolver.setFlashTime(3);
	revolver.setDrawFrame(0);
	revolver.setShotSample(SFX_REVOLVER_FIRE);
	weapons.push_back(revolver);

	WEAPON_INFO shotgun; // WEAPON_SHOTGUN
	shotgun.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	shotgun.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	shotgun.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	shotgun.setAimSpeed(ANGLE(10));
	shotgun.setShotAccuracy(0);
	shotgun.setGunHeight(500);
	shotgun.setTargetDistance(DISTANCE(8));
	shotgun.setDamage(3);
	shotgun.setAlternateDamage(0);
	shotgun.setRecoilFrame(9);
	shotgun.setFlashTime(3);
	shotgun.setDrawFrame(10);
	shotgun.setShotSample(SFX_LARA_SHOTGUN_FIRE);
	weapons.push_back(shotgun);

	WEAPON_INFO m16; // WEAPON_M16
	m16.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	m16.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	m16.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	m16.setAimSpeed(ANGLE(10));
	m16.setShotAccuracy(ANGLE(4));
	m16.setGunHeight(500);
	m16.setTargetDistance(DISTANCE(12));
	m16.setDamage(5); // When standing
	m16.setAlternateDamage(2); // When running
	m16.setRecoilFrame(4);
	m16.setFlashTime(3);
	m16.setDrawFrame(10);
	m16.setShotSample(SFX_LARA_FIRE);
	weapons.push_back(m16);

	WEAPON_INFO mp5; // WEAPON_MP5
	mp5.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	mp5.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	mp5.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	mp5.setAimSpeed(ANGLE(10));
	mp5.setShotAccuracy(ANGLE(7));
	mp5.setGunHeight(500);
	mp5.setTargetDistance(DISTANCE(12));
	mp5.setDamage(2); // When standing
	mp5.setAlternateDamage(1); // When running
	mp5.setRecoilFrame(2);
	mp5.setFlashTime(3);
	mp5.setDrawFrame(16);
	mp5.setShotSample(SFX_LARA_FIRE);
	weapons.push_back(mp5);

	WEAPON_INFO crossbow; // WEAPON_CROSSBOW
	crossbow.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	crossbow.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	crossbow.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	crossbow.setAimSpeed(ANGLE(10));
	crossbow.setShotAccuracy(ANGLE(8));
	crossbow.setGunHeight(500);
	crossbow.setTargetDistance(DISTANCE(8));
	crossbow.setDamage(5);
	crossbow.setAlternateDamage(0);
	crossbow.setRecoilFrame(5);
	crossbow.setFlashTime(0);
	crossbow.setDrawFrame(2);
	crossbow.setShotSample(SFX_LARA_CROSSBOW);
	weapons.push_back(crossbow);

	WEAPON_INFO harpoon; // WEAPON_HARPOON
	harpoon.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	harpoon.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	harpoon.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	harpoon.setAimSpeed(ANGLE(10));
	harpoon.setShotAccuracy(0);
	harpoon.setGunHeight(500);
	harpoon.setTargetDistance(DISTANCE(8));
	harpoon.setDamage(1); // When standing
	harpoon.setAlternateDamage(10); // When underwater
	harpoon.setRecoilFrame(5);
	harpoon.setFlashTime(0);
	harpoon.setDrawFrame(2);
	harpoon.setShotSample(SFX_LARA_CROSSBOW);
	weapons.push_back(harpoon);

	WEAPON_INFO grenade; // WEAPON_GRENADE
	grenade.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	grenade.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	grenade.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	grenade.setAimSpeed(ANGLE(10));
	grenade.setShotAccuracy(0);
	grenade.setGunHeight(500);
	grenade.setTargetDistance(DISTANCE(8));
	grenade.setDamage(1); // When standing
	grenade.setAlternateDamage(10); // When underwater
	grenade.setRecoilFrame(5);
	grenade.setFlashTime(0);
	grenade.setDrawFrame(2);
	grenade.setShotSample(SFX_LARA_MINI_FIRE);
	weapons.push_back(grenade);

	WEAPON_INFO rocket; // WEAPON_ROCKET
	rocket.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	rocket.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	rocket.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	rocket.setAimSpeed(ANGLE(10));
	rocket.setShotAccuracy(ANGLE(8));
	rocket.setGunHeight(500);
	rocket.setTargetDistance(DISTANCE(20));
	rocket.setDamage(30);
	rocket.setAlternateDamage(0);
	rocket.setRecoilFrame(0);
	rocket.setFlashTime(2);
	rocket.setDrawFrame(12);
	rocket.setShotSample(SFX_LARA_MINI_FIRE);
	weapons.push_back(rocket);

	WEAPON_INFO grappling; // WEAPON_GRAPPLING
	grappling.setLockAngle(-ANGLE(60), ANGLE(60), -ANGLE(60), ANGLE(60));
	grappling.setLeftAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	grappling.setRightAngle(-ANGLE(80), ANGLE(80), -ANGLE(65), ANGLE(65));
	grappling.setAimSpeed(ANGLE(10));
	grappling.setShotAccuracy(0);
	grappling.setGunHeight(500);
	grappling.setTargetDistance(DISTANCE(8));
	grappling.setDamage(40);
	grappling.setAlternateDamage(0);
	grappling.setRecoilFrame(5);
	grappling.setFlashTime(0);
	grappling.setDrawFrame(2);
	grappling.setShotSample(SFX_LARA_MINI_FIRE);
	weapons.push_back(grappling);

	WEAPON_INFO snowmobilegun; // WEAPON_SNOWMOBILEGUN
	snowmobilegun.setLockAngle(-ANGLE(30), ANGLE(30), -ANGLE(55), ANGLE(55));
	snowmobilegun.setLeftAngle(-ANGLE(30), ANGLE(30), -ANGLE(55), ANGLE(55));
	snowmobilegun.setRightAngle(-ANGLE(30), ANGLE(30), -ANGLE(55), ANGLE(55));
	snowmobilegun.setAimSpeed(ANGLE(10));
	snowmobilegun.setShotAccuracy(ANGLE(8));
	snowmobilegun.setGunHeight(400);
	snowmobilegun.setTargetDistance(DISTANCE(8));
	snowmobilegun.setDamage(3);
	snowmobilegun.setAlternateDamage(0);
	snowmobilegun.setRecoilFrame(0);
	snowmobilegun.setFlashTime(2);
	snowmobilegun.setDrawFrame(0);
	snowmobilegun.setShotSample(SFX_LARA_UZI_FIRE);
	weapons.push_back(snowmobilegun);
}

void InitialiseNewWeapon()
{
	lara.right_arm.frame_number = 0;
	lara.left_arm.frame_number = 0;
	lara.left_arm.z_rot = 0;
	lara.left_arm.y_rot = 0;
	lara.left_arm.x_rot = 0;
	lara.right_arm.z_rot = 0;
	lara.right_arm.y_rot = 0;
	lara.right_arm.x_rot = 0;
	lara.target = 0;
	lara.right_arm.lock = 0;
	lara.left_arm.lock = 0;
	lara.right_arm.flash_gun = 0;
	lara.left_arm.flash_gun = 0;

	switch (lara.gun_type)
	{
	case WEAPON_PISTOLS:
	case WEAPON_UZI:
	case WEAPON_AUTOPISTOLS:
	case WEAPON_MAGNUMS:
		lara.left_arm.frame_base = objects[PISTOLS_ANIM].frame_base;
		lara.right_arm.frame_base = objects[PISTOLS_ANIM].frame_base;

		if (lara.gun_status != LG_NO_ARMS)
			draw_pistol_meshes(lara.gun_type);

		break;

	case WEAPON_REVOLVER:
	case WEAPON_DESERTEAGLE:
	case WEAPON_SHOTGUN:
	case WEAPON_MP5:
	case WEAPON_M16:
	case WEAPON_CROSSBOW:
	case WEAPON_HARPOON:
	case WEAPON_GRENADE:
	case WEAPON_ROCKET:
	case WEAPON_GRAPPLING:
		lara.left_arm.frame_base = objects[GetWeaponAnimObject(lara.gun_type)].frame_base;
		lara.right_arm.frame_base = objects[GetWeaponAnimObject(lara.gun_type)].frame_base;

		if (lara.gun_status != LG_NO_ARMS)
			draw_shotgun_meshes(lara.gun_type);

		break;

	case WEAPON_FLARE:
		lara.left_arm.frame_base = objects[FLARE_ANIM].frame_base;
		lara.right_arm.frame_base = objects[FLARE_ANIM].frame_base;

		if (lara.gun_status != LG_NO_ARMS)
			draw_flare_meshes();

		break;

	default:
		lara.left_arm.frame_base = anims[lara_item->anim_number].frame_ptr;
		lara.right_arm.frame_base = anims[lara_item->anim_number].frame_ptr;
		break;
	}
}

static void find_target_point(ITEM_INFO* item, GAME_VECTOR* target)
{
	long x, y, z, c, s;
	short* bounds;

	bounds = GetBestFrame(item);
	x = (bounds[0] + bounds[1]) >> 1;
	y = (bounds[2] + (bounds[3] - bounds[2]) / 3);
	z = (bounds[4] + bounds[5]) >> 1;
	s = phd_sin(item->pos.y_rot);
	c = phd_cos(item->pos.y_rot);
	target->x = item->pos.x_pos + ((x * c + z * s) >> W2V_SHIFT);
	target->y = item->pos.y_pos + y;
	target->z = item->pos.z_pos + ((z * c - x * s) >> W2V_SHIFT);
	target->room_number = item->room_number;
}

void LaraTargetInfo(WEAPON_INFO* winfo)
{
	GAME_VECTOR src, target;
	short ang[2];

	if (!lara.target)
	{
		lara.right_arm.lock = 0;
		lara.left_arm.lock = 0;
		lara.target_angles[1] = 0;
		lara.target_angles[0] = 0;
		return;
	}

	src.x = 0;
	src.y = 0;
	src.z = 0;
	GetLaraJointPos((PHD_VECTOR*)&src, 11);
	src.x = lara_item->pos.x_pos;
	src.z = lara_item->pos.z_pos;
	src.room_number = lara_item->room_number;
	find_target_point(lara.target, &target);
	phd_GetVectorAngles(target.x - src.x, target.y - src.y, target.z - src.z, ang);
	ang[0] -= lara_item->pos.y_rot;
	ang[1] -= lara_item->pos.x_rot;

	if (LOS(&src, &target))
	{
		if (ang[0] >= winfo->lock_angles[0] && ang[0] <= winfo->lock_angles[1] &&
			ang[1] >= winfo->lock_angles[2] && ang[1] <= winfo->lock_angles[3])
		{
			lara.left_arm.lock = 1;
			lara.right_arm.lock = 1;
			lara.target_angles[0] = ang[0];
			lara.target_angles[1] = ang[1];
			return;
		}

		if (lara.left_arm.lock)
		{
			if (ang[0] < winfo->left_angles[0] || ang[0] > winfo->left_angles[1] ||
				ang[1] < winfo->left_angles[2] || ang[1] > winfo->left_angles[3])
				lara.left_arm.lock = 0;
		}

		if (lara.right_arm.lock)
		{
			if (ang[0] < winfo->right_angles[0] || ang[0] > winfo->right_angles[1] ||
				ang[1] < winfo->left_angles[2] || ang[1] > winfo->left_angles[3])
			{
				lara.right_arm.lock = 0;
				lara.target_angles[0] = ang[0];
				lara.target_angles[1] = ang[1];
				return;
			}
		}
	}
	else
	{
		lara.right_arm.lock = 0;
		lara.left_arm.lock = 0;
	}

	lara.target_angles[0] = ang[0];
	lara.target_angles[1] = ang[1];
}

short& get_current_ammo_pointer(long weapon_type)
{
	switch (weapon_type)
	{
	case WEAPON_PISTOLS:
		return lara.num_pistols_ammo;
	case WEAPON_UZI:
		return lara.num_uzi_ammo;
	case WEAPON_MAGNUMS:
		return lara.num_magnums_ammo;
	case WEAPON_AUTOPISTOLS:
		return lara.num_autopistols_ammo;
	case WEAPON_DESERTEAGLE:
		return lara.num_deserteagle_ammo;
	case WEAPON_REVOLVER:
		return lara.num_revolver_ammo;
	case WEAPON_SHOTGUN:
		if (lara.shotgun_type_carried & W_AMMO1)
			return lara.num_shotgun_ammo1;
		else if (lara.shotgun_type_carried & W_AMMO2)
			return lara.num_shotgun_ammo2;
		break;
	case WEAPON_M16:
		return lara.num_m16_ammo;
	case WEAPON_MP5:
		return lara.num_mp5_ammo;
	case WEAPON_CROSSBOW:
		if (lara.crossbow_type_carried & W_AMMO1)
			return lara.num_crossbow_ammo1;
		else if (lara.crossbow_type_carried & W_AMMO2)
			return lara.num_crossbow_ammo2;
		else if (lara.crossbow_type_carried & W_AMMO3)
			return lara.num_crossbow_ammo3;
		break;
	case WEAPON_HARPOON:
		return lara.num_harpoon_ammo;
	case WEAPON_GRENADE:
		if (lara.grenade_type_carried & W_AMMO1)
			return lara.num_grenade_ammo1;
		else if (lara.grenade_type_carried & W_AMMO2)
			return lara.num_grenade_ammo2;
		else if (lara.grenade_type_carried & W_AMMO3)
			return lara.num_grenade_ammo3;
		break;
	case WEAPON_ROCKET:
		return lara.num_rocket_ammo;
	case WEAPON_GRAPPLING:
		return lara.num_grappling_ammo;
	}
	return lara.num_pistols_ammo;
}

long FireWeapon(long weapon_type, ITEM_INFO* target, ITEM_INFO* src, short* angles)
{
	WEAPON_INFO* winfo;
	SPHERE* sptr;
	long r, nSpheres, bestdist, best;
	short room_number;

	bum_view.x_pos = 0;
	bum_view.y_pos = 0;
	bum_view.z_pos = 0;
	GetLaraJointPos((PHD_VECTOR*)&bum_view, 11);

	short& ammo = get_current_ammo_pointer(weapon_type);
	if (ammo == 0)
		return 0;
	if (ammo != -1)
		ammo--;

	winfo = &weapons[weapon_type];
	bum_view.x_pos = src->pos.x_pos;
	bum_view.z_pos = src->pos.z_pos;
	bum_view.x_rot = short(winfo->shot_accuracy * (GetRandomControl() - 0x4000) / 0x10000 + angles[1]);
	bum_view.y_rot = short(winfo->shot_accuracy * (GetRandomControl() - 0x4000) / 0x10000 + angles[0]);
	bum_view.z_rot = 0;
	phd_GenerateW2V(&bum_view);

	nSpheres = GetSpheres(target, Slist, 0);
	best = -1;
	bestdist = 0x7FFFFFFF;

	for (int i = 0; i < nSpheres; i++)
	{
		sptr = &Slist[i];
		r = sptr->r;

		if (abs(sptr->x) < r && abs(sptr->y) < r && sptr->z > r && SQUARE(sptr->x) + SQUARE(sptr->y) <= SQUARE(r))
		{
			if (sptr->z - r < bestdist)
			{
				bestdist = sptr->z - r;
				best = i;
			}
		}
	}

	lara.has_fired = 1;
	bum_vsrc.x = bum_view.x_pos;
	bum_vsrc.y = bum_view.y_pos;
	bum_vsrc.z = bum_view.z_pos;
	room_number = src->room_number;
	GetFloor(bum_view.x_pos, bum_view.y_pos, bum_view.z_pos, &room_number);
	bum_vsrc.room_number = room_number;

	if (best < 0)
	{
		bum_vdest.x = bum_vsrc.x + long(0x5000 * mMXPtr[M20]);
		bum_vdest.y = bum_vsrc.y + long(0x5000 * mMXPtr[M21]);
		bum_vdest.z = bum_vsrc.z + long(0x5000 * mMXPtr[M22]);
		GetTargetOnLOS(&bum_vsrc, &bum_vdest, 0, 1);
		return -1;
	}
	else
	{
		savegame.Game.AmmoHits++;
		bum_vdest.x = bum_vsrc.x + long(bestdist * mMXPtr[M20]);
		bum_vdest.y = bum_vsrc.y + long(bestdist * mMXPtr[M21]);
		bum_vdest.z = bum_vsrc.z + long(bestdist * mMXPtr[M22]);

		if (!GetTargetOnLOS(&bum_vsrc, &bum_vdest, 0, 1))
			HitTarget(target, &bum_vdest, winfo->damage, 0);

		return 1;
	}
}

void AimWeapon(WEAPON_INFO* winfo, LARA_ARM* arm)
{
	short speed, x, y;

	speed = winfo->aim_speed;

	if (arm->lock)
	{
		y = lara.target_angles[0];
		x = lara.target_angles[1];
	}
	else
	{
		x = 0;
		y = 0;
	}

	if (arm->y_rot >= y - speed && arm->y_rot <= speed + y)
		arm->y_rot = y;
	else if (arm->y_rot < y)
		arm->y_rot += speed;
	else
		arm->y_rot -= speed;

	if (arm->x_rot >= x - speed && arm->x_rot <= speed + x)
		arm->x_rot = x;
	else if (arm->x_rot < x)
		arm->x_rot += speed;
	else
		arm->x_rot -= speed;

	arm->z_rot = 0;
}

void LaraGetNewTarget(WEAPON_INFO* winfo)
{
	ITEM_INFO* item;
	ITEM_INFO* bestitem;
	CREATURE_INFO* creature;
	GAME_VECTOR src, target;
	long x, y, z, slot, dist, maxdist, maxdist2, bestdist;
	short ang[2];
	short bestyrot, targets, match;

	if (BinocularRange)
	{
		lara.target = 0;
		return;
	}

	bestitem = 0;
	src.x = lara_item->pos.x_pos;
	src.y = lara_item->pos.y_pos - 650;
	src.z = lara_item->pos.z_pos;
	src.room_number = lara_item->room_number;
	bestyrot = 0x7FFF;
	bestdist = 0x7FFFFFFF;
	maxdist = winfo->target_dist;
	maxdist2 = SQUARE(maxdist);
	creature = baddie_slots;
	targets = 0;

	for (slot = 0; slot < 5; slot++, creature++)
	{
		if (creature->item_num != NO_ITEM)
		{
			item = &items[creature->item_num];

			if (item->hit_points > 0)
			{
				x = item->pos.x_pos - src.x;
				y = item->pos.y_pos - src.y;
				z = item->pos.z_pos - src.z;

				if (abs(x) <= maxdist && abs(y) <= maxdist && abs(z) <= maxdist)
				{
					dist = SQUARE(x) + SQUARE(y) + SQUARE(z);

					if (dist < maxdist2)
					{
						find_target_point(item, &target);

						if (LOS(&src, &target))
						{
							phd_GetVectorAngles(target.x - src.x, target.y - src.y, target.z - src.z, ang);
							ang[0] -= (lara.torso_y_rot + lara_item->pos.y_rot);
							ang[1] -= (lara.torso_x_rot + lara_item->pos.x_rot);

							if (ang[0] >= winfo->lock_angles[0] && ang[0] <= winfo->lock_angles[1] &&
								ang[1] >= winfo->lock_angles[2] && ang[1] <= winfo->lock_angles[3])
							{
								TargetList[targets] = item;
								targets++;

								if (abs(ang[0]) < bestyrot + 2730 && dist < bestdist)
								{
									bestdist = dist;
									bestyrot = abs(ang[0]);
									bestitem = item;
								}
							}
						}
					}
				}
			}
		}
	}

	TargetList[targets] = 0;

	if (TargetList[0])
	{
		for (slot = 0; slot < 8; slot++)
		{
			if (!TargetList[slot])
				lara.target = 0;

			if (TargetList[slot] == lara.target)
				break;
		}

		if (savegame.AutoTarget || input & IN_TARGET)
		{
			if (!lara.target)
			{
				lara.target = bestitem;
				LastTargets[0] = 0;
			}
			else if (input & IN_TARGET)
			{
				lara.target = 0;

				for (match = 0; match < 8; match++)
				{
					if (!TargetList[match])
						break;

					for (slot = 0; slot < 8; slot++)
					{
						if (!LastTargets[slot])
						{
							slot = 8;
							break;
						}

						if (LastTargets[slot] == TargetList[match])
							break;
					}

					if (slot == 8)
					{
						lara.target = TargetList[match];
						break;
					}
				}

				if (!lara.target)
				{
					lara.target = bestitem;
					LastTargets[0] = 0;
				}
			}
		}
	}
	else
		lara.target = 0;

	if (lara.target != LastTargets[0])
	{
		for (slot = 7; slot > 0; slot--)
			LastTargets[slot] = LastTargets[slot - 1];

		LastTargets[0] = lara.target;
	}

	LaraTargetInfo(winfo);
}

void HitTarget(ITEM_INFO* item, GAME_VECTOR* hitpos, long damage, long grenade)
{
	OBJECT_INFO* obj;

	obj = &objects[item->object_number];
	item->hit_status = 1;

	if (item->data && item != lara_item)
		((CREATURE_INFO*)item->data)->hurt_by_lara = 1;

	if (hitpos && obj->HitEffect)
	{
		switch (obj->HitEffect)
		{
		case 1:

			if (item->object_number == SUPER_RAGHEAD && (item->current_anim_state == 8 || GetRandomControl() & 1) &&
				(lara.gun_type == WEAPON_PISTOLS || lara.gun_type == WEAPON_SHOTGUN || lara.gun_type == WEAPON_UZI))
			{
				SOUND_PlayEffect(SFX_BAD_SWORD_RICO, &item->pos, SFX_DEFAULT);
				TriggerRicochetSpark(hitpos, lara_item->pos.y_rot, 3, 0);
				return;
			}

			DoBloodSplat(hitpos->x, hitpos->y, hitpos->z, (GetRandomControl() & 3) + 3, item->pos.y_rot, item->room_number);
			break;

		case 2:
			TriggerRicochetSpark(hitpos, lara_item->pos.y_rot, 3, -5);
			break;

		case 3:
			TriggerRicochetSpark(hitpos, lara_item->pos.y_rot, 3, 0);
			break;
		}
	}

	if (!obj->undead || grenade || item->hit_points == -16384)
	{
		if (item->hit_points > 0 && item->hit_points <= damage)
			savegame.Level.Kills++;

		item->hit_points -= (short)damage;
	}
}

long GetWeaponMeshObject(long weapon_type)
{
	switch (weapon_type)
	{
	default:
	case WEAPON_NONE:
	case WEAPON_PISTOLS:
		return PISTOLS_ANIM;
	case WEAPON_FLARE:
		return FLARE_ANIM;
	case WEAPON_TORCH:
		return TORCH_ANIM;
	case WEAPON_UZI:
		return UZI_ANIM;
	case WEAPON_MAGNUMS:
		return MAGNUMS_ANIM;
	case WEAPON_AUTOPISTOLS:
		return AUTOPISTOLS_ANIM;
	case WEAPON_DESERTEAGLE:
		return DESERTEAGLE_ANIM;
	case WEAPON_REVOLVER:
		if (lara.revolver_type_carried & W_LASERSIGHT)
			return LARA_REVOLVER_LASER_MESH;
		else
			return REVOLVER_ANIM;
		break;
	case WEAPON_SHOTGUN:
		return SHOTGUN_ANIM;
	case WEAPON_M16:
		return M16_ANIM;
	case WEAPON_MP5:
		if (lara.mp5_type_carried & W_LASERSIGHT)
			return LARA_MP5_LASER_MESH;
		else
			return MP5_ANIM;
		break;
	case WEAPON_CROSSBOW:
		if (lara.crossbow_type_carried & W_LASERSIGHT)
			return LARA_CROSSBOW_LASER_MESH;
		else
			return CROSSBOW_ANIM;
		break;
	case WEAPON_HARPOON:
		return HARPOON_GUN_ANIM;
	case WEAPON_GRENADE:
		return GRENADE_GUN_ANIM;
	case WEAPON_ROCKET:
		return ROCKET_GUN_ANIM;
	case WEAPON_GRAPPLING:
		return GRAPPLING_GUN_ANIM;
	}
}

long GetWeaponHolsterObject(long weapon_type)
{
	switch (weapon_type)
	{
		// NORMAL Holsters
	default:
	case WEAPON_NONE:
		return LARA_HOLSTERS;
	case WEAPON_PISTOLS:
		return LARA_HOLSTERS_PISTOLS;
	case WEAPON_UZI:
		return LARA_HOLSTERS_UZIS;
	case WEAPON_MAGNUMS:
		return LARA_HOLSTERS_MAGNUMS;
	case WEAPON_AUTOPISTOLS:
		return LARA_HOLSTERS_AUTOPISTOLS;
	case WEAPON_DESERTEAGLE:
		return LARA_HOLSTERS_DESERTEAGLE;
	case WEAPON_REVOLVER:
		return LARA_HOLSTERS_REVOLVER;
	}
}

long GetWeaponAnimObject(long weapon_type)
{
	switch (weapon_type)
	{
	default:
	case WEAPON_NONE:
	case WEAPON_PISTOLS:
		return PISTOLS_ANIM;
	case WEAPON_FLARE:
		return FLARE_ANIM;
	case WEAPON_TORCH:
		return TORCH_ANIM;
	case WEAPON_UZI:
		return UZI_ANIM;
	case WEAPON_MAGNUMS:
		return MAGNUMS_ANIM;
	case WEAPON_AUTOPISTOLS:
		return AUTOPISTOLS_ANIM;
	case WEAPON_DESERTEAGLE:
		return DESERTEAGLE_ANIM;
	case WEAPON_REVOLVER:
		return REVOLVER_ANIM;
	case WEAPON_SHOTGUN:
		return SHOTGUN_ANIM;
	case WEAPON_M16:
		return M16_ANIM;
	case WEAPON_MP5:
		return MP5_ANIM;
	case WEAPON_CROSSBOW:
		return CROSSBOW_ANIM;
	case WEAPON_HARPOON:
		return HARPOON_GUN_ANIM;
	case WEAPON_GRENADE:
		return GRENADE_GUN_ANIM;
	case WEAPON_ROCKET:
		return ROCKET_GUN_ANIM;
	case WEAPON_GRAPPLING:
		return GRAPPLING_GUN_ANIM;
	}
}

void DoProperDetection(short item_number, long x, long y, long z, long xv, long yv, long zv)
{
	ITEM_INFO* item;
	FLOOR_INFO* floor;
	long ceiling, height, oldtype, oldonobj, oldheight, bs, yang, xs;
	short room_number;

	item = &items[item_number];

	room_number = item->room_number;
	floor = GetFloor(x, y, z, &room_number);
	oldheight = GetHeight(floor, x, y, z);

	room_number = item->room_number;
	floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);
	height = GetHeight(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);

	if (item->pos.y_pos >= height)
	{
		bs = 0;
		oldtype = height_type;

		if ((oldtype == BIG_SLOPE || oldtype == DIAGONAL) && oldheight < height)
		{
			yang = (ushort)item->pos.y_rot;

			if (tiltyoff < 0 && yang >= 32768 || tiltyoff > 0 && yang <= 32768 ||
				tiltxoff < 0 && yang >= 16384 && yang <= 49152 || tiltxoff > 0 && (yang <= 16384 || yang >= 49152))
				bs = 1;
		}

		if (y > height + 32 && !bs && ((item->pos.x_pos ^ x) & ~0x3FF || (item->pos.z_pos ^ z) & ~0x3FF))
		{
			xs = (item->pos.x_pos ^ x) & ~0x3FF && (item->pos.z_pos ^ z) & ~0x3FF ? abs(x - item->pos.x_pos) < abs(z - item->pos.z_pos) : 1;
			item->pos.y_rot = (item->pos.x_pos ^ x) & ~0x3FF && xs ? -item->pos.y_rot : -32768 - item->pos.y_rot;
			item->pos.x_pos = x;
			item->pos.y_pos = y;
			item->pos.z_pos = z;
			item->speed >>= 1;
		}
		else if (oldtype != BIG_SLOPE && oldtype != DIAGONAL)
		{
			if (item->fallspeed > 0)
			{
				if (item->fallspeed > 16)
				{
					if (item->object_number == GRENADE)
						item->fallspeed = (item->fallspeed >> 1) - item->fallspeed;
					else
					{
						item->fallspeed = -(item->fallspeed >> 2);

						if (item->fallspeed < -100)
							item->fallspeed = -100;
					}
				}
				else
				{
					item->fallspeed = 0;

					if (item->object_number == GRENADE)
					{
						item->speed--;
						item->required_anim_state = 1;
						item->pos.x_rot = 0;
					}
					else
						item->speed -= 3;

					if (item->speed < 0)
						item->speed = 0;
				}
			}

			item->pos.y_pos = height;
		}
		else
		{
			item->speed -= item->speed >> 2;

			if (tiltyoff < 0 && abs(tiltyoff) - abs(tiltxoff) >= 2)
			{
				if ((ushort)item->pos.y_rot > 32768)
				{
					item->pos.y_rot = -1 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed -= short(2 * tiltyoff);

						if ((ushort)item->pos.y_rot > 16384 && (ushort)item->pos.y_rot < 49152)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot < 16384)
								item->pos.y_rot = 16384;
						}
						else if ((ushort)item->pos.y_rot < 16384)
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot > 16384)
								item->pos.y_rot = 16384;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}
			else if (tiltyoff > 0 && abs(tiltyoff) - abs(tiltxoff) >= 2)
			{
				if ((ushort)item->pos.y_rot < 32768)
				{
					item->pos.y_rot = -1 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed += short(2 * tiltyoff);

						if ((ushort)item->pos.y_rot > 49152 || (ushort)item->pos.y_rot < 16384)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot < 49152)
								item->pos.y_rot = -16384;
						}
						else if ((ushort)item->pos.y_rot < 49152)
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot > 49152)
								item->pos.y_rot = -16384;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}
			else if (tiltxoff < 0 && abs(tiltxoff) - abs(tiltyoff) >= 2)
			{
				if ((ushort)item->pos.y_rot > 16384 && (ushort)item->pos.y_rot < 49152)
				{
					item->pos.y_rot = 32767 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed -= short(2 * tiltxoff);

						if ((ushort)item->pos.y_rot < 32768)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot > 61440)
								item->pos.y_rot = 0;
						}
						else
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot < 4096)
								item->pos.y_rot = 0;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}
			else if (tiltxoff > 0 && abs(tiltxoff) - abs(tiltyoff) >= 2)
			{
				if ((ushort)item->pos.y_rot > 49152 || (ushort)item->pos.y_rot < 16384)
				{
					item->pos.y_rot = 32767 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed += short(2 * tiltxoff);

						if ((ushort)item->pos.y_rot > 32768)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot < 32768)
								item->pos.y_rot = -32768;
						}
						else if ((ushort)item->pos.y_rot < 32768)
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot > 32768)
								item->pos.y_rot = -32768;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}
			else if (tiltyoff < 0 && tiltxoff < 0)
			{
				if ((ushort)item->pos.y_rot > 24576 && (ushort)item->pos.y_rot < 57344)
				{
					item->pos.y_rot = -16385 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed -= short(tiltxoff + tiltyoff);

						if ((ushort)item->pos.y_rot > 8192 && (ushort)item->pos.y_rot < 40960)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot < 8192)
								item->pos.y_rot = 8192;
						}
						else if (item->pos.y_rot != 8192)
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot > 8192)
								item->pos.y_rot = 8192;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}
			else if (tiltyoff < 0 && tiltxoff > 0)
			{
				if ((ushort)item->pos.y_rot > 40960 || (ushort)item->pos.y_rot < 8192)
				{
					item->pos.y_rot = 16383 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed += short(tiltxoff - tiltyoff);

						if ((ushort)item->pos.y_rot < 57344 && (ushort)item->pos.y_rot > 24576)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot < 24576)
								item->pos.y_rot = 24576;
						}
						else if (item->pos.y_rot != 24576)
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot > 24576)
								item->pos.y_rot = 24576;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}
			else if (tiltyoff > 0 && tiltxoff > 0)
			{
				if ((ushort)item->pos.y_rot > 57344 || (ushort)item->pos.y_rot < 24576)
				{
					item->pos.y_rot = -16385 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed += short(tiltxoff + tiltyoff);

						if ((ushort)item->pos.y_rot < 8192 || (ushort)item->pos.y_rot > 40960)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot < 40960)
								item->pos.y_rot = -24576;
						}
						else if (item->pos.y_rot != -24576)
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot > 40960)
								item->pos.y_rot = -24576;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}
			else if (tiltyoff > 0 && tiltxoff < 0)
			{
				if ((ushort)item->pos.y_rot > 8192 && (ushort)item->pos.y_rot < 40960)
				{
					item->pos.y_rot = 16383 - item->pos.y_rot;

					if (item->fallspeed > 0)
						item->fallspeed = -(item->fallspeed >> 1);
				}
				else
				{
					if (item->speed < 32)
					{
						item->speed += short(tiltyoff - tiltxoff);

						if ((ushort)item->pos.y_rot < 24576 || (ushort)item->pos.y_rot > 57344)
						{
							item->pos.y_rot -= 4096;

							if ((ushort)item->pos.y_rot < 57344)
								item->pos.y_rot = -8192;
						}
						else if (item->pos.y_rot != -8192)
						{
							item->pos.y_rot += 4096;

							if ((ushort)item->pos.y_rot > 57344)
								item->pos.y_rot = -8192;
						}
					}

					item->fallspeed = item->fallspeed > 0 ? -(item->fallspeed >> 1) : 0;
				}
			}

			item->pos.x_pos = x;
			item->pos.y_pos = y;
			item->pos.z_pos = z;
		}
	}
	else
	{
		if (yv >= 0)
		{
			room_number = item->room_number;
			floor = GetFloor(item->pos.x_pos, y, item->pos.z_pos, &room_number);
			height = GetHeight(floor, item->pos.x_pos, y, item->pos.z_pos);

			oldonobj = OnObject;

			room_number = item->room_number;
			floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);
			GetHeight(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);

			if (item->pos.y_pos >= height && oldonobj)
			{
				if (item->fallspeed > 0)
				{
					if (item->fallspeed > 16)
					{
						if (item->object_number == GRENADE)
							item->fallspeed = (item->fallspeed >> 1) - item->fallspeed;
						else
						{
							item->fallspeed = -(item->fallspeed >> 2);

							if (item->fallspeed < -100)
								item->fallspeed = -100;
						}
					}
					else
					{
						item->fallspeed = 0;

						if (item->object_number == GRENADE)
						{
							item->speed--;
							item->required_anim_state = 1;
							item->pos.x_rot = 0;
						}
						else
							item->speed -= 3;

						if (item->speed < 0)
							item->speed = 0;
					}
				}

				item->pos.y_pos = height;
			}
		}

		room_number = item->room_number;
		floor = GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);
		ceiling = GetCeiling(floor, item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);

		if (item->pos.y_pos < ceiling)
		{
			if (y < ceiling && ((item->pos.x_pos ^ x) & ~0x3FF || (item->pos.z_pos ^ z) & ~0x3FF))
			{
				item->pos.y_rot = (item->pos.x_pos ^ x) & ~0x3FF ? -item->pos.y_rot : -0x8000 - item->pos.y_rot;
				item->pos.x_pos = x;
				item->pos.y_pos = y;
				item->pos.z_pos = z;

				if (item->object_number == GRENADE)
					item->speed -= item->speed >> 3;
				else
					item->speed >>= 1;
			}
			else
				item->pos.y_pos = ceiling;

			if (item->fallspeed < 0)
				item->fallspeed = -item->fallspeed;
		}
	}

	room_number = item->room_number;
	GetFloor(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos, &room_number);

	if (room_number != item->room_number)
		ItemNewRoom(item_number, room_number);
}

void LaraGun()
{
	short state;

	if (lara.left_arm.flash_gun > 0)
		lara.left_arm.flash_gun--;
	if (lara.right_arm.flash_gun > 0)
		lara.right_arm.flash_gun--;

	if (lara.gun_type == WEAPON_TORCH)
	{
		DoFlameTorch();
		return;
	}

	if (lara_item->hit_points <= 0)
		lara.gun_status = LG_NO_ARMS;
	else if (lara.gun_status == LG_NO_ARMS)
	{
		if (input & IN_DRAW)
			lara.request_gun_type = lara.last_gun_type;
		else if (input & IN_FLARE && !(gfLevelFlags & GF_YOUNGLARA))
		{
			if (lara_item->current_anim_state == AS_DUCK && lara_item->anim_number != ANIM_DUCKBREATHE)
				return;

			if (lara.gun_type == WEAPON_FLARE)
			{
				if (!lara.left_arm.frame_number)
					lara.gun_status = LG_UNDRAW_GUNS;
			}
			else if (lara.num_flares)
			{
				if (lara.num_flares != -1)
					lara.num_flares--;

				lara.request_gun_type = WEAPON_FLARE;
			}
		}

		if (input & IN_DRAW || lara.request_gun_type != lara.gun_type)
		{
			state = lara_item->current_anim_state;

			if ((state == AS_DUCK || state == AS_DUCKROTL || state == AS_DUCKROTR) &&
				(lara.request_gun_type == WEAPON_SHOTGUN || lara.request_gun_type == WEAPON_CROSSBOW || lara.request_gun_type == WEAPON_GRENADE))
			{
				if (lara.gun_type == WEAPON_FLARE)
					lara.request_gun_type = WEAPON_FLARE;
			}
			else if (lara.request_gun_type != WEAPON_FLARE && (lara.vehicle != NO_ITEM || lara.water_status != LW_ABOVE_WATER &&
				(lara.water_status != LW_WADE || lara.water_surface_dist <= -weapons[lara.gun_type].gun_height)))
			{
				lara.last_gun_type = lara.request_gun_type;

				if (lara.gun_type != WEAPON_FLARE)
					lara.gun_type = lara.request_gun_type;
				else
					lara.request_gun_type = WEAPON_FLARE;
			}
			else
			{
				if (lara.gun_type == WEAPON_FLARE)
				{
					CreateFlare(FLARE_ITEM, 0);
					undraw_flare_meshes();
					lara.flare_control_left = 0;
					lara.flare_age = 0;
				}

				lara.gun_type = lara.request_gun_type;
				InitialiseNewWeapon();
				lara.gun_status = LG_DRAW_GUNS;
				lara.right_arm.frame_number = 0;
				lara.left_arm.frame_number = 0;
			}
		}
	}
	else if (lara.gun_status == LG_READY)
	{
		if (input & IN_DRAW || lara.request_gun_type != lara.gun_type || lara.water_status != LW_ABOVE_WATER &&
			(lara.water_status != LW_WADE || lara.water_surface_dist < -weapons[lara.gun_type].gun_height))
			lara.gun_status = LG_UNDRAW_GUNS;
	}
	else if (lara.gun_status == LG_HANDS_BUSY && input & IN_FLARE && lara_item->current_anim_state == AS_ALL4S && lara_item->anim_number == ANIM_ALL4S)
		lara.request_gun_type = 7;

	switch (lara.gun_status)
	{
	case LG_NO_ARMS:

		if (lara.gun_type == WEAPON_FLARE)
		{
			if (lara.vehicle == NO_ITEM && !CheckForHoldingState(lara_item->current_anim_state))
				lara.flare_control_left = 0;
			else if (lara.flare_control_left)
			{
				if (lara.left_arm.frame_number)
				{
					lara.left_arm.frame_number++;

					if (lara.left_arm.frame_number == 110)
						lara.left_arm.frame_number = 0;
				}
			}
			else
			{
				lara.flare_control_left = 1;
				lara.left_arm.frame_number = 95;
			}

			DoFlareInHand(lara.flare_age);
			set_flare_arm(lara.left_arm.frame_number);
		}

		break;

	case LG_HANDS_BUSY:

		if (lara.gun_type == WEAPON_FLARE && lara.mesh_ptrs[LM_LHAND] == meshes[objects[FLARE_ANIM].mesh_index + LM_LHAND * 2])
		{
			lara.flare_control_left = lara.vehicle != NO_ITEM || CheckForHoldingState(lara_item->current_anim_state);
			DoFlareInHand(lara.flare_age);
			set_flare_arm(lara.left_arm.frame_number);
		}

		break;

	case LG_DRAW_GUNS:

		if (lara.gun_type != WEAPON_FLARE && lara.gun_type != WEAPON_NONE)
			lara.last_gun_type = lara.gun_type;

		switch (lara.gun_type)
		{
		case WEAPON_PISTOLS:
		case WEAPON_REVOLVER:
		case WEAPON_DESERTEAGLE:
		case WEAPON_UZI:
		case WEAPON_MAGNUMS:
		case WEAPON_AUTOPISTOLS:

			if (camera.type != CINEMATIC_CAMERA && camera.type != LOOK_CAMERA && camera.type != HEAVY_CAMERA)
				camera.type = COMBAT_CAMERA;

			draw_pistols(lara.gun_type);
			break;

		case WEAPON_SHOTGUN:
		case WEAPON_M16:
		case WEAPON_MP5:
		case WEAPON_HARPOON:
		case WEAPON_ROCKET:
		case WEAPON_GRENADE:
		case WEAPON_CROSSBOW:
		case WEAPON_GRAPPLING:

			if (camera.type != CINEMATIC_CAMERA && camera.type != LOOK_CAMERA && camera.type != HEAVY_CAMERA)
				camera.type = COMBAT_CAMERA;

			draw_shotgun(lara.gun_type);
			break;

		case WEAPON_FLARE:
			draw_flare();
			break;

		default:
			lara.gun_status = LG_NO_ARMS;
			break;
		}

		break;

	case LG_UNDRAW_GUNS:
		lara.mesh_ptrs[LM_HEAD] = meshes[objects[LARA].mesh_index + LM_HEAD * 2];

		switch (lara.gun_type)
		{
		case WEAPON_PISTOLS:
		case WEAPON_MAGNUMS:
		case WEAPON_AUTOPISTOLS:
		case WEAPON_REVOLVER:
		case WEAPON_DESERTEAGLE:
		case WEAPON_UZI:
			undraw_pistols(lara.gun_type);
			break;

		case WEAPON_SHOTGUN:
		case WEAPON_M16:
		case WEAPON_MP5:
		case WEAPON_HARPOON:
		case WEAPON_ROCKET:
		case WEAPON_GRAPPLING:
		case WEAPON_GRENADE:
		case WEAPON_CROSSBOW:
			undraw_shotgun(lara.gun_type);
			break;

		case WEAPON_FLARE:
			undraw_flare();
			break;
		}

		break;

	case LG_READY:

		if (input & IN_ACTION)
			lara.mesh_ptrs[LM_HEAD] = meshes[objects[LARA_SCREAM].mesh_index + LM_HEAD * 2];
		else
			lara.mesh_ptrs[LM_HEAD] = meshes[objects[LARA].mesh_index + LM_HEAD * 2];

		if (camera.type != CINEMATIC_CAMERA && camera.type != LOOK_CAMERA && camera.type != HEAVY_CAMERA)
			camera.type = COMBAT_CAMERA;

		if (input & IN_ACTION)
		{
			if (get_current_ammo_pointer(lara.gun_type) == 0)
			{
				//SOUND_PlayEffect(SFX_LARA_RELOAD, &lara_item->pos, SFX_DEFAULT);
				lara.request_gun_type = WEAPON_PISTOLS;
				return;
			}
		}

		switch (lara.gun_type)
		{
		case WEAPON_PISTOLS:
		case WEAPON_UZI:
		case WEAPON_MAGNUMS:
		case WEAPON_AUTOPISTOLS:
			PistolHandler(lara.gun_type);
			break;

		case WEAPON_REVOLVER:
		case WEAPON_DESERTEAGLE:
		case WEAPON_SHOTGUN:
		case WEAPON_M16:
		case WEAPON_MP5:
		case WEAPON_ROCKET:
		case WEAPON_HARPOON:
		case WEAPON_GRAPPLING:
		case WEAPON_GRENADE:
		case WEAPON_CROSSBOW:
			RifleHandler(lara.gun_type);
			break;
		}

		break;

	case LG_FLARE:
		draw_flare();
		break;
	}
}
