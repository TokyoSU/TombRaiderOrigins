#pragma once
#include "../global/types.h"

long OnDrawBridge(ITEM_INFO* item, long z, long x);
void DrawBridgeFloor(ITEM_INFO* item, long x, long y, long z, long* h);
void DrawBridgeCeiling(ITEM_INFO* item, long x, long y, long z, long* c);
void DrawBridgeCollision(short item_number, ITEM_INFO* l, COLL_INFO* coll);
void InitialiseLift(short item_number);
void LiftControl(short item_number);
void LiftFloorCeiling(ITEM_INFO* item, long x, long y, long z, long* h, long* c);
void LiftFloor(ITEM_INFO* item, long x, long y, long z, long* h);
void LiftCeiling(ITEM_INFO* item, long x, long y, long z, long* c);
long GetOffset(ITEM_INFO* item, long x, long z);
void BridgeFlatFloor(ITEM_INFO* item, long x, long y, long z, long* h);
void BridgeFlatCeiling(ITEM_INFO* item, long x, long y, long z, long* c);
void BridgeTilt1Floor(ITEM_INFO* item, long x, long y, long z, long* h);
void BridgeTilt1Ceiling(ITEM_INFO* item, long x, long y, long z, long* c);
void BridgeTilt2Floor(ITEM_INFO* item, long x, long y, long z, long* h);
void BridgeTilt2Ceiling(ITEM_INFO* item, long x, long y, long z, long* c);
void ShutThatDoor(DOORPOS_DATA* d);
void OpenThatDoor(DOORPOS_DATA* d);
void InitialiseDoor(short item_number);
void DoorControl(short item_number);
void SmashWindow(short item_number);
void InitialiseWindow(short item_number);
void WindowControl(short item_number);
void GeneralControl(short item_number);
void DetonatorControl(short item_number);
void ControlAnimating_1_4(short item_number);
void MiniCopterControl(short item_number);
void EarthQuake(short item_number);
void ControlCutShotgun(short item_number);
void InitialiseFinalLevel();
void FinalLevelCounter(short item_number);

