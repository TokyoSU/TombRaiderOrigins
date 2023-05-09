#include "pch.h"
#include "item_script.h"
#include "control.h"
#include "scripting.h"
#include "objects.h"

static int GetItemObjectNumber(short item_number)
{
	return items[item_number].object_number;
}

static short GetItemCurrentState(short item_number)
{
	return items[item_number].current_anim_state;
}

static void SetItemCurrentState(short item_number, int state)
{
	items[item_number].current_anim_state = (short)state;
}

static short GetItemGoalState(short item_number)
{
	return items[item_number].goal_anim_state;
}

static void SetItemGoalState(short item_number, int state)
{
	items[item_number].goal_anim_state = (short)state;
}

static short GetItemRequiredState(short item_number)
{
	return items[item_number].required_anim_state;
}

static void SetItemRequiredState(short item_number, int state)
{
	items[item_number].required_anim_state = (short)state;
}

static short GetItemAnimNumber(short item_number)
{
	return items[item_number].anim_number;
}

static short GetItemFrameNumber(short item_number)
{
	return items[item_number].frame_number;
}

static short GetItemRoomNumber(short item_number)
{
	return items[item_number].room_number;
}

static int GetItemHeightFromFloor(short item_number)
{
	return items[item_number].floor;
}

static void IncreaseItemHealth(short item_number, short hp)
{
	items[item_number].hit_points += hp;
}

static void DecreaseItemHealth(short item_number, short hp)
{
	items[item_number].hit_points -= hp;
}

static void SetItemHealth(short item_number, short hp)
{
	items[item_number].hit_points = hp;
}

static short GetItemHeath(short item_number)
{
	return items[item_number].hit_points;
}

static bool IsItemAlive(short item_number)
{
	return items[item_number].hit_points > 0;
}

static bool IsItemDead(short item_number)
{
	return items[item_number].hit_points <= 0;
}

static short GetItemOcb(short item_number)
{
	return items[item_number].ocb;
}

static short GetItemSpeed(short item_number)
{
	return items[item_number].speed;
}

static short GetItemFallspeed(short item_number)
{
	return items[item_number].fallspeed;
}

static bool IsItemActive(short item_number)
{
	return items[item_number].active;
}

static short IsItemStatus(short item_number)
{
	return items[item_number].status;
}

static bool IsItemGravityEnabled(short item_number)
{
	return items[item_number].gravity_status;
}

static void SetItemFiredWeapon(short item_number, short fired_count)
{
	items[item_number].fired_weapon = fired_count;
}

static short GetItemFiredWeaponCount(short item_number)
{
	return items[item_number].fired_weapon;
}

static bool IsItemCollisionEnabled(short item_number)
{
	return items[item_number].collidable;
}

static bool IsItemReallyActiveEnabled(short item_number)
{
	return items[item_number].really_active;
}

static bool IsItemPoisoned(short item_number)
{
	return items[item_number].poisoned;
}

static bool IsItemHit(short item_number)
{
	return items[item_number].hit_status;
}

static bool IsItemLookedAt(short item_number)
{
	return items[item_number].looked_at;
}

static short GetItemFlags(short item_number)
{
	return items[item_number].flags;
}

static unsigned int GetItemTouchBits(short item_number)
{
	return items[item_number].touch_bits;
}

static unsigned int GetItemMeshBits(short item_number)
{
	return items[item_number].mesh_bits;
}

static unsigned int GetItemMeshswapMeshBits(short item_number)
{
	return items[item_number].meshswap_meshbits;
}

static unsigned short GetItemAIBits(short item_number)
{
	return items[item_number].ai_bits;
}

static short GetItemStorageValue(short item_number, short index)
{
	if (index < 0 || index >= ITEM_INFO_FLAG_COUNT)
		return 0;
	return items[item_number].item_flags[index];
}

static void SetItemStorageValue(short item_number, short index, short value)
{
	if (index < 0 || index >= ITEM_INFO_FLAG_COUNT)
		return;
	items[item_number].item_flags[index] = value;
}

static PHD_3DPOS GetItemPosition(short item_number)
{
	return items[item_number].pos;
}

