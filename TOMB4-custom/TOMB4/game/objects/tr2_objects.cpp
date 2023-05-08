#include "pch.h"
#include "tr2_objects.h"
#include "objects.h"
#include "collide.h"

#include "tr2_rat.h"

static void BaddyObjects(OBJECT_INFO* obj)
{
	obj = &objects[TR2_RAT];
	if (obj->loaded)
	{
		obj->initialise = InitialiseRat_TR2;
		obj->control = ControlRat_TR2;
		obj->collision = CreatureCollision;
		obj->shadow_size = 64;
		obj->radius = 128;
		obj->intelligent = 1;
		obj->hit_points = 8;
		obj->save_position = 1;
		obj->save_anim = 1;
		obj->save_flags = 1;
		obj->save_hitpoints = 1;
		obj->hit_effect = 1;
		obj->initialise_script = InitialiseRat_TR2_Script;
		obj->release_script = ReleaseRat_TR2_Script;
	}
}

static void ObjectObjects(OBJECT_INFO* obj)
{

}

static void TrapObjects(OBJECT_INFO* obj)
{

}

void InitialiseObjects_TR2()
{
	OBJECT_INFO* obj = NULL;
	BaddyObjects(obj);
	ObjectObjects(obj);
	TrapObjects(obj);
}
