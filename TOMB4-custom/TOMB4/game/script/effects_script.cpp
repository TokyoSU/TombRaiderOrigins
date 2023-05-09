#include "pch.h"
#include "effects_script.h"
#include "control.h"
#include "effects.h"
#include "effect2.h"
#include "scripting.h"
#include "objects.h"
#include "people.h"
#include "box.h"
#include "lara.h"
#include "tomb4fx.h"
#include "sphere.h"
#include "function_stubs.h"

static void DamageLara(short item_number, int damage, HIT_EFFECTS hitType, BITE_INFO bite)
{
	auto* item = &items[item_number];
	GAME_VECTOR pos;
	pos.x = bite.x;
	pos.y = bite.y;
	pos.z = bite.z;
	pos.room_number = item->room_number;

	lara_item->hit_points -= damage;
	lara_item->hit_status = 1;
	switch (hitType)
	{
	case HE_BLOOD:
		CreatureEffect(item, &bite, DoBloodSplat);
		break;
	case HE_RICHOCHET:
		GetJointAbsPosition(item, (PHD_VECTOR*)&pos, bite.mesh_num);
		TriggerRicochetSpark(&pos, item->pos.y_rot, 3, 0);
		break;
	case HE_SMOKE:
		GetJointAbsPosition(item, (PHD_VECTOR*)&pos, bite.mesh_num);
		for (int i = 0; i < 4; i++)
			TriggerGunSmoke(pos.x, pos.y, pos.z, 0, 0, 0, 0, WEAPON_PISTOLS, 32);
		break;
	default:
		Log(2, "This effect type is unknown: %d", hitType);
		break;
	}
}

void RegisterEffectsHelper()
{
	// Enums:
	assert(scriptEngine->RegisterEnum("HIT_EFFECT") >= 0);
	assert(scriptEngine->RegisterEnumValue("HIT_EFFECT", "HE_BLOOD", HE_BLOOD) >= 0);
	assert(scriptEngine->RegisterEnumValue("HIT_EFFECT", "HE_SMOKE", HE_SMOKE) >= 0);
	assert(scriptEngine->RegisterEnumValue("HIT_EFFECT", "HE_RICHOCHET", HE_RICHOCHET) >= 0);

	// Functions:
	assert(scriptEngine->RegisterGlobalFunction("void DamageLara(int16,int,HIT_EFFECT,BITE_INFO)", asFUNCTION(DamageLara), asCALL_CDECL) >= 0);
}
