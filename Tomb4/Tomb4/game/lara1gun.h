#pragma once

void DoGrenadeDamageOnBaddie(ItemInfo* baddie, ItemInfo* item);
void FireCrossbow(PHD_3DPOS* pos);
void draw_shotgun_meshes(long weapon_type);
void undraw_shotgun_meshes(long weapon_type);
void ready_shotgun(long weapon_type);
void FireShotgun();
void FireGrenade();
void AnimateShotgun(long weapon_type);
void RifleHandler(long weapon_type);
void CrossbowHitSwitchType78(ItemInfo* item, ItemInfo* target, long MustHitLastNode);
void TriggerUnderwaterExplosion(ItemInfo* item, long vehicle);
void draw_shotgun(long weapon_type);
void undraw_shotgun(long weapon_type);
void ControlCrossbow(short item_number);
void ControlGrenade(short item_number);
