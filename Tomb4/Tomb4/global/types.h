#pragma once
#include "math_tbls.h"

#pragma pack(push, 1)

constexpr auto NO_ROOM = 255;

/*math*/
#define SQUARE(x) ((x)*(x))
#define	TRIGMULT2(a,b)		(((a) * (b)) >> W2V_SHIFT)
#define	TRIGMULT3(a,b,c)	(TRIGMULT2((TRIGMULT2(a, b)), c))
#define	FTRIGMULT2(a,b)		((a) * (b))
#define	FTRIGMULT3(a,b,c)	(FTRIGMULT2((FTRIGMULT2(a, b)), c))

/*color*/
#define RGBONLY(r, g, b) ((b) | (((g) | ((r) << 8)) << 8))
#define RGBA(r, g, b, a) (RGBONLY(r, g, b) | ((a) << 24))
#define	CLRA(clr)	((clr >> 24) & 0xFF)	//shift r, g, and b out of the way and 0xFF
#define	CLRR(clr)	((clr >> 16) & 0xFF)	//shift g and b out of the way and 0xFF
#define	CLRG(clr)	((clr >> 8) & 0xFF)		//shift b out of the way and 0xFF
#define	CLRB(clr)	((clr) & 0xFF)			//and 0xFF

/*misc*/
#define SCRIPT_TEXT(num)		(&gfStringWad[gfStringOffset[num]])
#define SetCutPlayed(num)	(CutSceneTriggered |= 1 << (num))
#define SetCutNotPlayed(num)	(CutSceneTriggered &= ~(1 << (num)))
#define CheckCutPlayed(num)	(CutSceneTriggered & (1 << (num)))

#define NO_HEIGHT	-32512
#define NO_ITEM	-1
#define FVF (D3DFVF_TEX2 | D3DFVF_SPECULAR | D3DFVF_DIFFUSE | D3DFVF_XYZRHW)
#define W2V_SHIFT	14
#define MAX_SAMPLES	370
#define MAX_DYNAMICS	64
#define MALLOC_SIZE	15000000	//15MB

/********************DX defs********************/
#define LPDIRECTDRAWX			LPDIRECTDRAW4
#define LPDIRECT3DX				LPDIRECT3D3
#define LPDIRECT3DDEVICEX		LPDIRECT3DDEVICE3
#define LPDIRECTDRAWSURFACEX	LPDIRECTDRAWSURFACE4
#define LPDIRECT3DVIEWPORTX		LPDIRECT3DVIEWPORT3
#define LPDIRECTINPUTX			LPDIRECTINPUT2
#define LPDIRECTINPUTDEVICEX	LPDIRECTINPUTDEVICE2
#define DDSURFACEDESCX			DDSURFACEDESC2
#define LPDDSURFACEDESCX		DDSURFACEDESCX*
#define LPDIRECT3DMATERIALX		LPDIRECT3DMATERIAL3
#define LPDIRECT3DTEXTUREX		LPDIRECT3DTEXTURE2
#define TEXGUID					IID_IDirect3DTexture2
#define DDGUID					IID_IDirectDraw4
#define D3DGUID					IID_IDirect3D3
#define DIGUID					IID_IDirectInput8
#define DIDGUID					IID_IDirectInputDevice8
#define DSNGUID					IID_IDirectSoundNotify
/***********************************************/

/*typedefs*/
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

enum DX_FLAGS
{
	DXF_NONE = 0x0,
	DXF_FULLSCREEN = 0x1,
	DXF_WINDOWED = 0x2,
	DXF_ZBUFFER = 0x10,
	DXF_FPUSETUP = 0x20,
	DXF_NOFREE = 0x40,
	DXF_HWR = 0x80
};

enum carried_weapon_flags
{
	W_NONE = 0x0,
	W_PRESENT = 0x1,
	W_FLASHLIGHT = 0x2,	//speculation, actually unused
	W_LASERSIGHT = 0x4,
	W_AMMO1 = 0x8,
	W_AMMO2 = 0x10,
	W_AMMO3 = 0x20
};

enum anim_commands
{
	ACMD_NULL,
	ACMD_SETPOS,
	ACMD_JUMPVEL,
	ACMD_FREEHANDS,
	ACMD_KILL,
	ACMD_PLAYSFX,
	ACMD_FLIPEFFECT
};

enum ai_bits
{
	GUARD = 1 << 0,
	AMBUSH = 1 << 1,
	PATROL1 = 1 << 2,
	MODIFY = 1 << 3,
	FOLLOW = 1 << 4
};

enum languages
{
	ENGLISH,
	FRENCH,
	GERMAN,
	ITALIAN,
	SPANISH,
	US,
	JAPAN,
	DUTCH
};

enum font_flags
{
	FF_SMALL = 0x1000,
	FF_BLINK = 0x2000,
	FF_RJUSTIFY = 0x4000,
	FF_CENTER = 0x8000
};

enum room_flags
{
	ROOM_UNDERWATER = 0x1,
	ROOM_OUTSIDE = 0x8,
	ROOM_DYNAMIC_LIT = 0x10,
	ROOM_NOT_INSIDE = 0x20,
	ROOM_INSIDE = 0x40,
	ROOM_NO_LENSFLARE = 0x80
};

enum quadrant_names
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum collision_types
{
	CT_NONE = 0x0,
	CT_FRONT = 0x1,
	CT_LEFT = 0x2,
	CT_RIGHT = 0x4,
	CT_TOP = 0x8,
	CT_TOP_FRONT = 0x10,
	CT_CLAMP = 0x20
};

enum sfx_types
{
	SFX_LANDANDWATER = 0,
	SFX_LANDONLY = 0x4000,
	SFX_WATERONLY = 0x8000
};

enum target_type
{
	NO_TARGET,
	PRIME_TARGET,
	SECONDARY_TARGET
};

enum MOOD_TYPES
{
	BORED_MOOD,
	ATTACK_MOOD,
	ESCAPE_MOOD,
	STALK_MOOD,
};

enum ZONE_TYPES
{
	SKELLY_ZONE,
	BASIC_ZONE,
	CROC_ZONE,
	HUMAN_ZONE,
	FLYER_ZONE,
};

enum height_types
{
	WALL,
	SMALL_SLOPE,
	BIG_SLOPE,
	DIAGONAL,
	SPLIT_TRI
};

enum item_status
{
	ITEM_INACTIVE,
	ITEM_ACTIVE,
	ITEM_DEACTIVATED,
	ITEM_INVISIBLE
};

enum floor_types
{
	FLOOR_TYPE,
	DOOR_TYPE,
	TILT_TYPE,
	ROOF_TYPE,
	TRIGGER_TYPE,
	LAVA_TYPE,
	CLIMB_TYPE,
	SPLIT1,
	SPLIT2,
	SPLIT3,
	SPLIT4,
	NOCOLF1T,
	NOCOLF1B,
	NOCOLF2T,
	NOCOLF2B,
	NOCOLC1T,
	NOCOLC1B,
	NOCOLC2T,
	NOCOLC2B,
	MONKEY_TYPE,
	TRIGTRIGGER_TYPE,
	MINER_TYPE
};

enum weapon_types
{
	WEAPON_NONE,
	WEAPON_PISTOLS,
	WEAPON_REVOLVER,
	WEAPON_UZI,
	WEAPON_SHOTGUN,
	WEAPON_GRENADE,
	WEAPON_CROSSBOW,
	WEAPON_FLARE,
	WEAPON_TORCH
};

enum lara_water_status
{
	LW_ABOVE_WATER,
	LW_UNDERWATER,
	LW_SURFACE,
	LW_FLYCHEAT,
	LW_WADE
};

enum lara_mesh
{
	LM_HIPS,
	LM_LTHIGH,
	LM_LSHIN,
	LM_LFOOT,
	LM_RTHIGH,
	LM_RSHIN,
	LM_RFOOT,
	LM_TORSO,
	LM_RINARM,
	LM_ROUTARM,
	LM_RHAND,
	LM_LINARM,
	LM_LOUTARM,
	LM_LHAND,
	LM_HEAD,
	NUM_LARA_MESHES
};

enum lara_bone
{
	LB_HIPS = 1,
	LB_LTHIGH,
	LB_LSHIN,
	LB_LFOOT,
	LB_RTHIGH,
	LB_RSHIN,
	LB_RFOOT,
	LB_TORSO,
	LB_RINARM,
	LB_ROUTARM,
	LB_RHAND,
	LB_LINARM,
	LB_LOUTARM,
	LB_LHAND,
	LB_HEAD,
	NUM_LARA_BONES = LM_HEAD
};