enum object_types
{
	LARA, PISTOLS, HAIR, SHOTGUN, MAGNUM, UZI, M16, ROCKET_GUN, GRENADE_GUN, HARPOON_GUN, FLARE,
	VEHICLE_ANIM, VEHICLE_EXTRA, LARA_EXTRA,
	KAYAK, BOAT, QUADBIKE, MINECART, BIGGUN, UPV,
	TRIBEAXE, BLOWPIPE,
	DOG, SMALL_RAT, KILL_ALL_TRIGGERS,
	TR3_TRIBEAXE, TR3_BLOWPIPE,
	TR3_CROW, TR3_TIGER,
	TR3_VULTURE,
	TR3_TARGETS, BOO_MUTANT, TR3_CROCODILE, BUTTERFLY,
	TR3_COMPY, TR3_LIZARD_MAN, TR3_TRIBEBOSS, TR3_STHPAC_MERCENARY, CARCASS,
	TR3_OILRED, TR3_WHITE_SOLDIER, TR3_HUSKIE, TR3_BURNT_MUTANT, WILLARD, TR3_MUTANT1, TR3_MUTANT2, TR3_MUTANT3,
	FLYING_MUTANT_EMITTER, RAPTOR_EMITTER,
	TR3_WILLARD_BOSS, TR3_FLAMETHROWER_BLOKE,
	TR3_LONDON_MERCENARY1, LON_MERCENARY2, TR3_PUNK1, PUNK2, WATER_BLOKE, TR3_SECURITY_GUARD, TR3_LONDON_BOSS, TR3_ELECTRIC_CLEANER, FLOATING_CORPSE,
	TR3_MP1, TR3_MP2, TR3_BOB, TR3_SWAT_GUN, TR3_ROBOT_SENTRY_GUN, TR3_CIVVIE, SECURITY_LASER_ALARM, SECURITY_LASER_DEADLY, SECURITY_LASER_KILLER,
	TR3_COBRA, TR3_SHIVA, TR3_MONKEY, BEAR_TRAP, TR3_TONY,
	AI_GUARD, AI_AMBUSH, AI_PATROL1, AI_MODIFY, AI_FOLLOW, AI_PATROL2, AI_X1, AI_X2, AI_X3,
	FALLING_BLOCK, FALLING_BLOCK2, FALLING_PLANK,
	PENDULUM, SPIKES, ROLLING_BALL, BIG_ROLLING_BALL, DARTS, DART_EMITTER, HOMING_DART_EMITTER, DRAW_BRIDGE, TEETH_TRAP, LIFT,
	MOVING_BAR, MOVABLE_BLOCK, MOVABLE_BLOCK2, MOVABLE_BLOCK3, MOVABLE_BLOCK4,
	SMASH_WINDOW, SMASH_OBJECT1, SMASH_OBJECT2, SMASH_OBJECT3, SAW, HOOK,
	FALLING_CEILING1, SPINNING_BLADE, CIRCULAR_BLADE, TRAIN,
	BLADE, AVALANCHE, ICICLES, SPIKE_WALL, SPRING_BOARD, CEILING_SPIKES,
	AIRLOCK_SWITCH, SMALL_SWITCH,
	FAN, SMALL_FAN, SWING_BOX, MESHSWAP1, MESHSWAP2, MESHSWAP3, BORDERS, OILDRUMS, DEATH_SLIDE, PUSH_SWITCH,
	ABOVE_WATER_SWITCH, UNDERWATER_SWITCH,
	DOOR_TYPE1, DOOR_TYPE2, DOOR_TYPE3, DOOR_TYPE4, DOOR_TYPE5, DOOR_TYPE6, DOOR_TYPE7, DOOR_TYPE8,
	TRAPDOOR, TRAPDOOR2, BIGTRAPDOOR,
	BRIDGE_FLAT, BRIDGE_TILT1, BRIDGE_TILT2,
	PASSPORT_OPTION, MAP_OPTION, PHOTO_OPTION,
	PLAYER_1, PLAYER_2, PLAYER_3, PLAYER4, PLAYER_5, PLAYER_6, PLAYER_7, PLAYER_8, PLAYER_9, PLAYER_10,
	PASSPORT_CLOSED, MAP_CLOSED,
	GUN_ITEM, SHOTGUN_ITEM, MAGNUM_ITEM, UZI_ITEM, HARPOON_ITEM, M16_ITEM, ROCKET_GUN_ITEM, GRENADE_GUN_ITEM,
	GUN_AMMO_ITEM, SG_AMMO_ITEM, MAG_AMMO_ITEM, UZI_AMMO_ITEM, HARPOON_AMMO_ITEM, M16_AMMO_ITEM, ROCKET_AMMO_ITEM, GRENADE_AMMO_ITEM,
	MEDI_ITEM, BIGMEDI_ITEM, FLAREBOX_ITEM, FLARE_ITEM, SAVEGAME_CRYSTAL_ITEM,
	DETAIL_OPTION, SOUND_OPTION, CONTROL_OPTION, GAMMA_OPTION,
	GUN_OPTION, SHOTGUN_OPTION, MAGNUM_OPTION, UZI_OPTION, HARPOON_OPTION, M16_OPTION, ROCKET_OPTION, GRENADE_OPTION,
	GUN_AMMO_OPTION, SG_AMMO_OPTION, MAG_AMMO_OPTION, UZI_AMMO_OPTION, HARPOON_AMMO_OPTION, M16_AMMO_OPTION, ROCKET_AMMO_OPTION, GRENADE_AMMO_OPTION,
	MEDI_OPTION, BIGMEDI_OPTION, FLAREBOX_OPTION, SAVEGAME_CRYSTAL_OPTION,
	PUZZLE_ITEM1, PUZZLE_ITEM2, PUZZLE_ITEM3, PUZZLE_ITEM4,
	PUZZLE_OPTION1, PUZZLE_OPTION2, PUZZLE_OPTION3, PUZZLE_OPTION4,
	PUZZLE_HOLE1, PUZZLE_HOLE2, PUZZLE_HOLE3, PUZZLE_HOLE4,
	PUZZLE_DONE1, PUZZLE_DONE2, PUZZLE_DONE3, PUZZLE_DONE4,
	SECRET_ITEM1, SECRET_ITEM2, SECRET_ITEM3,
	KEY_ITEM1, KEY_ITEM2, KEY_ITEM3, KEY_ITEM4,
	KEY_OPTION1, KEY_OPTION2, KEY_OPTION3, KEY_OPTION4,
	KEY_HOLE1, KEY_HOLE2, KEY_HOLE3, KEY_HOLE4,
	PICKUP_ITEM1, PICKUP_ITEM2,
	PICKUP_OPTION1, PICKUP_OPTION2,
	ICON_PICKUP1_ITEM, ICON_PICKUP2_ITEM, ICON_PICKUP3_ITEM, ICON_PICKUP4_ITEM,
	ICON_PICKUP1_OPTION, ICON_PICKUP2_OPTION, ICON_PICKUP3_OPTION, ICON_PICKUP4_OPTION,

