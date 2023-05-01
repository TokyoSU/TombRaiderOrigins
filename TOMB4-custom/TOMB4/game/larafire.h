#pragma once
#include "../global/types.h"

void InitialiseWeaponArray();
void InitialiseNewWeapon();
void LaraTargetInfo(WEAPON_INFO* winfo);
short& get_current_ammo_pointer(long weapon_type);
long FireWeapon(long weapon_type, ITEM_INFO* target, ITEM_INFO* src, short* angles);
void AimWeapon(WEAPON_INFO* winfo, LARA_ARM* arm);
void LaraGetNewTarget(WEAPON_INFO* winfo);
void HitTarget(ITEM_INFO* item, GAME_VECTOR* hitpos, long damage, long grenade);
long GetWeaponMeshObject(long weapon_type);
long GetWeaponAnimObject(long weapon_type);
long GetWeaponHolsterObject(long weapon_type);
void DoProperDetection(short item_number, long x, long y, long z, long xv, long yv, long zv);
void LaraGun();

extern std::vector<WEAPON_INFO> weapons;
