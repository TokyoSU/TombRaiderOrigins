#include "pch.h"
#include "creature_script.h"
#include "box.h"
#include "control.h"
#include "scripting.h"
#include "objects.h"

static short GetCreatureJointRotation(short item_number, short index)
{
	if (index < 0 || index >= CREATURE_JOINT_ROTATION_COUNT)
		return 0;
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return 0;
	return creature->joint_rotation[index];
}

static short GetCreatureMaximumTurn(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return ANGLE(1); // default value for maximum turn !
	return creature->maximum_turn;
}

static void SetCreatureMaximumTurn(short item_number, short maximum_turn)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return;
	creature->maximum_turn = maximum_turn;
}

static bool IsCreatureAlerted(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->alerted;
}

static void SetCreatureAlerted(short item_number, bool enabled)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return;
	creature->alerted = enabled;
}

static bool IsCreatureHeadLeft(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->head_left;
}

static bool IsCreatureHeadRight(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->head_right;
}

static void SetCreatureReachedGoal(short item_number, bool enabled)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return;
	creature->reached_goal = enabled;
}

static bool IsCreatureReachedGoal(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->reached_goal;
}

static bool IsCreatureHurtByLara(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->hurt_by_lara;
}

static bool DoCreatureHaveJumpAhead(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->jump_ahead;
}

static bool DoCreatureHaveMonkeyAhead(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->monkey_ahead;
}

static short GetCreatureItemNumber(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->item_num;
}

static int GetCreatureMoodType(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->mood;
}

static void SetCreatureMoodType(short item_number, int moodtype)
{
	if (moodtype < 0 || moodtype >= MAX_MOODTYPE)
		return;
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return;
	creature->mood = (MOOD_TYPE_ENUM)moodtype;
}

static short Creature_Turn(short item_number)
{
	return CreatureTurn(&items[item_number], GetCreatureMaximumTurn(item_number));
}

static short Creature_Turn(short item_number, short maximum_turn)
{
	return CreatureTurn(&items[item_number], maximum_turn);
}

static void Creature_Underwater(short item_number, long depth)
{
	CreatureUnderwater(&items[item_number], depth);
}

static void Creature_AIInfo(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return;
	CreatureAIInfo(&items[item_number], &creature->ai);
}

static short GetZoneNumberFromCreatureAI(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return -1;
	return creature->ai.zone_number;
}

static short GetEnemyZoneFromCreatureAI(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return -1;
	return creature->ai.enemy_zone;
}

static int GetCreatureAITargetDistance(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return -1;
	return creature->ai.distance;
}

static bool IsCreatureAITargetAhead(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->ai.ahead;
}

static bool IsCreatureAITargetCouldBite(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return false;
	return creature->ai.bite;
}

static short GetCreatureAITargetAngleY(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return -1;
	return creature->ai.angle;
}

static short GetCreatureAITargetAngleX(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return -1;
	return creature->ai.x_angle;
}

static short GetCreatureAIEnemyFacingAngle(short item_number)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return -1;
	return creature->ai.enemy_facing;
}

static void CreatureMoodAlt(short item_number, bool isViolent)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return;
	CreatureMood(&items[item_number], &creature->ai, isViolent);
}

static void GetCreatureMoodAlt(short item_number, bool isViolent)
{
	auto* creature = GetCreatureInfo(item_number);
	if (creature == NULL)
		return;
	GetCreatureMood(&items[item_number], &creature->ai, isViolent);
}