enum trigger_types
{
	TRIGGER,
	PAD,
	SWITCH,
	KEY,
	PICKUP,
	HEAVY,
	ANTIPAD,
	COMBAT,
	DUMMY,
	ANTITRIGGER,
	HEAVYSWITCH,
	HEAVYANTITRIGGER,
	MONKEY
};

enum trigobj_types
{
	TO_OBJECT,
	TO_CAMERA,
	TO_SINK,
	TO_FLIPMAP,
	TO_FLIPON,
	TO_FLIPOFF,
	TO_TARGET,
	TO_FINISH,
	TO_CD,
	TO_FLIPEFFECT,
	TO_SECRET,
	TO_BODYBAG,
	TO_FLYBY,
	TO_CUTSCENE
};

enum input_buttons
{
	IN_NONE = 0x0,
	IN_FORWARD = 0x1,
	IN_BACK = 0x2,
	IN_LEFT = 0x4,
	IN_RIGHT = 0x8,
	IN_JUMP = 0x10,
	IN_DRAW = 0x20,
	IN_ACTION = 0x40,
	IN_WALK = 0x80,
	IN_OPTION = 0x100,
	IN_LOOK = 0x200,
	IN_LSTEP = 0x400,
	IN_RSTEP = 0x800,
	IN_ROLL = 0x1000,
	IN_PAUSE = 0x2000,
	IN_A = 0x4000,
	IN_B = 0x8000,
	IN_CHEAT = 0x10000,
	IN_D = 0x20000,
	IN_C = 0x30000,
	IN_E = 0x40000,
	IN_FLARE = 0x80000,
	IN_SELECT = 0x100000,
	IN_DESELECT = 0x200000,
	IN_SAVE = 0x400000,
	IN_LOAD = 0x800000,
	IN_STEPSHIFT = 0x1000000,
	IN_LOOKLEFT = 0x2000000,
	IN_LOOKRIGHT = 0x4000000,
	IN_LOOKFORWARD = 0x8000000,
	IN_LOOKBACK = 0x10000000,
	IN_DUCK = 0x20000000,
	IN_SPRINT = 0x40000000,
	IN_TARGET = 0x80000000,
	IN_ALL = 0xFFFFFFFF
};

enum ITEM_FLAGS
{
	IFL_TRIGGERED = 0x20,
	IFL_SWITCH_ONESHOT = 0x40,	//oneshot for switch items
	IFL_ANTITRIGGER_ONESHOT = 0x80,	//oneshot for antitriggers
	IFL_INVISIBLE = 0x100,	//also used as oneshot for everything else
	IFL_CODEBITS = 0x3E00,
	IFL_REVERSE = 0x4000,
	IFL_CLEARBODY = 0x8000
};

enum lara_gun_status
{
	LG_NO_ARMS,
	LG_HANDS_BUSY,
	LG_DRAW_GUNS,
	LG_UNDRAW_GUNS,
	LG_READY,
	LG_FLARE,
};

enum camera_type
{
	CHASE_CAMERA,
	FIXED_CAMERA,
	LOOK_CAMERA,
	COMBAT_CAMERA,
	CINEMATIC_CAMERA,
	HEAVY_CAMERA,
};

enum LightTypes
{
	LIGHT_SUN,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_SHADOW,
	LIGHT_FOG
};

enum gf_level_options
{
	GF_YOUNGLARA = 0x1,
	GF_WEATHER = 0x2,
	GF_HORIZON = 0x4,
	GF_LAYER1 = 0x8,
	GF_LAYER2 = 0x10,
	GF_STARFIELD = 0x20,
	GF_LIGHTNING = 0x40,
	GF_TRAIN = 0x80,
	GF_PULSE = 0x100,
	GF_HORIZONCOLADD = 0x200,
	GF_RESETHUB = 0x400,
	GF_LENSFLARE = 0x800,
	GF_TIMER = 0x1000,
	GF_MIRROR = 0x2000,
	GF_REMOVEAMULET = 0x4000,
	GF_NOLEVEL = 0x8000
};

enum BONE_FLAGS
{
	BN_NONE = 0x0,
	BN_PUSH = 0x1,
	BN_POP = 0x2,
	BN_ROT_X = 0x4,
	BN_ROT_Y = 0x8,
	BN_ROT_Z = 0x10
};

enum OBJECTS_TYPES
{
	LARA, PISTOLS_ANIM, UZI_ANIM, SHOTGUN_ANIM, CROSSBOW_ANIM, GRENADE_GUN_ANIM, SIXSHOOTER_ANIM, FLARE_ANIM,
	LARA_SKIN, LARA_SKIN_JOINTS, LARA_SCREAM,
	LARA_CROSSBOW_LASER,
	LARA_REVOLVER_LASER,
	LARA_HOLSTERS, LARA_HOLSTERS_PISTOLS, LARA_HOLSTERS_UZIS, LARA_HOLSTERS_SIXSHOOTER,
	LARA_SPEECH_HEAD1, LARA_SPEECH_HEAD2, LARA_SPEECH_HEAD3, LARA_SPEECH_HEAD4,
	ACTOR1_SPEECH_HEAD1, ACTOR1_SPEECH_HEAD2,
	ACTOR2_SPEECH_HEAD1, ACTOR2_SPEECH_HEAD2,
	LARA_WATER_MESH, LARA_PETROL_MESH, LARA_DIRT_MESH,
	CROWBAR_ANIM, TORCH_ANIM,
	HAIR,
	MOTORBIKE, JEEP, VEHICLE_EXTRA,

	ENEMY_JEEP,

	SKELETON, SKELETON_MIP,
	GUIDE, GUIDE_MIP,
	VON_CROY, VON_CROY_MIP,
	RAGHEAD, RAGHEAD_MIP,
	SUPER_RAGHEAD, SUPER_RAGHEAD_MIP,
	SETHA, SETHA_MIP,

	MUMMY, MUMMY_MIP,
	SPHINX, SPHINX_MIP,
	CROCODILE, CROCODILE_MIP,
	HORSEMAN, HORSEMAN_MIP,
	SCORPION, SCORPION_MIP,
	JEAN_YVES, JEAN_YVES_MIP,
	TROOPS, TROOPS_MIP,
	KNIGHTS_TEMPLAR, KNIGHTS_TEMPLAR_MIP,
	MUTANT, MUTANT_MIP,
	HORSE, HORSE_MIP,
	BABOON_NORMAL, BABOON_NORMAL_MIP,
	BABOON_INV, BABOON_INV_MIP,
	BABOON_SILENT, BABOON_SILENT_MIP,
	WILD_BOAR, WILD_BOAR_MIP,
	HARPY, HARPY_MIP,
	DEMIGOD1, DEMIGOD1_MIP,
	DEMIGOD2, DEMIGOD2_MIP,
	DEMIGOD3, DEMIGOD3_MIP,
	LITTLE_BEETLE,
	BIG_BEETLE, BIG_BEETLE_MIP,
	WRAITH1, WRAITH2, WRAITH3, WRAITH4,
	BAT,
	DOG, FUCKED_UP_DOG_MIP,
	HAMMERHEAD, HAMMERHEAD_MIP,
	SAS, SAS_MIP,
	SAS_DYING, SAS_DYING_MIP,
	SAS_CAPTAIN, SAS_CAPTAIN_MIP,
	SAS_DRAG_BLOKE,
	AHMET, AHMET_MIP,
	LARA_DOUBLE, LARA_DOUBLE_MIP,
	SMALL_SCORPION,
	LOCUST_EMITTER,

	GAME_PIECE1, GAME_PIECE2, GAME_PIECE3, ENEMY_PIECE, WHEEL_OF_FORTUNE,
	SCALES,

	DART, DART_EMITTER, HOMING_DART_EMITTER,
	FALLING_CEILING, FALLING_BLOCK, FALLING_BLOCK2,
	SMASHABLE_BIKE_WALL,
	SMASHABLE_BIKE_FLOOR,
	TRAPDOOR1, TRAPDOOR2, TRAPDOOR3,
	FLOOR_TRAPDOOR1, FLOOR_TRAPDOOR2,
	CEILING_TRAPDOOR1, CEILING_TRAPDOOR2,
	SCALING_TRAPDOOR,
	ROLLINGBALL,
	SPIKEY_FLOOR, TEETH_SPIKES, JOBY_SPIKES, SLICER_DICER,
	CHAIN, PLOUGH, STARGATE, HAMMER, BURNING_FLOOR, COG, SPIKEBALL,
	FLAME, FLAME_EMITTER, FLAME_EMITTER2, FLAME_EMITTER3,
	ROPE, FIREROPE, POLEROPE,
	ONEBLOCK_PLATFORM, TWOBLOCK_PLATFORM,
	RAISING_BLOCK1, RAISING_BLOCK2, EXPANDING_PLATFORM,
	SQUISHY_BLOCK1, SQUISHY_BLOCK2,
	PUSHABLE_OBJECT1, PUSHABLE_OBJECT2, PUSHABLE_OBJECT3, PUSHABLE_OBJECT4, PUSHABLE_OBJECT5,
	TRIPWIRE, SENTRY_GUN, MINE,
	MAPPER, OBELISK,
	FLOOR_4BLADE, ROOF_4BLADE, BIRD_BLADE, CATWALK_BLADE, MOVING_BLADE, PLINTH_BLADE, SETH_BLADE,
	LIGHTNING_CONDUCTOR,
	ELEMENT_PUZZLE,