static void SetItemAnimation(short item_number, int anim_id)
{
	auto& item = items[item_number];
	item.anim_number = objects[item.object_number].anim_index + anim_id;
	auto& anim = anims[item.anim_number];
	item.frame_number = anim.frame_base;
	item.current_anim_state = anim.current_anim_state;
	item.goal_anim_state = item.current_anim_state;
}

static void SetItemAnimationAndFrame(short item_number, short anim_id, short frame_start)
{
	auto& item = items[item_number];
	item.anim_number = objects[item.object_number].anim_index + anim_id;
	auto& anim = anims[item.anim_number];
	item.frame_number = anim.frame_base + frame_start;
	item.current_anim_state = anim.current_anim_state;
	item.goal_anim_state = anim.current_anim_state;
}

static bool IsItemMeshCollidingWithTarget(short item_number, int mesh_num)
{
	return items[item_number].touch_bits & (1 << mesh_num);
}

void RegisterItemHelper()
{
	scriptEngine->RegisterGlobalFunction("int GetItemHeightFromFloor(int16)", asFUNCTION(GetItemHeightFromFloor), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int GetItemObjectNumber(int16)", asFUNCTION(GetItemObjectNumber), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemCurrentState(int16)", asFUNCTION(GetItemCurrentState), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemGoalState(int16)", asFUNCTION(GetItemGoalState), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemRequiredState(int16)", asFUNCTION(GetItemRequiredState), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemCurrentState(int16,int)", asFUNCTION(SetItemCurrentState), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemGoalState(int16,int)", asFUNCTION(SetItemGoalState), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemRequiredState(int16,int)", asFUNCTION(SetItemRequiredState), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemAnimNumber(int16)", asFUNCTION(GetItemAnimNumber), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemFrameNumber(int16)", asFUNCTION(GetItemFrameNumber), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemRoomNumber(int16)", asFUNCTION(GetItemRoomNumber), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void IncreaseItemHealth(int16,int16)", asFUNCTION(IncreaseItemHealth), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void DecreaseItemHealth(int16,int16)", asFUNCTION(DecreaseItemHealth), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemHealth(int16,int16)", asFUNCTION(SetItemHealth), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemHeath(int16)", asFUNCTION(GetItemHeath), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemAlive(int16)", asFUNCTION(IsItemAlive), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemDead(int16)", asFUNCTION(IsItemDead), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemOcb(int16)", asFUNCTION(GetItemOcb), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemSpeed(int16)", asFUNCTION(GetItemSpeed), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemFallspeed(int16)", asFUNCTION(GetItemFallspeed), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemActive(int16)", asFUNCTION(IsItemActive), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 IsItemStatus(int16)", asFUNCTION(IsItemStatus), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemGravityEnabled(int16)", asFUNCTION(IsItemGravityEnabled), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemFiredWeapon(int16,int16)", asFUNCTION(SetItemFiredWeapon), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemCollisionEnabled(int16)", asFUNCTION(IsItemCollisionEnabled), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemReallyActiveEnabled(int16)", asFUNCTION(IsItemReallyActiveEnabled), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemPoisoned(int16)", asFUNCTION(IsItemPoisoned), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemHit(int16)", asFUNCTION(IsItemHit), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemLookedAt(int16)", asFUNCTION(IsItemLookedAt), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemFlags(int16)", asFUNCTION(GetItemFlags), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("uint64 GetItemTouchBits(int16)", asFUNCTION(GetItemTouchBits), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("uint64 GetItemMeshBits(int16)", asFUNCTION(GetItemMeshBits), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("uint64 GetItemMeshswapMeshBits(int16)", asFUNCTION(GetItemMeshswapMeshBits), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("uint16 GetItemAIBits(int16)", asFUNCTION(GetItemAIBits), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("int16 GetItemStorageValue(int16,int16)", asFUNCTION(GetItemStorageValue), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemStorageValue(int16,int16,int16)", asFUNCTION(SetItemStorageValue), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("PHD_3DPOS GetItemPosition(int16)", asFUNCTION(SetItemStorageValue), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemAnimation(int16,int16)", asFUNCTION(SetItemAnimation), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void SetItemAnimationAndFrame(int16,int16,int16)", asFUNCTION(SetItemAnimationAndFrame), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("bool IsItemMeshCollidingWithTarget(int16,int)", asFUNCTION(IsItemMeshCollidingWithTarget), asCALL_CDECL);
}