void RegisterCreatureHelper()
{
	// Enums
	assert(scriptEngine->RegisterEnum("MOOD_TYPE") >= 0);
	assert(scriptEngine->RegisterEnumValue("MOOD_TYPE", "BORED_MOOD", BORED_MOOD) >= 0);
	assert(scriptEngine->RegisterEnumValue("MOOD_TYPE", "ATTACK_MOOD", ATTACK_MOOD) >= 0);
	assert(scriptEngine->RegisterEnumValue("MOOD_TYPE", "ESCAPE_MOOD", ESCAPE_MOOD) >= 0);
	assert(scriptEngine->RegisterEnumValue("MOOD_TYPE", "STALK_MOOD", STALK_MOOD) >= 0);

	// Helper
	assert(scriptEngine->RegisterGlobalFunction("int16 GetCreatureJointRotation(int16,int16)", asFUNCTION(GetCreatureJointRotation), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 GetCreatureMaximumTurn(int16)", asFUNCTION(GetCreatureMaximumTurn), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void SetCreatureMaximumTurn(int16,int16)", asFUNCTION(SetCreatureMaximumTurn), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool IsCreatureAlerted(int16)", asFUNCTION(IsCreatureAlerted), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void SetCreatureAlerted(int16,bool)", asFUNCTION(SetCreatureAlerted), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool IsCreatureHeadLeft(int16)", asFUNCTION(IsCreatureHeadLeft), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool IsCreatureHeadRight(int16)", asFUNCTION(IsCreatureHeadRight), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool IsCreatureReachedGoal(int16)", asFUNCTION(IsCreatureReachedGoal), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void SetCreatureReachedGoal(int16,bool)", asFUNCTION(SetCreatureReachedGoal), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool IsCreatureHurtByLara(int16)", asFUNCTION(IsCreatureHurtByLara), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool DoCreatureHaveJumpAhead(int16)", asFUNCTION(DoCreatureHaveJumpAhead), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool DoCreatureHaveMonkeyAhead(int16)", asFUNCTION(DoCreatureHaveMonkeyAhead), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 GetCreatureItemNumber(int16)", asFUNCTION(GetCreatureItemNumber), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("MOOD_TYPE GetCreatureMoodType(int16)", asFUNCTION(GetCreatureMoodType), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void SetCreatureMoodType(int16,MOOD_TYPE)", asFUNCTION(SetCreatureMoodType), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 GetZoneNumberFromCreatureAI(int16)", asFUNCTION(GetZoneNumberFromCreatureAI), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 GetEnemyZoneFromCreatureAI(int16)", asFUNCTION(GetEnemyZoneFromCreatureAI), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int GetCreatureAITargetDistance(int16)", asFUNCTION(GetCreatureAITargetDistance), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool IsCreatureAITargetAhead(int16)", asFUNCTION(IsCreatureAITargetAhead), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool IsCreatureAITargetCouldBite(int16)", asFUNCTION(IsCreatureAITargetCouldBite), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 GetCreatureAITargetAngleY(int16)", asFUNCTION(GetCreatureAITargetAngleY), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 GetCreatureAITargetAngleX(int16)", asFUNCTION(GetCreatureAITargetAngleX), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 GetCreatureAIEnemyFacingAngle(int16)", asFUNCTION(GetCreatureAIEnemyFacingAngle), asCALL_CDECL) >= 0);
	
	// From box.cpp
	assert(scriptEngine->RegisterGlobalFunction("void CreatureAIInfo(int16)", asFUNCTION(Creature_AIInfo), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void CreatureMood(int16,bool)", asFUNCTION(CreatureMoodAlt), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void GetCreatureMood(int16,bool)", asFUNCTION(GetCreatureMoodAlt), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void CreatureDie(int16, int64)", asFUNCTION(CreatureDie), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void InitialiseCreature(int16)", asFUNCTION(InitialiseCreature), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("bool CreatureActive(int16)", asFUNCTION(CreatureActive), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 CreatureTurn(int16)", asFUNCTIONPR(Creature_Turn, (short), short), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int16 CreatureTurn(int16,int16)", asFUNCTIONPR(Creature_Turn, (short,short), short), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("int CreatureAnimation(int16,int16,int16)", asFUNCTION(CreatureAnimation), asCALL_CDECL) >= 0);
	assert(scriptEngine->RegisterGlobalFunction("void CreatureUnderwater(int16,int)", asFUNCTION(Creature_Underwater), asCALL_CDECL) >= 0);
	
}