	PUZZLE_ITEM1, PUZZLE_ITEM2, PUZZLE_ITEM3, PUZZLE_ITEM4,
	PUZZLE_ITEM5, PUZZLE_ITEM6, PUZZLE_ITEM7, PUZZLE_ITEM8,
	PUZZLE_ITEM9, PUZZLE_ITEM10, PUZZLE_ITEM11, PUZZLE_ITEM12,

	PUZZLE_ITEM1_COMBO1, PUZZLE_ITEM1_COMBO2,
	PUZZLE_ITEM2_COMBO1, PUZZLE_ITEM2_COMBO2,
	PUZZLE_ITEM3_COMBO1, PUZZLE_ITEM3_COMBO2,
	PUZZLE_ITEM4_COMBO1, PUZZLE_ITEM4_COMBO2,
	PUZZLE_ITEM5_COMBO1, PUZZLE_ITEM5_COMBO2,
	PUZZLE_ITEM6_COMBO1, PUZZLE_ITEM6_COMBO2,
	PUZZLE_ITEM7_COMBO1, PUZZLE_ITEM7_COMBO2,
	PUZZLE_ITEM8_COMBO1, PUZZLE_ITEM8_COMBO2,

	KEY_ITEM1, KEY_ITEM2, KEY_ITEM3, KEY_ITEM4,
	KEY_ITEM5, KEY_ITEM6, KEY_ITEM7, KEY_ITEM8,
	KEY_ITEM9, KEY_ITEM10, KEY_ITEM11, KEY_ITEM12,

	KEY_ITEM1_COMBO1, KEY_ITEM1_COMBO2,
	KEY_ITEM2_COMBO1, KEY_ITEM2_COMBO2,
	KEY_ITEM3_COMBO1, KEY_ITEM3_COMBO2,
	KEY_ITEM4_COMBO1, KEY_ITEM4_COMBO2,
	KEY_ITEM5_COMBO1, KEY_ITEM5_COMBO2,
	KEY_ITEM6_COMBO1, KEY_ITEM6_COMBO2,
	KEY_ITEM7_COMBO1, KEY_ITEM7_COMBO2,
	KEY_ITEM8_COMBO1, KEY_ITEM8_COMBO2,

	PICKUP_ITEM1, PICKUP_ITEM2, PICKUP_ITEM3, PICKUP_ITEM4,

	PICKUP_ITEM1_COMBO1, PICKUP_ITEM1_COMBO2,
	PICKUP_ITEM2_COMBO1, PICKUP_ITEM2_COMBO2,
	PICKUP_ITEM3_COMBO1, PICKUP_ITEM3_COMBO2,
	PICKUP_ITEM4_COMBO1, PICKUP_ITEM4_COMBO2,

	EXAMINE1, EXAMINE2, EXAMINE3,

	CROWBAR_ITEM,
	BURNING_TORCH_ITEM,
	CLOCKWORK_BEETLE, CLOCKWORK_BEETLE_COMBO1, CLOCKWORK_BEETLE_COMBO2,
	MINE_DETECTOR,

	QUEST_ITEM1, QUEST_ITEM2, QUEST_ITEM3, QUEST_ITEM4, QUEST_ITEM5, QUEST_ITEM6,
	MAP, SECRET_MAP,
	PUZZLE_HOLE1, PUZZLE_HOLE2, PUZZLE_HOLE3, PUZZLE_HOLE4,
	PUZZLE_HOLE5, PUZZLE_HOLE6, PUZZLE_HOLE7, PUZZLE_HOLE8,
	PUZZLE_HOLE9, PUZZLE_HOLE10, PUZZLE_HOLE11, PUZZLE_HOLE12,
	PUZZLE_DONE1, PUZZLE_DONE2, PUZZLE_DONE3, PUZZLE_DONE4,
	PUZZLE_DONE5, PUZZLE_DONE6, PUZZLE_DONE7, PUZZLE_DONE8,
	PUZZLE_DONE9, PUZZLE_DONE10, PUZZLE_DONE11, PUZZLE_DONE12,
	KEY_HOLE1, KEY_HOLE2, KEY_HOLE3, KEY_HOLE4,
	KEY_HOLE5, KEY_HOLE6, KEY_HOLE7, KEY_HOLE8,
	KEY_HOLE9, KEY_HOLE10, KEY_HOLE11, KEY_HOLE12,

	WATERSKIN1_EMPTY, WATERSKIN1_1, WATERSKIN1_2, WATERSKIN1_3,
	WATERSKIN2_EMPTY, WATERSKIN2_1, WATERSKIN2_2, WATERSKIN2_3, WATERSKIN2_4, WATERSKIN2_5,

	SWITCH_TYPE1, SWITCH_TYPE2, SWITCH_TYPE3, SWITCH_TYPE4, SWITCH_TYPE5, SWITCH_TYPE6,
	SHOOT_SWITCH1, SHOOT_SWITCH2,
	UNDERWATER_SWITCH1, UNDERWATER_SWITCH2,
	TURN_SWITCH,
	COG_SWITCH,
	LEVER_SWITCH,
	JUMP_SWITCH,
	CROWBAR_SWITCH,
	PULLEY,
	DOOR_TYPE1, DOOR_TYPE2, DOOR_TYPE3, DOOR_TYPE4, DOOR_TYPE5, DOOR_TYPE6, DOOR_TYPE7, DOOR_TYPE8,
	PUSHPULL_DOOR1, PUSHPULL_DOOR2,
	KICK_DOOR1, KICK_DOOR2,
	UNDERWATER_DOOR,
	DOUBLE_DOORS,
	BRIDGE_FLAT, BRIDGE_TILT1, BRIDGE_TILT2,

	SARCOPHAGUS,

	SEQUENCE_DOOR,
	SEQUENCE_SWITCH1, SEQUENCE_SWITCH2, SEQUENCE_SWITCH3,

	SARCOPHAGUS_CUT,
	HORUS_STATUE,
	GOD_HEAD,
	SETH_DOOR,
	STATUE_PLINTH,

	PISTOLS_ITEM,
	PISTOLS_AMMO_ITEM,
	UZI_ITEM,
	UZI_AMMO_ITEM,
	SHOTGUN_ITEM,
	SHOTGUN_AMMO1_ITEM,
	SHOTGUN_AMMO2_ITEM,
	CROSSBOW_ITEM,
	CROSSBOW_AMMO1_ITEM,
	CROSSBOW_AMMO2_ITEM,
	CROSSBOW_AMMO3_ITEM,
	CROSSBOW_BOLT,
	GRENADE_GUN_ITEM,
	GRENADE_GUN_AMMO1_ITEM,
	GRENADE_GUN_AMMO2_ITEM,
	GRENADE_GUN_AMMO3_ITEM,
	GRENADE,
	SIXSHOOTER_ITEM,
	SIXSHOOTER_AMMO_ITEM,
	BIGMEDI_ITEM, SMALLMEDI_ITEM,
	LASERSIGHT_ITEM, BINOCULARS_ITEM,
	FLARE_ITEM, FLARE_INV_ITEM,
	DIARY_ITEM, COMPASS_ITEM,
	MEMCARD_LOAD_INV_ITEM,
	MEMCARD_SAVE_INV_ITEM,
	PC_LOAD_INV_ITEM,
	PC_SAVE_INV_ITEM,

	SMOKE_EMITTER_WHITE, SMOKE_EMITTER_BLACK, STEAM_EMITTER,
	EARTHQUAKE,
	BUBBLES,
	WATERFALLMIST,
	GUNSHELL, SHOTGUNSHELL,
	GUN_FLASH,
	BUTTERFLY,
	SPRINKLER,

	RED_LIGHT, GREEN_LIGHT, BLUE_LIGHT, AMBER_LIGHT, WHITE_LIGHT, BLINKING_LIGHT,
	LENS_FLARE,

	AI_GUARD, AI_AMBUSH, AI_PATROL1, AI_MODIFY, AI_FOLLOW, AI_PATROL2, AI_X1, AI_X2, LARA_START_POS,

