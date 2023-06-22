#include "pch.h"
#include "objects_register.h"
#include "objects.h"
#include "box.h"
#include "control.h"
#include "collide.h"
#include "pickup.h"
#include "init.h"

ObjectRegisterFactory* ObjectRegisterFactory::From(int objID)
{
	if (objID >= 0 && objID < NUMBER_OBJECTS)
		obj = &objects[objID];
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::LoadedByDefault()
{
	if (obj)
		obj->loaded = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::NoMeshes()
{
	if (obj && obj->loaded)
		obj->nmeshes = 0;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Initialise(Obj_InitialiseFunc func)
{
	if (obj && obj->loaded)
		obj->initialise = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Control(Obj_ControlFunc func)
{
	if (obj && obj->loaded)
		obj->control = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Collision(Obj_CollisionFunc func)
{
	if (obj && obj->loaded)
		obj->collision = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Floor(Obj_FloorFunc func)
{
	if (obj && obj->loaded)
		obj->floor = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Ceiling(Obj_CeilingFunc func)
{
	if (obj && obj->loaded)
		obj->ceiling = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Draw(Obj_DrawRoutineFunc func)
{
	if (obj && obj->loaded)
		obj->draw_routine = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::DrawExtra(Obj_DrawRoutineExtraFunc func)
{
	if (obj && obj->loaded)
		obj->draw_routine_extra = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::FrameBase(short* frame)
{
	if (obj && obj->loaded)
		obj->frame_base = (short*)((long)obj->frame_base + (char*)frame);
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::AnimIndexFromObject(OBJECTS_TYPES objID)
{
	if (obj && obj->loaded)
	{
		auto& from = objects[objID];
		if (from.loaded)
			obj->anim_index = from.anim_index;
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::HitPoints(short hitPoints)
{
	if (obj && obj->loaded)
		obj->hit_points = hitPoints;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Radius(short r)
{
	if (obj && obj->loaded)
		obj->r = r;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Pivot(short pivot)
{
	if (obj && obj->loaded)
		obj->pivot_length = pivot;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Shadow(short shadow)
{
	if (obj && obj->loaded)
		obj->shadow_size = shadow;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::HitEffect(int hitEffect)
{
	if (obj && obj->loaded)
		obj->hit_effect = hitEffect;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::ExplosionMesh(DWORD meshbits)
{
	if (obj && obj->loaded)
		obj->explodable_meshbits |= 1 << meshbits;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Intelligent()
{
	if (obj && obj->loaded)
		obj->intelligent = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Undead()
{
	if (obj && obj->loaded)
		obj->undead = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::NonLot()
{
	if (obj && obj->loaded)
		obj->non_lot = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Save(bool position, bool hitpoints, bool flags, bool anim, bool mesh)
{
	if (obj && obj->loaded)
	{
		obj->save_anim = anim;
		obj->save_flags = flags;
		obj->save_hitpoints = hitpoints;
		obj->save_mesh = mesh;
		obj->save_position = position;
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Semitransparent()
{
	if (obj && obj->loaded)
		obj->semi_transparent = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::WaterCreature()
{
	if (obj && obj->loaded)
		obj->water_creature = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Pickup()
{
	if (obj && obj->loaded)
		obj->is_pickup = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Puzzlehole()
{
	if (obj && obj->loaded)
		obj->is_puzzlehole = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::CreatureDefault(bool saveMesh)
{
	if (obj && obj->loaded)
	{
		obj->initialise = InitialiseCreature;
		obj->collision = CreatureCollision;
		Intelligent(); // Creature is intelligent by default !
		Shadow(128); // Default shadow size for entity.
		Pivot(0);
		Radius(102);
		HitPoints(1);
		HitEffect(1); // Blood
		Save(true, true, true, true, saveMesh);
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::SaveDefault(bool saveMesh)
{
	return Save(true, true, true, true, saveMesh);
}

ObjectRegisterFactory* ObjectRegisterFactory::Bone(DWORD boneID, DWORD flags)
{
	if (obj && obj->loaded)
		bones[obj->bone_index + boneID * 4] |= flags;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::MeshFrom(DWORD meshID, OBJECTS_TYPES fromObj)
{
	if (obj && obj->loaded)
	{
		auto& from = objects[fromObj];
		if (from.loaded)
			meshes[obj->mesh_index + meshID] = meshes[from.mesh_index + meshID];
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::InvSwitchMesh(DWORD meshID, OBJECTS_TYPES fromObj)
{
	if (obj && obj->loaded && (fromObj >= 0 && fromObj < NUMBER_OBJECTS))
	{
		auto& from = objects[fromObj];
		if (from.loaded)
			meshes[from.mesh_index + (meshID * 2)] = meshes[obj->mesh_index + (meshID * 2)];
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::PickupDefault()
{
	if (obj && obj->loaded)
	{
		obj->initialise = InitialisePickUp;
		obj->control = AnimatingPickUp;
		obj->collision = PickUpCollision;
		obj->is_pickup = true;
		obj->save_flags = true;
		obj->save_position = true;
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::KeyHoleDefault()
{
	if (obj && obj->loaded)
	{
		obj->collision = KeyHoleCollision;
		obj->save_flags = 1;
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::PuzzleHoleDefault()
{
	if (obj && obj->loaded)
	{
		obj->control = ControlAnimatingSlots;
		obj->collision = PuzzleHoleCollision;
		obj->is_puzzlehole = true;
		obj->save_flags = 1;
		obj->save_anim = 1;
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::PuzzleDoneDefault()
{
	if (obj && obj->loaded)
	{
		obj->control = ControlAnimatingSlots;
		obj->collision = PuzzleDoneCollision;
		obj->is_puzzlehole = false;
		obj->save_flags = 1;
		obj->save_anim = 1;
	}
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::AnimatingDefault(bool isCollidable)
{
	if (obj && obj->loaded)
	{
		obj->control = ControlAnimatingSlots;
		if (isCollidable)
		{
			obj->collision = ObjectCollision;
			obj->hit_effect = 2; // Ricochet
		}
		obj->save_flags = 1;
		obj->save_anim = 1;
	}
	return this;
}