	PICKUPDISPLAY_PISTOLS,
	PICKUPDISPLAY_SHOTGUN,
	PICKUPDISPLAY_DESERTEAGLE,
	PICKUPDISPLAY_UZIS,
	PICKUPDISPLAY_HARPOON,
	PICKUPDISPLAY_HANDK,
	PICKUPDISPLAY_ROCKET_LAUNCHER,
	PICKUPDISPLAY_GRENADE_LAUNCHER,

	PICKUPDISPLAY_PISTOLS_AMMO,
	PICKUPDISPLAY_SHOTGUN_AMMO,
	PICKUPDISPLAY_DESERTEAGLE_AMMO,
	PICKUPDISPLAY_UZIS_AMMO,
	PICKUPDISPLAY_HARPOON_AMMO,
	PICKUPDISPLAY_HANDK_AMMO,
	PICKUPDISPLAY_ROCKET_LAUNCHER_AMMO,
	PICKUPDISPLAY_GRENADE_LAUNCHER_AMMO,

	PICKUPDISPLAY_SMALLMEDI,
	PICKUPDISPLAY_BIGMEDI,
	PICKUPDISPLAY_FLAREBOX,
	PICKUPDISPLAY_SGCRYSTAL,

	PICKUPDISPLAY_PUZZLE1,
	PICKUPDISPLAY_PUZZLE2,
	PICKUPDISPLAY_PUZZLE3,
	PICKUPDISPLAY_PUZZLE4,
	PICKUPDISPLAY_KEY1,
	PICKUPDISPLAY_KEY2,
	PICKUPDISPLAY_KEY3,
	PICKUPDISPLAY_KEY4,
	PICKUPDISPLAY_ICON1,
	PICKUPDISPLAY_ICON2,
	PICKUPDISPLAY_ICON3,
	PICKUPDISPLAY_ICON4,
	PICKUPDISPLAY_PICKUP1,
	PICKUPDISPLAY_PICKUP2,