	KILL_ALL_TRIGGERS,
	TRIGGER_TRIGGERER,
	SMASH_OBJECT1, SMASH_OBJECT2, SMASH_OBJECT3, SMASH_OBJECT4, SMASH_OBJECT5, SMASH_OBJECT6, SMASH_OBJECT7, SMASH_OBJECT8,
	MESHSWAP1, MESHSWAP2, MESHSWAP3,
	DEATH_SLIDE,
	BODY_PART, CAMERA_TARGET,
	WATERFALL1, WATERFALL2, WATERFALL3,
	PLANET_EFFECT,
	ANIMATING1, ANIMATING1_MIP, ANIMATING2, ANIMATING2_MIP, ANIMATING3, ANIMATING3_MIP, ANIMATING4, ANIMATING4_MIP,
	ANIMATING5, ANIMATING5_MIP, ANIMATING6, ANIMATING6_MIP, ANIMATING7, ANIMATING7_MIP, ANIMATING8, ANIMATING8_MIP,
	ANIMATING9, ANIMATING9_MIP, ANIMATING10, ANIMATING10_MIP, ANIMATING11, ANIMATING11_MIP, ANIMATING12, ANIMATING12_MIP,
	ANIMATING13, ANIMATING13_MIP, ANIMATING14, ANIMATING14_MIP, ANIMATING15, ANIMATING15_MIP, ANIMATING16, ANIMATING16_MIP,
	HORIZON, SKY_GRAPHICS, BINOCULAR_GRAPHICS, TARGET_GRAPHICS,

	DEFAULT_SPRITES,
	MISC_SPRITES,

	TR1_WOLF,
	TR1_BEAR,
	TR1_APE,
	TR1_CROCODILE_LAND,
	TR1_CROCODILE_WATER,
	TR1_COWBOY,
	TR1_KOLD,
	TR1_BAT,
	TR1_RAT_LAND,
	TR1_RAT_WATER,
	TR1_PANTHER,
	TR1_PIERRE,
	TR1_LARSON,
	TR1_TREX,
	TR1_RAPTOR,
	TR1_LION_FEMALE,
	TR1_LION_MALE,
	TR1_ATLANTEAN,
	TR1_ATLANTEAN_WINGS, // Winged_Mutant
	TR1_ATLANTEAN_CENTAUR,
	TR1_ATLANTEAN_GIANT,
	TR1_NATLA,
	TR1_NATLA_ATLANTEAN,
	TR1_DOPPELGANGER,
	TR1_DOPPELGANGER_ORIGIN,
	TR1_SKATEBOARD,
	TR1_SKATEBOARD_KID,

	TR1_DART,
	TR1_DART_EMITTER,
	TR1_SWINGING_BLADE_OR_AXE,
	TR1_SLAMMING_DOORS,
	TR1_CLASSIC_ROLLING_BALL,
	TR1_FALLING_BLOCK,
	TR1_SWORD_OF_DAMOCLES,
	TR1_THOR_HAMMER_HANDLE,
	TR1_THOR_HAMMER_BLOCK,
	TR1_THOR_ELECTRICAL_BALL,
	TR1_FALLING_CEILING,
	TR1_METAL_ROD,

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
	SHATTER0, SHATTER1, SHATTER2, SHATTER3, SHATTER4, SHATTER5, SHATTER6, SHATTER7, SHATTER8, SHATTER9,
	NUMBER_STATIC_OBJECTS
};

struct MATRIX_INT
{
	long m00;
	long m01;
	long m02;
	long m03;

	long m10;
	long m11;
	long m12;
	long m13;

	long m20;
	long m21;
	long m22;
	long m23;
};

struct MATRIX_FLT
{
	float m00;
	float m01;
	float m02;
	float m03;

	float m10;
	float m11;
	float m12;
	float m13;

	float m20;
	float m21;
	float m22;
	float m23;
};

struct CVECTOR
{
	char b;
	char g;
	char r;
	char a;
};

struct SPHERE
{
	long x;
	long y;
	long z;
	long r;
};

struct PHD_VECTOR
{
	long x;
	long y;
	long z;
};

struct PHD_3DPOS
{
	long x_pos;
	long y_pos;
	long z_pos;
	short x_rot;
	short y_rot;
	short z_rot;
};

struct GAME_VECTOR
{
	long x;
	long y;
	long z;
	short room_number;
	short box_number;
};

struct OBJECT_VECTOR
{
	long x;
	long y;
	long z;
	short data;
	short flags;
};

struct FVECTOR
{
	float x;
	float y;
	float z;
};

struct SVECTOR
{
	short x;
	short y;
	short z;
	short pad;
};

struct PCLIGHT
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	long shadow;
	float Inner;
	float Outer;
	float InnerAngle;
	float OuterAngle;
	float Cutoff;
	float nx;
	float ny;
	float nz;
	long ix;
	long iy;
	long iz;
	long inx;
	long iny;
	long inz;
	float tr;
	float tg;
	float tb;
	float rs;
	float gs;
	float bs;
	long fcnt;
	uchar Type;
	uchar Active;
	PHD_VECTOR rlp;
	long Range;
};

struct ITEM_LIGHT
{
	long r;
	long g;
	long b;
	long ambient;
	long rs;
	long gs;
	long bs;
	long fcnt;
	PCLIGHT	CurrentLights[21];
	PCLIGHT	PrevLights[21];
	long nCurrentLights;
	long nPrevLights;
	long room_number;
	long RoomChange;
	PHD_VECTOR item_pos;
	void* pCurrentLights;
	void* pPrevLights;
};

struct ItemInfo
{
	long floor;
	ulong touch_bits;
	ulong mesh_bits;
	short object_number;
	short current_anim_state;
	short goal_anim_state;
	short required_anim_state;
	short anim_number;
	short frame_number;
	short room_number;
	short next_item;
	short next_active;
	short speed;
	short fallspeed;
	short hit_points;
	ushort box_number;
	short timer;
	short flags;
	short shade;
	short trigger_flags;
	short carried_item;
	short after_death;
	ushort fired_weapon;
	short item_flags[4];
	void* data;
	PHD_3DPOS pos;
	ITEM_LIGHT il;
	ulong active : 1;
	ulong status : 2;
	ulong gravity_status : 1;
	ulong hit_status : 1;
	ulong collidable : 1;
	ulong looked_at : 1;
	ulong dynamic_light : 1;
	ulong poisoned : 1;
	ulong ai_bits : 5;
	ulong really_active : 1;
	ulong meshswap_meshbits;
	short draw_room;
	short TOSSPAD;
};

struct BOX_NODE
{
	short exit_box;
	ushort search_number;
	short next_expansion;
	short box_number;
};

struct LOT_INFO
{
	BOX_NODE* node;
	short head;
	short tail;
	ushort search_number;
	ushort block_mask;
	short step;
	short drop;
	short zone_count;
	short target_box;
	short required_box;
	short fly;
	ushort can_jump : 1;
	ushort can_monkey : 1;
	ushort is_amphibious : 1;
	ushort is_jumping : 1;
	ushort is_monkeying : 1;
	PHD_VECTOR target;
	ZONE_TYPES zone;
};

struct CreatureInfo
{
	short joint_rotation[4];
	short maximum_turn;
	short flags;
	ushort alerted : 1;
	ushort head_left : 1;
	ushort head_right : 1;
	ushort reached_goal : 1;
	ushort hurt_by_lara : 1;
	ushort patrol2 : 1;
	ushort jump_ahead : 1;
	ushort monkey_ahead : 1;
	MOOD_TYPES mood;
	ItemInfo* enemy;
	ItemInfo ai_target;
	short pad;
	short item_num;
	PHD_VECTOR target;
	LOT_INFO LOT;

	void DamageTarget(int damage)
	{
		if (enemy && enemy->hit_points > 0)
		{
			enemy->hit_points -= damage;
			enemy->hit_status = 1;
		}
	}
};

struct FX_INFO
{
	PHD_3DPOS pos;
	short room_number;
	short object_number;
	short next_fx;
	short next_active;
	short speed;
	short fallspeed;
	short frame_number;
	short counter;
	short shade;
	short flag1;
	short flag2;
};

struct LARA_ARM
{
	short* frame_base;
	short frame_number;
	short anim_number;
	short lock;
	short y_rot;
	short x_rot;
	short z_rot;
	short flash_gun;
};

