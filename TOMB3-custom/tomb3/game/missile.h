#pragma once
#include "../global/types.h"

void ControlMissile(short fx_number);
void ShootAtLara(FX_INFO* fx);
long ExplodingDeath(short item_number, long mesh_bits, short counter, short damage = 0);
void ControlBodyPart(short fx_number);
