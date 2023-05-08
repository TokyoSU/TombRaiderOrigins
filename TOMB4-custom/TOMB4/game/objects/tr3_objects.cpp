#include "pch.h"
#include "tr3_objects.h"
#include "collide.h"
#include "objects.h"
#include "fish_emitter.h"

void ObjectObjects(OBJECT_INFO* obj)
{
	obj = &objects[FISH_EMITTER];
	if (obj->loaded)
	{
		obj->initialise = InitialiseFish;
		obj->control = ControlFish;
		obj->draw_routine = S_DrawFish;
		obj->hit_points = NO_ITEM;
		obj->save_position = 1;
		obj->save_hitpoints = 1;
		obj->save_flags = 1;
	}

	obj = &objects[FISH_TARGET];
	if (obj->loaded)
	{
		obj->control = FishTargetControl;
		obj->collision = ObjectCollision;
		obj->save_position = 1;
		obj->save_hitpoints = 1;
		obj->save_flags = 1;
		obj->hit_effect = 1;
	}
}

void InitialiseObjects_TR3()
{
	OBJECT_INFO* obj = NULL;
	ObjectObjects(obj);
}