struct LARA_INFO
{
	short item_number;
	short gun_status;
	short gun_type;
	short request_gun_type;
	short last_gun_type;
	short calc_fallspeed;
	short water_status;
	short climb_status;
	short pose_count;
	short hit_frame;
	short hit_direction;
	short air;
	short dive_count;
	short death_count;
	short current_active;
	short current_xvel;
	short current_yvel;
	short current_zvel;
	short spaz_effect_count;
	short flare_age;
	short vehicle;
	short weapon_item;
	short back_gun;
	short flare_frame;
	short poisoned;
	short dpoisoned;
	uchar electric;
	uchar wet[15];
	ushort flare_control_left : 1;
	ushort Unused1 : 1;
	ushort look : 1;
	ushort burn : 1;
	ushort keep_ducked : 1;
	ushort IsMoving : 1;
	ushort CanMonkeySwing : 1;
	ushort Unused2 : 1;
	ushort OnBeetleFloor : 1;
	ushort BurnGreen : 1;
	ushort IsDucked : 1;
	ushort has_fired : 1;
	ushort Busy : 1;
	ushort LitTorch : 1;
	ushort IsClimbing : 1;
	ushort Fired : 1;
	long water_surface_dist;
	PHD_VECTOR last_pos;
	FX_INFO* spaz_effect;
	long mesh_effects;
	short* mesh_ptrs[15];
	ItemInfo* target;
	short target_angles[2];
	short turn_rate;
	short move_angle;
	short head_y_rot;
	short head_x_rot;
	short head_z_rot;
	short torso_y_rot;
	short torso_x_rot;
	short torso_z_rot;
	LARA_ARM left_arm;
	LARA_ARM right_arm;
	ushort holster_l;
	ushort holster_r;
	CreatureInfo* creature;
	long CornerX;
	long CornerZ;
	char RopeSegment;
	char RopeDirection;
	short RopeArcFront;
	short RopeArcBack;
	short RopeLastX;
	short RopeMaxXForward;
	short RopeMaxXBackward;
	long RopeDFrame;
	long RopeFrame;
	ushort RopeFrameRate;
	ushort RopeY;
	long RopePtr;
	void* GeneralPtr;
	long RopeOffset;
	ulong RopeDownVel;
	char RopeFlag;
	char MoveCount;
	long RopeCount;
	char pistols_type_carried;
	char uzis_type_carried;
	char shotgun_type_carried;
	char crossbow_type_carried;
	char grenade_type_carried;
	char sixshooter_type_carried;
	char lasersight;
	char binoculars;
	char crowbar;
	char mechanical_scarab;
	uchar small_water_skin;
	uchar big_water_skin;
	char examine1;
	char examine2;
	char examine3;
	char puzzleitems[12];
	ushort puzzleitemscombo;
	ushort keyitems;
	ushort keyitemscombo;
	ushort pickupitems;
	ushort pickupitemscombo;
	short questitems;
	short num_small_medipack;
	short num_large_medipack;
	short num_flares;
	short num_pistols_ammo;
	short num_uzi_ammo;
	short num_revolver_ammo;
	short num_shotgun_ammo1;
	short num_shotgun_ammo2;
	short num_grenade_ammo1;
	short num_grenade_ammo2;
	short num_grenade_ammo3;
	short num_crossbow_ammo1;
	short num_crossbow_ammo2;
	short num_crossbow_ammo3;
	char beetle_uses;
	char blindTimer;
	char location;
	char highest_location;
	char locationPad;
};

struct GAMEFLOW
{
	ulong CheatEnabled : 1;
	ulong LoadSaveEnabled : 1;
	ulong TitleEnabled : 1;
	ulong PlayAnyLevel : 1;
	ulong Language : 3;
	ulong DemoDisc : 1;
	ulong Unused : 24;
	ulong InputTimeout;
	uchar SecurityTag;
	uchar nLevels;
	uchar nFileNames;
	uchar Pad;
	ushort FileNameLen;
	ushort ScriptLen;
};

struct CAMERA_INFO
{
	GAME_VECTOR pos;
	GAME_VECTOR target;
	camera_type type;
	camera_type old_type;
	long shift;
	long flags;
	long fixed_camera;
	long number_frames;
	long bounce;
	long underwater;
	long target_distance;
	short target_angle;
	short target_elevation;
	short actual_elevation;
	short actual_angle;
	short number;
	short last;
	short timer;
	short speed;
	ItemInfo* item;
	ItemInfo* last_item;
	OBJECT_VECTOR* fixed;
	long mike_at_lara;
	PHD_VECTOR mike_pos;
};

struct FACE4
{
	ushort vertices[4];
	ushort texture;
	ushort flags;
};

struct FACE3
{
	ushort vertices[3];
	ushort texture;
	ushort flags;
};

struct COLL_INFO
{
	long mid_floor;
	long mid_ceiling;
	long mid_type;
	long front_floor;
	long front_ceiling;
	long front_type;
	long left_floor;
	long left_ceiling;
	long left_type;
	long right_floor;
	long right_ceiling;
	long right_type;
	long left_floor2;
	long left_ceiling2;
	long left_type2;
	long right_floor2;
	long right_ceiling2;
	long right_type2;
	long r;
	long bad_pos;
	long bad_neg;
	long bad_ceiling;
	PHD_VECTOR shift;
	PHD_VECTOR old;
	short old_anim_state;
	short old_anim_number;
	short old_frame_number;
	short facing;
	short quadrant;
	short coll_type;
	short* trigger;
	char tilt_x;
	char tilt_z;
	char hit_by_baddie;
	char hit_static;
	ushort slopes_are_walls : 2;
	ushort slopes_are_pits : 1;
	ushort lava_is_pit : 1;
	ushort enable_baddie_push : 1;
	ushort enable_spaz : 1;
	ushort hit_ceiling : 1;
};

typedef std::function<void(short item_number)> Obj_InitialiseFunc;
typedef std::function<void(short item_number, ItemInfo* laraitem, COLL_INFO* coll)> Obj_CollisionFunc;
typedef std::function<void(short item_number)> Obj_ControlFunc;
typedef std::function<void(ItemInfo* item, long x, long y, long z, long* height)> Obj_FloorFunc;
typedef std::function<void(ItemInfo* item, long x, long y, long z, long* height)> Obj_CeilingFunc;
typedef std::function<void(ItemInfo* item)> Obj_DrawRoutineFunc;
typedef std::function<void(ItemInfo* item)> Obj_DrawRoutineExtraFunc;

struct OBJECT_INFO
{
	short nmeshes;
	short mesh_index;
	long bone_index;
	short* frame_base;
	Obj_InitialiseFunc initialise;
	Obj_CollisionFunc collision;
	Obj_ControlFunc control;
	Obj_FloorFunc floor;
	Obj_CeilingFunc ceiling;
	Obj_DrawRoutineFunc draw_routine;
	Obj_DrawRoutineExtraFunc draw_routine_extra;
	short anim_index;
	short hit_points;
	short pivot_length;
	short r;
	short shadow_size;
	short bite_offset;
	bool loaded;
	bool intelligent;
	bool is_pickup;
	bool is_puzzlehole;
	bool undead;
	bool non_lot;
	bool save_position;
	bool save_hitpoints;
	bool save_flags;
	bool save_anim;
	bool save_mesh;
	bool semi_transparent;
	bool water_creature;
	int hit_effect;
	DWORD explodable_meshbits;
};

struct FloorInfo
{
	ushort index;
	ushort fx : 4;
	ushort box : 11;
	ushort stopper : 1;
	uchar pit_room;
	char floor;
	uchar sky_room;
	char ceiling;
};

struct LIGHTINFO
{
	long x;
	long y;
	long z;
	uchar r;
	uchar g;
	uchar b;
	uchar Type;
	short Intensity;
	float Inner;
	float Outer;
	float Length;
	float Cutoff;
	float nx;
	float ny;
	float nz;
};

struct FOGBULB_STRUCT
{
	FVECTOR WorldPos;
	FVECTOR pos;
	FVECTOR vec;
	float rad;
	float sqrad;
	float inv_sqrad;
	float dist;
	long density;
	long inRange;
	long timer;
	long active;
	long FXRad;
	long room_number;
	long r;
	long g;
	long b;
};

struct MESH_INFO
{
	long x;
	long y;
	long z;
	short y_rot;
	short shade;
	short Flags;
	short static_number;
};

struct PCLIGHT_INFO
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	long shadow;
	float Inner;
	float Outer;
	float InnerAngle;
	float OuterAngle;
	float Cutoff;
	float nx;
	float ny;
	float nz;
	long ix;
	long iy;
	long iz;
	long inx;
	long iny;
	long inz;
	uchar Type;
	uchar Pad;
};