	FIREHEAD, TONYFIREBALL, SPHERE_OF_DOOM3,
	ALARM_SOUND, WATER_DRIP, TR3_TREX, TR3_RAPTOR, BIRD_TWEETER, CLOCK_CHIMES, AREA51_LASER, LIGHTNING_EMITTER2,
	HOT_LIQUID,
	SHADOW,
	DETONATOR,
	EXPLOSION1, BUBBLES1, BUBBLES2,
	GLOW,
	GUN_FLASH, M16_FLASH, DESERT_EAGLE_FLASH, BODY_PART, CAMERA_TARGET,
	WATERFALL, DIVER_HARPOON, DRAGON_FIRE, KNIFE, ROCKET, HARPOON_BOLT, GRENADE,
	SPECIAL_FX1, SPECIAL_FX2, SPECIAL_FX3, LARA_SKIN,
	LAVA, LAVA_EMITTER,
	STROBE_LIGHT, ELECTRICAL_LIGHT, ON_OFF_LIGHT, PULSE_LIGHT, EXTRA_LIGHT1, EXTRA_LIGHT2,
	RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT, AMBER_LIGHT, WHITE_LIGHT,
	FLAME, FLAME_EMITTER, FLAME_EMITTER2, FLAME_EMITTER3, SIDE_FLAME_EMITTER,
	SMOKE_EMITTER_WHITE, SMOKE_EMITTER_BLACK, STEAM_EMITTER, GHOST_GAS_EMITTER,
	PIRAHNAS, TROPICAL_FISH,
	PIRAHNA_GFX, TROPICAL_FISH_GFX, BAT_GFX, TRIBEBOSS_GFX, SPIDER_GFX,
	TUMBLEWEED, LEAVES,
	BAT_EMITTER, BIRD_EMITTER,
	ANIMATING1, ANIMATING2, ANIMATING3, ANIMATING4, ANIMATING5, ANIMATING6,
	HORIZON,
	ALPHABET,
	DING_DONG, LARA_ALARM, MINI_COPTER, TR3_WINSTON, TR3_ARMY_WINSTON, ASSAULT_NUMBERS, FINAL_LEVEL, CUT_SHOTGUN, EARTHQUAKE,
	GUNSHELL, SHOTGUNSHELL,
	EXTRAFX1, EXTRAFX2, EXTRAFX3, EXTRAFX4, EXTRAFX5, EXTRAFX6, EXTRAFX7, EXTRAFX8,
	STATIC_2D,
	STATIC_SPRITE_01 = 378,
	STATIC_SPRITE_02,
	STATIC_SPRITE_03,
	STATIC_SPRITE_04,
	STATIC_SPRITE_05,
	STATIC_SPRITE_06,
	STATIC_SPRITE_07,
	STATIC_SPRITE_08,
	STATIC_SPRITE_09,
	STATIC_SPRITE_10,
	STATIC_SPRITE_11,
	STATIC_SPRITE_12,
	STATIC_SPRITE_13,
	STATIC_SPRITE_14,
	STATIC_SPRITE_15,
	STATIC_SPRITE_16,
	STATIC_SPRITE_17,
	STATIC_SPRITE_18,
	STATIC_SPRITE_19,
	TR1_WOLF = 400,
	TR1_BAT,
	TR1_BEAR,
	TR1_DART,
	TR1_DART_EMITTER,
	NUMBER_OBJECTS
};

enum static_types
{
	PLANT0, PLANT1, PLANT2, PLANT3, PLANT4, PLANT5, PLANT6, PLANT7, PLANT8, PLANT9,
	FURNITURE0, FURNITURE1, FURNITURE2, FURNITURE3, FURNITURE4,
	FURNITURE5, FURNITURE6, FURNITURE7, FURNITURE8, FURNITURE9,
	ROCK0, ROCK1, ROCK2, ROCK3, ROCK4, ROCK5, ROCK6, ROCK7, ROCK8, ROCK9,
	ARCHITECTURE0, ARCHITECTURE1, ARCHITECTURE2, ARCHITECTURE3, ARCHITECTURE4,
	ARCHITECTURE5, ARCHITECTURE6, ARCHITECTURE7, ARCHITECTURE8, ARCHITECTURE9,
	DEBRIS0, DEBRIS1, DEBRIS2, DEBRIS3, DEBRIS4, DEBRIS5, DEBRIS6, DEBRIS7, DEBRIS8, DEBRIS9,

	NUMBER_STATIC_OBJECTS = MAX_STATICS
};

extern OBJECT_INFO objects[NUMBER_OBJECTS];
extern STATIC_INFO static_objects[NUMBER_STATIC_OBJECTS];
