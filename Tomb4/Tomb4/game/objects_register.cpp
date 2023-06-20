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
	if (objID < 0 || objID >= NUMBER_OBJECTS)
		return this;
	obj = &objects[objID];
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::LoadedByDefault()
{
	if (obj == nullptr)
		return this;
	obj->loaded = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::NoMeshes()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->nmeshes = 0;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Initialise(Obj_InitialiseFunc func)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->initialise = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Control(Obj_ControlFunc func)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->control = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Collision(Obj_CollisionFunc func)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->collision = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Floor(Obj_FloorFunc func)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->floor = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Ceiling(Obj_CeilingFunc func)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->ceiling = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Draw(Obj_DrawRoutineFunc func)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->draw_routine = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::DrawExtra(Obj_DrawRoutineExtraFunc func)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->draw_routine_extra = func;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::FrameBase(short* frame)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->frame_base = (short*)((long)obj->frame_base + (char*)frame);
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::AnimIndexFromObject(OBJECTS_TYPES objID)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	if (objects[objID].loaded)
		obj->anim_index = objects[objID].anim_index;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::HitPoints(short hitPoints)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->hit_points = hitPoints;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Radius(short radius)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->radius = radius;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Pivot(short pivot)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->pivot_length = pivot;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Shadow(short shadow)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->shadow_size = shadow;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::HitEffect(int hitEffect)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->hit_effect = hitEffect;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::ExplosionMesh(DWORD meshbits)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->explodable_meshbits |= 1 << meshbits;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Intelligent()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->intelligent = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Undead()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->undead = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::NonLot()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->non_lot = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Save(bool position, bool hitpoints, bool flags, bool anim, bool mesh)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->save_anim = anim;
	obj->save_flags = flags;
	obj->save_hitpoints = hitpoints;
	obj->save_mesh = mesh;
	obj->save_position = position;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Semitransparent()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->semi_transparent = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::WaterCreature()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->water_creature = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Pickup()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->is_pickup = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::Puzzlehole()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->is_puzzlehole = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::CreatureDefault(bool saveMesh)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->initialise = InitialiseCreature;
	obj->collision = CreatureCollision;
	Intelligent(); // Creature is intelligent by default !
	Shadow(128); // Default shadow size for entity.
	Pivot(0);
	Radius(102);
	HitPoints(1);
	HitEffect(1); // Blood
	Save(true, true, true, true, saveMesh);
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::SaveDefault(bool saveMesh)
{
	return Save(true, true, true, true, saveMesh);
}

ObjectRegisterFactory* ObjectRegisterFactory::Bone(DWORD boneID, DWORD flags)
{
	if (obj == nullptr || !obj->loaded || flags == BN_NONE)
		return this;
	bones[obj->bone_index + boneID * 4] |= flags;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::MeshFrom(DWORD meshID, OBJECTS_TYPES fromObj)
{
	if (obj == nullptr || !obj->loaded || (fromObj < 0 || fromObj >= NUMBER_OBJECTS))
		return this;
	if (objects[fromObj].loaded)
		meshes[obj->mesh_index + meshID] = meshes[objects[fromObj].mesh_index + meshID];
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::InvSwitchMesh(DWORD meshID, OBJECTS_TYPES fromObj)
{
	if (obj == nullptr || !obj->loaded || (fromObj < 0 || fromObj >= NUMBER_OBJECTS))
		return this;
	if (objects[fromObj].loaded)
		meshes[objects[fromObj].mesh_index + (meshID * 2)] = meshes[obj->mesh_index + (meshID * 2)];
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::PickupDefault()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->initialise = InitialisePickUp;
	obj->control = AnimatingPickUp;
	obj->collision = PickUpCollision;
	obj->is_pickup = true;
	obj->save_flags = true;
	obj->save_position = true;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::KeyHoleDefault()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->collision = KeyHoleCollision;
	obj->save_flags = 1;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::PuzzleHoleDefault()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->control = ControlAnimatingSlots;
	obj->collision = PuzzleHoleCollision;
	obj->is_puzzlehole = true;
	obj->save_flags = 1;
	obj->save_anim = 1;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::PuzzleDoneDefault()
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->control = ControlAnimatingSlots;
	obj->collision = PuzzleDoneCollision;
	obj->is_puzzlehole = false;
	obj->save_flags = 1;
	obj->save_anim = 1;
	return this;
}

ObjectRegisterFactory* ObjectRegisterFactory::AnimatingDefault(bool isCollidable)
{
	if (obj == nullptr || !obj->loaded)
		return this;
	obj->control = ControlAnimatingSlots;
	if (isCollidable)
	{
		obj->collision = ObjectCollision;
		obj->hit_effect = 2; // Ricochet
	}
	obj->save_flags = 1;
	obj->save_anim = 1;
	return this;
}