struct ROOM_INFO
{
	short* data;
	short* door;
	FloorInfo* floor;
	LIGHTINFO* light;
	MESH_INFO* mesh;
	long x;
	long y;
	long z;
	long minfloor;
	long maxceiling;
	short x_size;
	short y_size;
	long ambient;
	short num_lights;
	short num_meshes;
	uchar ReverbType;
	uchar FlipNumber;
	char MeshEffect;
	char bound_active;
	short left;
	short right;
	short top;
	short bottom;
	short test_left;
	short test_right;
	short test_top;
	short test_bottom;
	short item_number;
	short fx_number;
	short flipped_room;
	ushort flags;
	long nVerts;
	long nWaterVerts;
	long nShoreVerts;
	LPDIRECT3DVERTEXBUFFER vb;
	short* FaceData;
	float posx;
	float posy;
	float posz;
	D3DVECTOR* vnormals;
	D3DVECTOR* fnormals;
	long* prelight;
	long* prelightwater;
	long watercalc;
	D3DVECTOR* verts;
	long gt3cnt;
	long gt4cnt;
	PCLIGHT_INFO* pclight;
};

struct ANIM_STRUCT
{
	short* frame_ptr;
	short interpolation;
	short current_anim_state;
	long velocity;
	long acceleration;
	long Xvelocity;
	long Xacceleration;
	short frame_base;
	short frame_end;
	short jump_anim_num;
	short jump_frame_num;
	short number_changes;
	short change_index;
	short number_commands;
	short command_index;
};

struct ROPE_STRUCT
{
	PHD_VECTOR Segment[24];
	PHD_VECTOR Velocity[24];
	PHD_VECTOR NormalisedSegment[24];
	PHD_VECTOR MeshSegment[24];
	PHD_VECTOR Position;
	long Coords[24][3];
	long SegmentLength;
	long Active;
};

struct PENDULUM
{
	PHD_VECTOR Position;
	PHD_VECTOR Velocity;
	long node;
	ROPE_STRUCT* Rope;
};

struct RotateBasedOnSlope
{
	short x_rot = 0;
	short z_rot = 0;
	RotateBasedOnSlope() = default;
	RotateBasedOnSlope(const short& xrot, const short& zrot) : x_rot(xrot), z_rot(zrot) {}
};

struct STATS
{
	ulong Timer;
	ulong Distance;
	ulong AmmoUsed;
	ulong AmmoHits;
	ushort Kills;
	uchar Secrets;
	uchar HealthUsed;
};

struct SAVEGAME_INFO
{
	LARA_INFO Lara;
	long cutscene_triggered;
	uchar HubLevels[10];	//saved level indices. highest one that isn't 0 is the one we are currently in
	ushort HubOffsets[10];	//offset of each level's data inside the savegame buffer
	ushort HubSizes[10];	//size of each level's data inside the savegame buffer
	char CurrentLevel;
	char Checksum;
	STATS Game;
	STATS Level;
	short WeaponObject;
	short WeaponAnim;
	short WeaponFrame;
	short WeaponCurrent;
	short WeaponGoal;
	CVECTOR fog_colour;
	uchar HubSavedLara : 1;	//flag that we saved Lara's data when we initialised hub, only set to 1 when InitialiseHub is called with 1
	uchar AutoTarget : 1;
	uchar HaveBikeBooster : 1;	//have the bike nitro thing
	char buffer[15410];
};

struct BIKEINFO
{
	short right_front_wheelrot;
	short right_back_wheelrot;
	long left_wheelrot;
	long velocity;
	long unused1;
	long pitch1;
	short move_angle;
	short extra_rotation;
	short rot_thing;
	long bike_turn;
	long pitch2;
	short flags;
	short light_intensity;
};

struct SPARKS
{
	long x;
	long y;
	long z;
	short Xvel;
	short Yvel;
	short Zvel;
	short Gravity;
	short RotAng;
	short Flags;
	uchar sSize;
	uchar dSize;
	uchar Size;
	uchar Friction;
	uchar Scalar;
	uchar Def;
	char RotAdd;
	char MaxYvel;
	uchar On;
	uchar sR;
	uchar sG;
	uchar sB;
	uchar dR;
	uchar dG;
	uchar dB;
	uchar R;
	uchar G;
	uchar B;
	uchar ColFadeSpeed;
	uchar FadeToBlack;
	uchar sLife;
	uchar Life;
	uchar TransType;
	uchar extras;
	char Dynamic;
	uchar FxObj;
	uchar RoomNumber;
	uchar NodeNumber;
};

struct STATIC_INFO
{
	short mesh_number;
	short flags;
	short x_minp;
	short x_maxp;
	short y_minp;
	short y_maxp;
	short z_minp;
	short z_maxp;
	short x_minc;
	short x_maxc;
	short y_minc;
	short y_maxc;
	short z_minc;
	short z_maxc;
};

struct DXPTR
{
	LPDIRECTDRAWX lpDD;
	LPDIRECT3DX lpD3D;
	LPDIRECT3DDEVICEX lpD3DDevice;
	LPDIRECT3DDEVICEX _lpD3DDevice;
	LPDIRECTDRAWSURFACEX lpPrimaryBuffer;
	LPDIRECTDRAWSURFACEX lpBackBuffer;
	LPDIRECTDRAWSURFACEX lpZBuffer;
	LPDIRECT3DVIEWPORTX lpViewport;
	LPDIRECTSOUND8 lpDS;
	IXAudio2* lpXA;
	ulong dwRenderWidth;
	ulong dwRenderHeight;
	RECT rViewport;
	RECT rScreen;
	long Flags;
	ulong WindowStyle;
	long CoopLevel;
	LPDIRECTINPUTX lpDirectInput;
	LPDIRECTINPUTDEVICEX Keyboard;
	HWND hWnd;
	volatile long InScene;
	volatile long WaitAtBeginScene;
	volatile long DoneBlit;
};

struct DXDISPLAYMODE
{
	long w;
	long h;
	long bpp;
	long RefreshRate;
	long bPalette;
	DDSURFACEDESCX ddsd;
	uchar rbpp;
	uchar gbpp;
	uchar bbpp;
	uchar rshift;
	uchar gshift;
	uchar bshift;
};

struct DXTEXTUREINFO
{
	DDPIXELFORMAT ddpf;
	ulong bpp;
	long bPalette;
	long bAlpha;
	uchar rbpp;
	uchar gbpp;
	uchar bbpp;
	uchar abpp;
	uchar rshift;
	uchar gshift;
	uchar bshift;
	uchar ashift;
};

struct DXZBUFFERINFO
{
	DDPIXELFORMAT ddpf;
	ulong bpp;
};

struct DXD3DDEVICE
{
	char Name[30];
	char About[80];
	LPGUID lpGuid;
	GUID Guid;
	D3DDEVICEDESC DeviceDesc;
	long bHardware;
	long nDisplayModes;
	DXDISPLAYMODE* DisplayModes;
	long nTextureInfos;
	DXTEXTUREINFO* TextureInfos;
	long nZBufferInfos;
	DXZBUFFERINFO* ZBufferInfos;
};

struct DXDIRECTDRAWINFO
{
	char Name[30];
	char About[80];
	LPGUID lpGuid;
	GUID Guid;
	DDCAPS DDCaps;
	DDDEVICEIDENTIFIER DDIdentifier;
	long nDisplayModes;
	DXDISPLAYMODE* DisplayModes;
	long nD3DDevices;
	DXD3DDEVICE* D3DDevices;
};

struct DXDIRECTSOUNDINFO
{
	char Name[30];
	char About[80];
	LPGUID lpGuid;
	GUID Guid;
};

struct DXINFO
{
	long nDDInfo;
	long nDSInfo;
	DXDIRECTDRAWINFO* DDInfo;
	DXDIRECTSOUNDINFO* DSInfo;
	long nDD;
	long nD3D;
	long nDisplayMode;
	long nTexture;
	long nZBuffer;
	long nDS;
	bool bHardware;
};

struct WINAPP
{
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASS WindowClass;
	DXINFO DXInfo;
	DXPTR dx;
	HANDLE mutex;
	float fps;
	LPDIRECT3DMATERIALX GlobalMaterial;
	D3DMATERIALHANDLE GlobalMaterialHandle;
	HACCEL hAccel;
	bool SetupComplete;
	bool BumpMapping;
	long TextureSize;
	long BumpMapSize;
	bool mmx;
	bool Filtering;
	bool Volumetric;
	bool SoundDisabled;
	long StartFlags;
	volatile bool fmv;
	long Desktopbpp;
	long AutoTarget;
};

struct SPRITESTRUCT
{
	ushort tpage;
	ushort offset;
	ushort width;
	ushort height;
	float x1;	//left
	float y1;	//top
	float x2;	//right
	float y2;	//bottom
};

