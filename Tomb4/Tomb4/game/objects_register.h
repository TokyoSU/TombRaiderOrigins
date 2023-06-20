#pragma once

class ObjectRegisterFactory
{
public:
	ObjectRegisterFactory() = default;

public:
	ObjectRegisterFactory* From(int objID);
public:
	ObjectRegisterFactory* Initialise(Obj_InitialiseFunc func);
	ObjectRegisterFactory* Control(Obj_ControlFunc func);
	ObjectRegisterFactory* Collision(Obj_CollisionFunc func);
	ObjectRegisterFactory* Floor(Obj_FloorFunc func);
	ObjectRegisterFactory* Ceiling(Obj_CeilingFunc func);
	ObjectRegisterFactory* Draw(Obj_DrawRoutineFunc func);
	ObjectRegisterFactory* DrawExtra(Obj_DrawRoutineExtraFunc func);
public:
	ObjectRegisterFactory* FrameBase(short* frame);
	ObjectRegisterFactory* AnimIndexFromObject(OBJECTS_TYPES objID);
	ObjectRegisterFactory* HitPoints(short hitPoints);
	ObjectRegisterFactory* Radius(short radius);
	ObjectRegisterFactory* Pivot(short pivot);
	ObjectRegisterFactory* Shadow(short shadow);
	ObjectRegisterFactory* HitEffect(int hitEffect);
	ObjectRegisterFactory* ExplosionMesh(DWORD meshbits);
public:
	ObjectRegisterFactory* Intelligent();
	ObjectRegisterFactory* Undead();
	ObjectRegisterFactory* NonLot();
	ObjectRegisterFactory* Save(bool position, bool hitpoints, bool flags, bool anim, bool mesh);
	ObjectRegisterFactory* Semitransparent();
	ObjectRegisterFactory* WaterCreature();
	ObjectRegisterFactory* Pickup();
	ObjectRegisterFactory* Puzzlehole();
public:
	ObjectRegisterFactory* CreatureDefault(bool saveMesh = false);
	ObjectRegisterFactory* SaveDefault(bool saveMesh = false);
public:
	ObjectRegisterFactory* Bone(DWORD boneID, DWORD flags = BN_NONE);// Use BONE_FLAGS
	ObjectRegisterFactory* MeshFrom(DWORD meshID, OBJECTS_TYPES fromObj);
	ObjectRegisterFactory* InvSwitchMesh(DWORD meshID, OBJECTS_TYPES fromObj);
private:
	OBJECT_INFO* obj = nullptr;
};
