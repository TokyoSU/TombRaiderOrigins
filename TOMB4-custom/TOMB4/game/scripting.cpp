#include "pch.h"
#include "scripting.h"
#include "box.h"
#include "function_stubs.h"
#include "item_script.h"
#include "creature_script.h"
#include "effects_script.h"

asIScriptEngine* scriptEngine;
CContextMgr scriptManager;
CScriptBuilder scriptBuilder;

static void MessageCallback(const asSMessageInfo* msg, void* param)
{
	switch (msg->type)
	{
	case asMSGTYPE_INFORMATION:
		Log(1, "[SCRIPTING][INFO] From script: %d, (%d, %d), msg: %s", msg->section, msg->row, msg->col, msg->message);
		break;
	case asMSGTYPE_WARNING:
		Log(2, "[SCRIPTING][WARNING] From script: %d, (%d, %d), msg: %s", msg->section, msg->row, msg->col, msg->message);
		break;
	case asMSGTYPE_ERROR:
		Log(3, "[SCRIPTING][ERROR] From script: %d, (%d, %d), msg: %s", msg->section, msg->row, msg->col, msg->message);
		break;
	}
}

void RegisterStructure_PHDVECTOR()
{
	assert(scriptEngine->RegisterObjectType("PHD_VECTOR", sizeof(PHD_VECTOR), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_VECTOR", "int64 x", asOFFSET(PHD_VECTOR, x)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_VECTOR", "int64 y", asOFFSET(PHD_VECTOR, y)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_VECTOR", "int64 z", asOFFSET(PHD_VECTOR, z)) >= 0);
}

void RegisterStructure_GAMEVECTOR()
{
	assert(scriptEngine->RegisterObjectType("GAME_VECTOR", sizeof(GAME_VECTOR), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS) >= 0);
	assert(scriptEngine->RegisterObjectProperty("GAME_VECTOR", "int64 x", asOFFSET(GAME_VECTOR, x)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("GAME_VECTOR", "int64 y", asOFFSET(GAME_VECTOR, y)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("GAME_VECTOR", "int64 z", asOFFSET(GAME_VECTOR, z)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("GAME_VECTOR", "int16 room_number", asOFFSET(GAME_VECTOR, room_number)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("GAME_VECTOR", "int16 box_number", asOFFSET(GAME_VECTOR, box_number)) >= 0);
}

void RegisterStructure_PHD3DPOS()
{
	assert(scriptEngine->RegisterObjectType("PHD_3DPOS", sizeof(PHD_3DPOS), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_3DPOS", "int64 x_pos", asOFFSET(PHD_3DPOS, x_pos)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_3DPOS", "int64 y_pos", asOFFSET(PHD_3DPOS, y_pos)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_3DPOS", "int64 z_pos", asOFFSET(PHD_3DPOS, z_pos)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_3DPOS", "int16 x_rot", asOFFSET(PHD_3DPOS, x_rot)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_3DPOS", "int16 y_rot", asOFFSET(PHD_3DPOS, y_rot)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("PHD_3DPOS", "int16 z_rot", asOFFSET(PHD_3DPOS, z_rot)) >= 0);
}

void RegisterStructure_BITEINFO()
{
	assert(scriptEngine->RegisterObjectType("BITE_INFO", sizeof(BITE_INFO), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS) >= 0);
	assert(scriptEngine->RegisterObjectProperty("BITE_INFO", "int x", asOFFSET(BITE_INFO, x)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("BITE_INFO", "int y", asOFFSET(BITE_INFO, y)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("BITE_INFO", "int z", asOFFSET(BITE_INFO, z)) >= 0);
	assert(scriptEngine->RegisterObjectProperty("BITE_INFO", "int mesh_num", asOFFSET(BITE_INFO, mesh_num)) >= 0);
}

/*void RegisterStructure_ITEMINFO()
{
	int r = scriptEngine->RegisterObjectType("ITEM_INFO", sizeof(ITEM_INFO), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 index", asOFFSET(ITEM_INFO, index)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int floor", asOFFSET(ITEM_INFO, floor)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "uint touch_bits", asOFFSET(ITEM_INFO, touch_bits)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "uint mesh_bits", asOFFSET(ITEM_INFO, mesh_bits)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 object_number", asOFFSET(ITEM_INFO, object_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 current_anim_state", asOFFSET(ITEM_INFO, current_anim_state)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 goal_anim_state", asOFFSET(ITEM_INFO, goal_anim_state)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 required_anim_state", asOFFSET(ITEM_INFO, required_anim_state)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 anim_number", asOFFSET(ITEM_INFO, anim_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 frame_number", asOFFSET(ITEM_INFO, frame_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 room_number", asOFFSET(ITEM_INFO, room_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 next_item", asOFFSET(ITEM_INFO, next_item)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 next_active", asOFFSET(ITEM_INFO, next_active)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 speed", asOFFSET(ITEM_INFO, speed)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 fallspeed", asOFFSET(ITEM_INFO, fallspeed)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 hit_points", asOFFSET(ITEM_INFO, hit_points)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "uint16 box_number", asOFFSET(ITEM_INFO, box_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 timer", asOFFSET(ITEM_INFO, timer)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 flags", asOFFSET(ITEM_INFO, flags)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 shade", asOFFSET(ITEM_INFO, shade)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 ocb", asOFFSET(ITEM_INFO, ocb)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 carried_item", asOFFSET(ITEM_INFO, carried_item)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 after_death", asOFFSET(ITEM_INFO, after_death)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "uint16 fired_weapon", asOFFSET(ITEM_INFO, fired_weapon)); assert(r >= 0);
	for (int i = 0; i < ITEM_INFO_FLAG_COUNT; i++)
	{
		r = scriptEngine->RegisterObjectProperty("ITEM_INFO", std::string("int16 item_flags" + std::to_string(i)).c_str(), asOFFSET(ITEM_INFO, item_flags[i])); assert(r >= 0);
	}
	//r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "any& data", asOFFSET(ITEM_INFO, data)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "PHD_3DPOS pos", asOFFSET(ITEM_INFO, pos)); assert(r >= 0);
	// NOTE: There is ITEMLIGHT which is really big, it's not required for scripting so it's ignored !
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "bool active", asOFFSET(ITEM_INFO, active)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 status", asOFFSET(ITEM_INFO, status)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "bool gravity_status", asOFFSET(ITEM_INFO, gravity_status)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "bool hit_status", asOFFSET(ITEM_INFO, hit_status)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "bool looked_at", asOFFSET(ITEM_INFO, looked_at)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 dynamic_light", asOFFSET(ITEM_INFO, dynamic_light)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "bool poisoned", asOFFSET(ITEM_INFO, poisoned)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "uint16 ai_bits", asOFFSET(ITEM_INFO, ai_bits)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "bool really_active", asOFFSET(ITEM_INFO, really_active)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "uint meshswap_meshbits", asOFFSET(ITEM_INFO, meshswap_meshbits)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "uint draw_room", asOFFSET(ITEM_INFO, draw_room)); assert(r >= 0);
	//r = scriptEngine->RegisterObjectProperty("ITEM_INFO", "int16 tosspad", asOFFSET(ITEM_INFO, meshswap_meshbits)); assert(r >= 0); // there is tosspad here, but it's not used anywhere so commented !
}

void RegisterStructure_COLLINFO()
{
	int r = scriptEngine->RegisterObjectType("COLL_INFO", sizeof(COLL_INFO), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 mid_floor", asOFFSET(COLL_INFO, mid_floor)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 mid_ceiling", asOFFSET(COLL_INFO, mid_ceiling)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 mid_type", asOFFSET(COLL_INFO, mid_type)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 front_floor", asOFFSET(COLL_INFO, front_floor)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 front_ceiling", asOFFSET(COLL_INFO, front_ceiling)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 front_type", asOFFSET(COLL_INFO, front_type)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 left_floor", asOFFSET(COLL_INFO, left_floor)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 left_ceiling", asOFFSET(COLL_INFO, left_ceiling)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 left_type", asOFFSET(COLL_INFO, left_type)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 right_floor", asOFFSET(COLL_INFO, right_floor)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 right_ceiling", asOFFSET(COLL_INFO, right_ceiling)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 right_type", asOFFSET(COLL_INFO, right_type)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 left_floor2", asOFFSET(COLL_INFO, left_floor2)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 left_ceiling2", asOFFSET(COLL_INFO, left_ceiling2)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 left_type2", asOFFSET(COLL_INFO, left_type2)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 right_floor2", asOFFSET(COLL_INFO, right_floor2)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 right_ceiling2", asOFFSET(COLL_INFO, right_ceiling2)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 right_type2", asOFFSET(COLL_INFO, right_type2)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 radius", asOFFSET(COLL_INFO, radius)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 bad_pos", asOFFSET(COLL_INFO, bad_pos)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 bad_neg", asOFFSET(COLL_INFO, bad_neg)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int64 bad_ceiling", asOFFSET(COLL_INFO, bad_ceiling)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "PHD_VECTOR shift", asOFFSET(COLL_INFO, shift)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "PHD_VECTOR old", asOFFSET(COLL_INFO, old)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int16 old_anim_state", asOFFSET(COLL_INFO, old_anim_state)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int16 old_anim_number", asOFFSET(COLL_INFO, old_anim_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int16 old_frame_number", asOFFSET(COLL_INFO, old_frame_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int16 facing", asOFFSET(COLL_INFO, facing)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int16 quadrant", asOFFSET(COLL_INFO, quadrant)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int16 coll_type", asOFFSET(COLL_INFO, coll_type)); assert(r >= 0);
	//r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int16& trigger", asOFFSET(COLL_INFO, trigger)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int8 tilt_x", asOFFSET(COLL_INFO, tilt_x)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "int8 tilt_z", asOFFSET(COLL_INFO, tilt_z)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool hit_by_baddie", asOFFSET(COLL_INFO, hit_by_baddie)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool hit_static", asOFFSET(COLL_INFO, hit_static)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool slopes_are_walls", asOFFSET(COLL_INFO, slopes_are_walls)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool slopes_are_pits", asOFFSET(COLL_INFO, slopes_are_pits)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool lava_is_pit", asOFFSET(COLL_INFO, lava_is_pit)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool enable_baddie_push", asOFFSET(COLL_INFO, enable_baddie_push)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool enable_spaz", asOFFSET(COLL_INFO, enable_spaz)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("COLL_INFO", "bool hit_ceiling", asOFFSET(COLL_INFO, hit_ceiling)); assert(r >= 0);
}

void RegisterStructure_LOTINFO()
{
	int r = scriptEngine->RegisterObjectType("LOT_INFO", sizeof(LOT_INFO), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 head", asOFFSET(LOT_INFO, head)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 tail", asOFFSET(LOT_INFO, tail)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "uint16 search_number", asOFFSET(LOT_INFO, search_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "uint16 block_mask", asOFFSET(LOT_INFO, block_mask)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 step", asOFFSET(LOT_INFO, step)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 drop", asOFFSET(LOT_INFO, drop)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 zone_count", asOFFSET(LOT_INFO, zone_count)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 target_box", asOFFSET(LOT_INFO, target_box)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 required_box", asOFFSET(LOT_INFO, required_box)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int16 fly", asOFFSET(LOT_INFO, fly)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "bool can_jump", asOFFSET(LOT_INFO, can_jump)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "bool can_monkey", asOFFSET(LOT_INFO, can_monkey)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "bool is_amphibious", asOFFSET(LOT_INFO, is_amphibious)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "bool is_jumping", asOFFSET(LOT_INFO, is_jumping)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "bool is_monkeying", asOFFSET(LOT_INFO, is_monkeying)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "PHD_VECTOR target", asOFFSET(LOT_INFO, target)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("LOT_INFO", "int zone", asOFFSET(LOT_INFO, zone)); assert(r >= 0);
}

void RegisterStructure_AIINFO()
{
	int r = scriptEngine->RegisterObjectType("AI_INFO", sizeof(AI_INFO), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "int16 zone_number", asOFFSET(AI_INFO, zone_number)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "int16 enemy_zone", asOFFSET(AI_INFO, enemy_zone)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "int64 distance", asOFFSET(AI_INFO, distance)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "bool ahead", asOFFSET(AI_INFO, ahead)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "bool bite", asOFFSET(AI_INFO, bite)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "int16 angle", asOFFSET(AI_INFO, angle)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "int16 x_angle", asOFFSET(AI_INFO, x_angle)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("AI_INFO", "int16 enemy_facing", asOFFSET(AI_INFO, enemy_facing)); assert(r >= 0);
}

void RegisterStructure_CREATUREINFO()
{
	int r = scriptEngine->RegisterObjectType("CREATURE_INFO", sizeof(CREATURE_INFO), asOBJ_VALUE|asOBJ_POD|asOBJ_APP_CLASS); assert(r >= 0);
	for (int i = 0; i < CREATURE_JOINT_ROTATION_COUNT; i++)
	{
		r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", std::string("int16 joint_rotation" + std::to_string(i)).c_str(), asOFFSET(CREATURE_INFO, joint_rotation[i])); assert(r >= 0);
	}
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "int16 maximum_turn", asOFFSET(CREATURE_INFO, maximum_turn)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "int16 flags", asOFFSET(CREATURE_INFO, flags)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool alerted", asOFFSET(CREATURE_INFO, alerted)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool head_left", asOFFSET(CREATURE_INFO, head_left)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool head_right", asOFFSET(CREATURE_INFO, head_right)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool reached_goal", asOFFSET(CREATURE_INFO, reached_goal)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool hurt_by_lara", asOFFSET(CREATURE_INFO, hurt_by_lara)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool patrol2", asOFFSET(CREATURE_INFO, patrol2)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool jump_ahead", asOFFSET(CREATURE_INFO, jump_ahead)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "bool monkey_ahead", asOFFSET(CREATURE_INFO, monkey_ahead)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "int mood", asOFFSET(CREATURE_INFO, mood)); assert(r >= 0);
	//r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "ITEM_INFO& enemy", asOFFSET(CREATURE_INFO, enemy)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "ITEM_INFO ai_target", asOFFSET(CREATURE_INFO, ai_target)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "int16 item_num", asOFFSET(CREATURE_INFO, item_num)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "PHD_VECTOR target", asOFFSET(CREATURE_INFO, target)); assert(r >= 0);
	r = scriptEngine->RegisterObjectProperty("CREATURE_INFO", "LOT_INFO LOT", asOFFSET(CREATURE_INFO, LOT)); assert(r >= 0);
}

void RegisterFunction_BOX()
{
	int r = scriptEngine->RegisterGlobalFunction("void CreatureDie(int16, int64)", asFUNCTION(CreatureDie), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void InitialiseCreature(int16)", asFUNCTION(InitialiseCreature), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("bool CreatureActive(int16)", asFUNCTION(CreatureActive), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureAIInfo(ITEM_INFO item, AI_INFO ai)", asFUNCTION(CreatureAIInfo), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 SearchLOT(LOT_INFO lot, int64 expansion)", asFUNCTION(SearchLOT), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 UpdateLOT(LOT_INFO lot, int64 expansion)", asFUNCTION(UpdateLOT), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 TargetBox(LOT_INFO lot, int16 box_number)", asFUNCTION(TargetBox), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 EscapeBox(ITEM_INFO item, ITEM_INFO enemy, int16 box_number)", asFUNCTION(EscapeBox), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 ValidBox(ITEM_INFO item, int16 zone_number, int16 box_number)", asFUNCTION(ValidBox), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 StalkBox(ITEM_INFO item, ITEM_INFO enemy, int16 box_number)", asFUNCTION(StalkBox), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 CalculateTarget(PHD_VECTOR target, ITEM_INFO item, LOT_INFO lot)", asFUNCTION(CalculateTarget), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureMood(ITEM_INFO item, AI_INFO ai, int64 violent)", asFUNCTION(CreatureMood), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void GetCreatureMood(ITEM_INFO item, AI_INFO ai, int64 violent)", asFUNCTION(GetCreatureMood), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 CreatureCreature(int16 item_number)", asFUNCTION(CreatureCreature), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 BadFloor(int64 x, int64 y, int64 z, int64 box_height, int64 next_height, int16 room_number, LOT_INFO LOT)", asFUNCTION(BadFloor), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 CreatureAnimation(int16 item_number, int16 angle, int16 tilt)", asFUNCTION(CreatureAnimation), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int16 CreatureTurn(ITEM_INFO item, int16 maximum_turn)", asFUNCTION(CreatureTurn), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureTilt(ITEM_INFO item, int16 angle)", asFUNCTION(CreatureTilt), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureJoint(ITEM_INFO item, int16 joint, int16 required)", asFUNCTION(CreatureJoint), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureFloat(int16 item_number)", asFUNCTION(CreatureFloat), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureUnderwater(ITEM_INFO item, int64 depth)", asFUNCTION(CreatureUnderwater), asCALL_CDECL);
	// CreatureEffect()
	// CreatureEffectT()
	r = scriptEngine->RegisterGlobalFunction("int64 CreatureVault(int16 item_number, int16 angle, int16 vault, int16 shift)", asFUNCTION(CreatureVault), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureKill(ITEM_INFO item, int16 kill_anim, int16 kill_state, int16 lara_anim)", asFUNCTION(CreatureVault), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void AlertAllGuards(int16 item_number)", asFUNCTION(AlertAllGuards), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void AlertNearbyGuards(ITEM_INFO item)", asFUNCTION(AlertNearbyGuards), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int16 AIGuard(CREATURE_INFO creature)", asFUNCTION(AIGuard), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void FindAITargetObject(CREATURE_INFO creature, int16 object_number)", asFUNCTION(FindAITargetObject), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void GetAITarget(CREATURE_INFO creature)", asFUNCTION(GetAITarget), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("void CreatureYRot(PHD_3DPOS, int16, int16)", asFUNCTION(CreatureYRot), asCALL_CDECL);
	r = scriptEngine->RegisterGlobalFunction("int64 MoveCreature3DPos(PHD_3DPOS, PHD_3DPOS, int64, int16, int64)", asFUNCTION(MoveCreature3DPos), asCALL_CDECL);
}*/

void RegisterFunction_FunctionStubs()
{
	assert(scriptEngine->RegisterGlobalFunction("int64 GetRandomControl()", asFUNCTION(GetRandomControl), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int64 GetRandomDraw()", asFUNCTION(GetRandomDraw), asCALL_CDECL) >= 0);
}

void RegisterVariablesGlobal()
{
	
}

long SquaredBlock(long value, long div)
{
	if (div == 0)
		div = 1;
	return SQUARE((1024 * value) / div);
}

void InitialiseScripting()
{
	scriptEngine = asCreateScriptEngine();
	assert(scriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL) >= 0);
	assert(scriptBuilder.StartNewModule(scriptEngine, "tomb4") >= 0);

	// Basic register from angelscript.

	RegisterStdString(scriptEngine);
	RegisterExceptionRoutines(scriptEngine);
	RegisterScriptMath(scriptEngine);
	RegisterScriptMathComplex(scriptEngine);
	RegisterScriptFile(scriptEngine);
	RegisterScriptArray(scriptEngine, false);
	RegisterScriptHandle(scriptEngine);
	RegisterScriptDictionary(scriptEngine);
	RegisterScriptAny(scriptEngine);
	RegisterScriptDateTime(scriptEngine);
	RegisterStdStringUtils(scriptEngine);

	// Custom register
	assert(scriptEngine->RegisterGlobalFunction("void Log2(string)", asFUNCTIONPR(Log2, (std::string), void), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void Log2(int16)", asFUNCTIONPR(Log2, (short), void), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void Log2(int)", asFUNCTIONPR(Log2, (int), void), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void Log2(int64)", asFUNCTIONPR(Log2, (long), void), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int SquaredBlock(int,int)", asFUNCTION(SquaredBlock), asCALL_CDECL) >= 0);
	
	// First register the structure !
	RegisterStructure_PHDVECTOR();
	RegisterStructure_GAMEVECTOR();
	RegisterStructure_PHD3DPOS();
	RegisterStructure_BITEINFO();
	//RegisterStructure_PCLIGHT(); // ITEMINFO require it, but for scripting, light is not required, it's managed by the renderer
	//RegisterStructure_ITEMLIGHT(); // ITEMINFO require it, but for scripting, light is not required, it's managed by the renderer
	//RegisterStructure_ITEMINFO();
	//RegisterStructure_COLLINFO();
	//RegisterStructure_LOTINFO();
	//RegisterStructure_AIINFO();
	//RegisterStructure_CREATUREINFO();

	// Then we can register the global function !
	//RegisterFunction_BOX();

	RegisterFunction_FunctionStubs();

	RegisterItemHelper();
	RegisterCreatureHelper();
	RegisterEffectsHelper();
	RegisterVariablesGlobal();
}

void ReleaseScripting()
{
	if (scriptEngine != NULL)
	{
		scriptEngine->ShutDownAndRelease();
		scriptEngine = NULL;
	}
}