struct MESH_DATA
{
	short x;
	short y;
	short z;
	short r;
	short flags;
	short nVerts;
	short nNorms;
	short ngt4;
	FACE4* gt4;
	short ngt3;
	FACE3* gt3;
	long* prelight;
	LPDIRECT3DVERTEXBUFFER vb;
	D3DVECTOR* normals;
};

struct TEXTURESTRUCT
{
	ushort drawtype;
	ushort tpage;
	ushort flag;
	float u1;
	float v1;
	float u2;
	float v2;
	float u3;
	float v3;
	float u4;
	float v4;
};

struct LIGHTNING_STRUCT
{
	PHD_VECTOR Point[4];
	uchar r;
	uchar g;
	uchar b;
	uchar Life;
	char Xvel1;
	char Yvel1;
	char Zvel1;
	char Xvel2;
	char Yvel2;
	char Zvel2;
	char Xvel3;
	char Yvel3;
	char Zvel3;
	uchar Size;
	uchar Flags;
	uchar Rand;
	uchar Segments;
	uchar Pad[3];
};

struct DYNAMIC
{
	long x;
	long y;
	long z;
	uchar on;
	uchar r;
	uchar g;
	uchar b;
	ushort falloff;
	uchar used;
	uchar pad1[1];
	long FalloffScale;
};

struct INVOBJ
{
	short object_number;
	short yoff;
	short scale1;
	short yrot;
	short xrot;
	short zrot;
	short flags;
	short objname;
	ulong meshbits;
};

struct MENUTHANG
{
	long type;
	char* text;
};

struct AMMOLIST
{
	short invitem;
	short amount;
	ushort yrot;
};

struct OBJLIST
{
	short invitem;
	ushort yrot;
	ushort bright;
};

struct INVDRAWITEM
{
	short xrot;
	short yrot;
	short zrot;
	short object_number;
	ulong mesh_bits;
};

struct RINGME
{
	OBJLIST current_object_list[119];
	long ringactive;
	long objlistmovement;
	long curobjinlist;
	long numobjectsinlist;
};

struct COMBINELIST
{
	void(*combine_routine)(long flag);
	short item1;
	short item2;
	short combined_item;
};

struct CUTSEQ_ROUTINES
{
	void(*init_func)();
	void(*control_func)();
	void(*end_func)();
};

struct ACTORME
{
	long offset;
	short objslot;
	short nodes;
};

struct NEW_CUTSCENE
{
	short numactors;
	short numframes;
	long orgx;
	long orgy;
	long orgz;
	long audio_track;
	long camera_offset;
	ACTORME actor_data[10];
};

struct RTDECODE
{
	ulong length;
	ulong off;
	ushort counter;
	ushort data;
	uchar decodetype;
	uchar packmethod;
	ushort padfuck;
};

struct PACKNODE
{
	short xrot_run;
	short yrot_run;
	short zrot_run;
	short xkey;
	short ykey;
	short zkey;
	RTDECODE decode_x;
	RTDECODE decode_y;
	RTDECODE decode_z;
	ulong xlength;
	ulong ylength;
	ulong zlength;
	char* xpacked;
	char* ypacked;
	char* zpacked;
};

struct NODELOADHEADER
{
	short xkey;
	short ykey;
	short zkey;
	short packmethod;
	short xlength;
	short ylength;
	short zlength;
};

struct HAIR_STRUCT
{
	PHD_3DPOS pos;
	PHD_VECTOR vel;
};

struct SORTLIST
{
	float zVal;
	short drawtype;
	short tpage;
	short nVtx;
	short polytype;
};

struct WATERTAB
{
	char shimmer;
	char choppy;
	uchar random;
	uchar abs;
};

struct FOOTPRINT
{
	long x;
	long y;
	long z;
	short YRot;
	short Active;
};

struct DISPLAYPU
{
	short life;
	short object_number;
};

struct GUNSHELL_STRUCT
{
	PHD_3DPOS pos;
	short fallspeed;
	short room_number;
	short speed;
	short counter;
	short DirXrot;
	short object_number;
};

struct BiteInfo
{
	long x;
	long y;
	long z;
	long mesh_num;
};

struct D3DTLBUMPVERTEX
{
	D3DVALUE sx;
	D3DVALUE sy;
	D3DVALUE sz;
	D3DVALUE rhw;
	D3DCOLOR color;
	D3DCOLOR specular;
	D3DVALUE tu;
	D3DVALUE tv;
	D3DVALUE tx;
	D3DVALUE ty;
};

struct TEXTURE
{
	LPDIRECT3DTEXTUREX tex;
	LPDIRECTDRAWSURFACEX surface;
	ulong xoff;
	ulong yoff;
	ulong width;
	ulong height;
	long tpage;
	bool bump;
	long bumptpage;
};

struct TEXTUREBUCKET
{
	long tpage;
	long nVtx;
	D3DTLBUMPVERTEX vtx[544];
};

struct THREAD
{
	volatile long active;
	long unk;
	volatile long ended;
	uintptr_t handle;
	unsigned int address;
};

struct DRIP_STRUCT
{
	long x;
	long y;
	long z;
	uchar On;
	uchar R;
	uchar G;
	uchar B;
	short Yvel;
	uchar Gravity;
	uchar Life;
	short RoomNumber;
	uchar Outside;
	uchar Pad;
};

struct AIInfo
{
	short zone_number;
	short enemy_zone;
	long distance;
	long ahead;
	long bite;
	short angle;
	short x_angle;
	short enemy_facing;
};

struct AIOBJECT
{
	short object_number;
	short room_number;
	long x;
	long y;
	long z;
	short trigger_flags;
	short flags;
	short y_rot;
	short box_number;
};

struct OLD_CAMERA
{
	short current_anim_state;
	short goal_anim_state;
	long target_distance;
	short target_angle;
	short target_elevation;
	PHD_3DPOS pos;
	PHD_3DPOS pos2;
	PHD_VECTOR t;
};

struct SHATTER_ITEM
{
	SPHERE Sphere;
	ITEM_LIGHT* il;
	short* meshp;
	long Bit;
	short YRot;
	short Flags;
};

struct SPOTCAM
{
	long x;
	long y;
	long z;
	long tx;
	long ty;
	long tz;
	uchar sequence;
	uchar camera;
	short fov;
	short roll;
	short timer;
	short speed;
	short flags;
	short room_number;
	short pad;
};

struct WRAITH_STRUCT
{
	PHD_VECTOR pos;
	short xv;
	short yv;
	short zv;
	uchar r;
	uchar g;
	uchar b;
	uchar pad[3];
};

struct LOCUST_STRUCT
{
	PHD_3DPOS pos;
	short room_number;
	short speed;
	short Counter;
	short LaraTarget;
	char XTarget;
	char ZTarget;
	uchar On;
	uchar flags;
};

struct DOORPOS_DATA
{
	FloorInfo* floor;
	FloorInfo data;
	short block;
};

struct DOOR_DATA
{
	DOORPOS_DATA d1;
	DOORPOS_DATA d1flip;
	DOORPOS_DATA d2;
	DOORPOS_DATA d2flip;
	short Opened;
};

struct BOX_INFO
{
	uchar left;
	uchar right;
	uchar top;
	uchar bottom;
	short height;
	short overlap_index;
};

struct SMOKE_SPARKS
{
	long x;
	long y;
	long z;
	short Xvel;
	short Yvel;
	short Zvel;
	short Gravity;
	short RotAng;
	short Flags;
	uchar sSize;
	uchar dSize;
	uchar Size;
	uchar Friction;
	uchar Scalar;
	uchar Def;
	char RotAdd;
	char MaxYvel;
	uchar On;
	uchar sShade;
	uchar dShade;
	uchar Shade;
	uchar ColFadeSpeed;
	uchar FadeToBlack;
	char sLife;
	char Life;
	uchar TransType;
	uchar FxObj;
	uchar NodeNumber;
	uchar mirror;
};

struct MONOSCREEN_STRUCT
{
	LPDIRECT3DTEXTUREX tex;
	LPDIRECTDRAWSURFACEX surface;
};

struct VonCroyCutData
{
	PHD_VECTOR CameraPos;
	PHD_VECTOR CameraTarget;
	long f;
};

struct DEBRIS_STRUCT
{
	void* TextInfo;
	long x;
	long y;
	long z;
	short XYZOffsets1[3];
	short Dir;
	short XYZOffsets2[3];
	short Speed;
	short XYZOffsets3[3];
	short Yvel;
	short Gravity;
	short RoomNumber;
	uchar On;
	uchar XRot;
	uchar YRot;
	uchar r;
	uchar g;
	uchar b;
	uchar Pad[2];
	long color1;
	long color2;
	long color3;
	long ambient;
	long flags;
};

struct JEEPINFO
{
	short right_front_wheelrot;
	short left_front_wheelrot;
	short left_back_wheelrot;
	short right_back_wheelrot;
	long velocity;
	long unused1;
	long pitch1;
	long turn_rate;
	long camera_angle;
	short move_angle;
	short extra_rotation;
	short rot_thing;
	long pitch2;
	short flags;
	short unused2;
	short gear;
};

struct PISTOL_DEF
{
	short ObjectNum;
	char Draw1Anim2;
	char Draw1Anim;
	char Draw2Anim;
	char RecoilAnim;
};

struct BINK_STRUCT
{
	long pad;
	long num;
	char padfuck[8];
	long num2;
};

struct SAVEFILE_INFO
{
	char name[75];
	char valid;
	short hours;
	short minutes;
	short seconds;
	short days;
	long num;
};

struct COMMANDLINES
{
	char command[20];
	bool needs_parameter;
	void (*code)(char*);
	char parameter[20];
};

struct CHANGE_STRUCT
{
	short goal_anim_state;
	short number_ranges;
	short range_index;
};

struct RANGE_STRUCT
{
	short start_frame;
	short end_frame;
	short link_anim_num;
	short link_frame_num;
};

struct PHDSPRITESTRUCT
{
	ushort tpage;
	ushort offset;
	ushort width;
	ushort height;
	short x1;
	short y1;
	short x2;
	short y2;
};

struct PHDTEXTURESTRUCT
{
	ushort drawtype;
	ushort tpage;
	ushort flag;
	ushort u1;
	ushort v1;
	ushort u2;
	ushort v2;
	ushort u3;
	ushort v3;
	ushort u4;
	ushort v4;
	ulong xoff;
	ulong yoff;
	ulong width;
	ulong height;
};

struct SAMPLE_INFO
{
	short number;
	uchar volume;
	uchar r;
	uchar randomness;
	char pitch;
	short flags;
};

struct DS_SAMPLE
{
	LPDIRECTSOUNDBUFFER buffer;
	long frequency;
	long playing;
};

struct BUBBLE_STRUCT
{
	PHD_VECTOR pos;
	short room_number;
	short speed;
	short size;
	short dsize;
	uchar shade;
	uchar vel;
	short pad;
};

struct SHOCKWAVE_STRUCT
{
	long x;
	long y;
	long z;
	short InnerRad;
	short OuterRad;
	short XRot;
	short Flags;
	uchar r;
	uchar g;
	uchar b;
	uchar life;
	short Speed;
	short Temp;
};

struct SPLASH_STRUCT
{
	long x;
	long y;
	long z;
	short InnerRad;
	short InnerSize;
	short InnerRadVel;
	short InnerYVel;
	short InnerY;
	short MiddleRad;
	short MiddleSize;
	short MiddleRadVel;
	short MiddleYVel;
	short MiddleY;
	short OuterRad;
	short OuterSize;
	short OuterRadVel;
	char flags;
	uchar life;
};

struct RIPPLE_STRUCT
{
	long x;
	long y;
	long z;
	char flags;
	uchar life;
	uchar size;
	uchar init;
};

struct FIRE_SPARKS
{
	short x;
	short y;
	short z;
	short Xvel;
	short Yvel;
	short Zvel;
	short Gravity;
	short RotAng;
	short Flags;
	uchar sSize;
	uchar dSize;
	uchar Size;
	uchar Friction;
	uchar Scalar;
	uchar Def;
	char RotAdd;
	char MaxYvel;
	uchar On;
	uchar sR;
	uchar sG;
	uchar sB;
	uchar dR;
	uchar dG;
	uchar dB;
	uchar R;
	uchar G;
	uchar B;
	uchar ColFadeSpeed;
	uchar FadeToBlack;
	uchar sLife;
	uchar Life;
};

struct BLOOD_STRUCT
{
	long x;
	long y;
	long z;
	short Xvel;
	short Yvel;
	short Zvel;
	short Gravity;
	short RotAng;
	uchar sSize;
	uchar dSize;
	uchar Size;
	uchar Friction;
	char RotAdd;
	uchar On;
	uchar sShade;
	uchar dShade;
	uchar Shade;
	uchar ColFadeSpeed;
	uchar FadeToBlack;
	char sLife;
	char Life;
	char Pad;
};

struct WATER_DUST
{
	PHD_VECTOR pos;
	char xvel;
	uchar yvel;
	char zvel;
	uchar life;
};

struct CHARDEF
{
	float u;
	float v;
	short w;
	short h;
	short YOffset;
	char TopShade;
	char BottomShade;
};

struct STRINGHEADER
{
	ushort nStrings;
	ushort nPSXStrings;
	ushort nPCStrings;
	ushort StringWadLen;
	ushort PSXStringWadLen;
	ushort PCStringWadLen;
};

struct GUNFLASH_STRUCT
{
	MATRIX_FLT mx;
	short on;
};

struct FIRE_LIST
{
	long x;
	long y;
	long z;
	char on;
	char size;
	short room_number;
};

struct SoundSlot
{
	long OrigVolume;
	long nVolume;
	long nPan;
	long nPitch;
	long nSampleInfo;
	ulong distance;
	PHD_VECTOR pos;
};

struct WEAPON_INFO
{
	short lock_angles[4];
	short left_angles[4];
	short right_angles[4];
	short aim_speed;
	short shot_accuracy;
	short gun_height;
	short target_dist;
	char damage;
	char recoil_frame;
	char flash_time;
	char draw_frame;
	short sample_num;
};

struct SCARAB_STRUCT
{
	PHD_3DPOS pos;
	short room_number;
	short speed;
	short fallspeed;
	uchar On;
	uchar flags;
};

struct SPLASH_SETUP
{
	long x;
	long y;
	long z;
	short InnerRad;
	short InnerSize;
	short InnerRadVel;
	short InnerYVel;
	short pad1;
	short MiddleRad;
	short MiddleSize;
	short MiddleRadVel;
	short MiddleYVel;
	short pad2;
	short OuterRad;
	short OuterSize;
	short OuterRadVel;
	short pad3;
};

struct SP_DYNAMIC
{
	uchar On;
	uchar Falloff;
	uchar R;
	uchar G;
	uchar B;
	uchar Flags;
	uchar Pad[2];
};

struct NODEOFFSET_INFO
{
	short x;
	short y;
	short z;
	char mesh_num;
	uchar GotIt;
};

struct TRAIN_STATIC
{
	short type;
	short zoff;
};

struct ROOM_DYNAMIC
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float falloff;
	float sqr_falloff;
	float inv_falloff;
};

struct SUNLIGHT_STRUCT
{
	FVECTOR vec;
	float r;
	float g;
	float b;
};

struct POINTLIGHT_STRUCT
{
	FVECTOR vec;
	float r;
	float g;
	float b;
	float rad;
};

struct GouraudBarColourSet
{
	uchar abLeftRed[5];
	uchar abLeftGreen[5];
	uchar abLeftBlue[5];
	uchar abRightRed[5];
	uchar abRightGreen[5];
	uchar abRightBlue[5];
};

struct COLOR_BIT_MASKS
{
	ulong dwRBitMask;
	ulong dwGBitMask;
	ulong dwBBitMask;
	ulong dwRGBAlphaBitMask;
	ulong dwRBitDepth;
	ulong dwGBitDepth;
	ulong dwBBitDepth;
	ulong dwRGBAlphaBitDepth;
	ulong dwRBitOffset;
	ulong dwGBitOffset;
	ulong dwBBitOffset;
	ulong dwRGBAlphaBitOffset;
};

struct tomb4_options	//keep this at the bottom of the file, please
{
	bool footprints;
	ulong shadow_mode;			//1-> original, 2-> circle, 3-> PSX like circle, 4-> PSX sprite, 5-> dynamic
	bool crawltilt;
	bool flexible_crawling;
	bool fix_climb_up_delay;
	bool gameover;
	ulong bar_mode;				//1-> original, 2-> TR5, 3-> PSX
	ulong bars_pos;				//1-> original, 2-> improved, 3-> PSX
	bool enemy_bars;
	bool cutseq_skipper;
	bool cheats;
	bool loadingtxt;
	ulong inv_bg_mode;			//1-> original, 2->TR5, 3-> clear
	bool tr5_loadbar;
	bool look_transparency;
	bool ammo_counter;
	bool ammotype_hotkeys;
	bool combat_cam_tilt;
	bool hpbar_inv;
	bool static_lighting;
	ulong reverb;				//1-> off, 2-> Lara room, 3->camera room
	ulong distance_fog;			//value in blocks
	float GUI_Scale;
};
#pragma pack(pop)