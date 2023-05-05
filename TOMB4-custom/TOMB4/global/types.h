#pragma once
#include "math_tbls.h"
#include "../tomb4/bass.h"
#include "../tomb4/bassmix.h"
#include "../tomb4/bass_fx.h"

#pragma pack(push, 1)

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

enum carried_weapon_flags
{
	W_NONE =		0x0,
	W_PRESENT =		0x1,
	W_FLASHLIGHT =	0x2,	//speculation, actually unused
	W_LASERSIGHT =	0x4,
	W_AMMO1 =		0x8,
	W_AMMO2 =		0x10,
	W_AMMO3 =		0x20
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
	GUARD =		1 << 0,
	AMBUSH =	1 << 1,
	PATROL1 =	1 << 2,
	MODIFY =	1 << 3,
	FOLLOW =	1 << 4
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
	FF_SMALL =		0x1000,
	FF_BLINK =		0x2000,
	FF_RJUSTIFY =	0x4000,
	FF_CENTER =		0x8000
};

enum room_flags
{
	ROOM_UNDERWATER =	0x1,
	ROOM_OUTSIDE =		0x8,
	ROOM_DYNAMIC_LIT =	0x10,
	ROOM_NOT_INSIDE =	0x20,
	ROOM_INSIDE =		0x40,
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
	CT_NONE =			0x0,
	CT_FRONT =			0x1,
	CT_LEFT =			0x2,
	CT_RIGHT =			0x4,
	CT_TOP =			0x8,
	CT_TOP_FRONT =		0x10,
	CT_CLAMP =			0x20
};

enum sfx_types
{
	SFX_LANDANDWATER =	0,
	SFX_LANDONLY =		0x4000,
	SFX_WATERONLY =		0x8000
};

enum target_type
{
	NO_TARGET,
	PRIME_TARGET,
	SECONDARY_TARGET
};

enum mood_type
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

enum WEAPON_TYPES
{
	WEAPON_NONE,
	WEAPON_FLARE,
	WEAPON_TORCH,
	WEAPON_PISTOLS,
	WEAPON_UZI,
	WEAPON_MAGNUMS,
	WEAPON_AUTOPISTOLS,
	WEAPON_DESERTEAGLE,
	WEAPON_REVOLVER,
	WEAPON_SHOTGUN,
	WEAPON_M16,
	WEAPON_MP5,
	WEAPON_CROSSBOW,
	WEAPON_HARPOON,
	WEAPON_GRENADE,
	WEAPON_ROCKET,
	WEAPON_GRAPPLING,
	WEAPON_SNOWMOBILEGUN,
	NUM_WEAPONS
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

enum MATRIX_INDICES
{
	M00, M01, M02, M03,
	M10, M11, M12, M13,
	M20, M21, M22, M23,
	MATRIX_COUNT
};

enum input_buttons
{
	IN_NONE =				0x0,
	IN_FORWARD =			0x1,
	IN_BACK =				0x2,
	IN_LEFT =				0x4,
	IN_RIGHT =				0x8,
	IN_JUMP =				0x10,
	IN_DRAW =				0x20,
	IN_ACTION =				0x40,
	IN_WALK =				0x80,
	IN_OPTION =				0x100,
	IN_LOOK =				0x200,
	IN_LSTEP =				0x400,
	IN_RSTEP =				0x800,
	IN_ROLL =				0x1000,
	IN_PAUSE =				0x2000,
	IN_A =					0x4000,
	IN_B =					0x8000,
	IN_CHEAT =				0x10000,
	IN_D =					0x20000,
	IN_C =					0x30000,
	IN_E =					0x40000,
	IN_FLARE =				0x80000,
	IN_SELECT =				0x100000,
	IN_DESELECT =			0x200000,
	IN_SAVE =				0x400000,
	IN_LOAD =				0x800000,
	IN_STEPSHIFT =			0x1000000,
	IN_LOOKLEFT =			0x2000000,
	IN_LOOKRIGHT =			0x4000000,
	IN_LOOKFORWARD =		0x8000000,
	IN_LOOKBACK =			0x10000000,
	IN_DUCK =				0x20000000,
	IN_SPRINT =				0x40000000,
	IN_TARGET =				0x80000000,
	IN_ALL =				0xFFFFFFFF
};

enum ITEM_FLAGS
{
	IFL_TRIGGERED =				0x20,
	IFL_SWITCH_ONESHOT =		0x40,	//oneshot for switch items
	IFL_ANTITRIGGER_ONESHOT =	0x80,	//oneshot for antitriggers
	IFL_INVISIBLE =				0x100,	//also used as oneshot for everything else
	IFL_CODEBITS =				0x3E00,
	IFL_REVERSE =				0x4000,
	IFL_CLEARBODY =				0x8000
};

enum LARA_GUN_STATUS
{
	LG_NO_ARMS,
	LG_HANDS_BUSY,
	LG_DRAW_GUNS,
	LG_UNDRAW_GUNS,
	LG_READY,
	LG_FLARE,
};

enum CAMERA_TYPES
{
	CHASE_CAMERA,
	FIXED_CAMERA,
	LOOK_CAMERA,
	COMBAT_CAMERA,
	CINEMATIC_CAMERA,
	HEAVY_CAMERA,
};

enum LIGHT_TYPES
{
	LIGHT_SUN,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_SHADOW,
	LIGHT_FOG
};

enum GF_LEVEL_OPTIONS
{
	GF_YOUNGLARA =		0x1,
	GF_WEATHER =		0x2,
	GF_HORIZON =		0x4,
	GF_LAYER1 =			0x8,
	GF_LAYER2 =			0x10,
	GF_STARFIELD =		0x20,
	GF_LIGHTNING =		0x40,
	GF_TRAIN =			0x80,
	GF_PULSE =			0x100,
	GF_HORIZONCOLADD =	0x200,
	GF_RESETHUB =		0x400,
	GF_LENSFLARE =		0x800,
	GF_TIMER =			0x1000,
	GF_MIRROR =			0x2000,
	GF_REMOVEAMULET =	0x4000,
	GF_NOLEVEL =		0x8000
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

struct PHD_ANGLE
{
	short x = 0;
	short y = 0;
	short z = 0;

	PHD_ANGLE() {}
	PHD_ANGLE(short _x, short _y, short _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

struct PHD_VECTOR
{
	long x = 0;
	long y = 0;
	long z = 0;

	PHD_VECTOR() {}
	PHD_VECTOR(long _x, long _y, long _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

struct GAME_VECTOR
{
	long x = 0;
	long y = 0;
	long z = 0;
	short room_number = 255;
	short box_number = 2048;

	GAME_VECTOR() {}
	GAME_VECTOR(long _x, long _y, long _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

struct OBJECT_VECTOR
{
	long x;
	long y;
	long z;
	short data;
	short flags;
};

struct PHD_3DPOS
{
	long x_pos = 0;
	long y_pos = 0;
	long z_pos = 0;
	short x_rot = 0;
	short y_rot = 0;
	short z_rot = 0;

	PHD_3DPOS() {}
	PHD_3DPOS(long _x, long _y, long _z, short _xr, short _yr, short _zr)
	{
		x_pos = _x;
		y_pos = _y;
		z_pos = _z;
		x_rot = _xr;
		y_rot = _yr;
		z_rot = _zr;
	}
};

static const PHD_3DPOS Phd3DPosEmpty = PHD_3DPOS(0, 0, 0, 0, 0, 0);

struct FVECTOR
{
	float x = 0;
	float y = 0;
	float z = 0;

	FVECTOR() {}

	Vector3 ToVector3()
	{
		return Vector3(x, y, z);
	}

	BASS_3DVECTOR ToBassVector()
	{
		return BASS_3DVECTOR(x, y, z);
	}
};

struct SVECTOR
{
	short x = 0;
	short y = 0;
	short z = 0;
	short pad = 0;

	SVECTOR() {}
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
	unsigned char Type;
	unsigned char Active;
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

struct ITEM_INFO
{
	short index;
	long floor;
	unsigned long touch_bits;
	unsigned long mesh_bits;
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
	unsigned short box_number;
	short timer;
	short flags;
	short shade;
	short trigger_flags;
	short carried_item;
	short after_death;
	unsigned short fired_weapon;
	short item_flags[4];
	void* data;
	PHD_3DPOS pos;
	ITEM_LIGHT il;
	unsigned long active : 1;
	unsigned long status : 2;
	unsigned long gravity_status : 1;
	unsigned long hit_status : 1;
	unsigned long collidable : 1;
	unsigned long looked_at : 1;
	unsigned long dynamic_light : 1;
	unsigned long poisoned : 1;
	unsigned long ai_bits : 5;
	unsigned long really_active : 1;
	unsigned long meshswap_meshbits;
	short draw_room;
	short TOSSPAD;
};

struct BOX_NODE
{
	short exit_box;
	unsigned short search_number;
	short next_expansion;
	short box_number;
};

struct LOT_INFO
{
	BOX_NODE* node;
	short head;
	short tail;
	unsigned short search_number;
	unsigned short block_mask;
	short step;
	short drop;
	short zone_count;
	short target_box;
	short required_box;
	short fly;
	unsigned short can_jump : 1;
	unsigned short can_monkey : 1;
	unsigned short is_amphibious : 1;
	unsigned short is_jumping : 1;
	unsigned short is_monkeying : 1;
	PHD_VECTOR target;
	ZONE_TYPES zone;
};

struct CREATURE_INFO
{
	short joint_rotation[4];
	short maximum_turn;
	short flags;
	unsigned short alerted : 1;
	unsigned short head_left : 1;
	unsigned short head_right : 1;
	unsigned short reached_goal : 1;
	unsigned short hurt_by_lara : 1;
	unsigned short patrol2 : 1;
	unsigned short jump_ahead : 1;
	unsigned short monkey_ahead : 1;
	mood_type mood;
	ITEM_INFO* enemy;
	ITEM_INFO ai_target;
	short pad;
	short item_num;
	PHD_VECTOR target;
	LOT_INFO LOT;
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
	short flare_frame;
	short poisoned;
	short dpoisoned;
	short weapon_item;
	unsigned char electric;
	unsigned char wet[15];
	unsigned short flare_control_left : 1;
	unsigned short Unused1 : 1;
	unsigned short look : 1;
	unsigned short burn : 1;
	unsigned short keep_ducked : 1;
	unsigned short IsMoving : 1;
	unsigned short CanMonkeySwing : 1;
	unsigned short Unused2 : 1;
	unsigned short OnBeetleFloor : 1;
	unsigned short BurnGreen : 1;
	unsigned short IsDucked : 1;
	unsigned short has_fired : 1;
	unsigned short Busy : 1;
	unsigned short LitTorch : 1;
	unsigned short IsClimbing : 1;
	unsigned short Fired : 1;
	long water_surface_dist;
	PHD_VECTOR last_pos;
	FX_INFO* spaz_effect;
	long mesh_effects;
	short* mesh_ptrs[15];
	ITEM_INFO* target;
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
	unsigned short holster_right;
	unsigned short holster_left;
	unsigned short holster_back;
	CREATURE_INFO* creature;
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
	unsigned short RopeFrameRate;
	unsigned short RopeY;
	long RopePtr;
	void* GeneralPtr;
	long RopeOffset;
	unsigned long RopeDownVel;
	char RopeFlag;
	char MoveCount;
	long RopeCount;
	char pistols_type_carried;
	char uzis_type_carried;
	char magnums_type_carried;
	char autopistols_type_carried;
	char deserteagle_type_carried;
	char revolver_type_carried;
	char shotgun_type_carried;
	char m16_type_carried;
	char mp5_type_carried;
	char crossbow_type_carried;
	char harpoon_type_carried;
	char grenade_type_carried;
	char rocket_type_carried;
	char grappling_type_carried;
	char lasersight;
	char binoculars;
	char crowbar;
	char mechanical_scarab;
	unsigned char small_water_skin;
	unsigned char big_water_skin;
	char examine1;
	char examine2;
	char examine3;
	char puzzleitems[12];
	unsigned short puzzleitemscombo[16];
	unsigned short keyitems[12];
	unsigned short keyitemscombo[16];
	unsigned short pickupitems[4];
	unsigned short pickupitemscombo[8];
	short questitems[6];
	short num_small_medipack;
	short num_large_medipack;
	short num_flares;
	short num_pistols_ammo;
	short num_uzi_ammo;
	short num_magnums_ammo;
	short num_autopistols_ammo;
	short num_deserteagle_ammo;
	short num_revolver_ammo;
	short num_shotgun_ammo1;
	short num_shotgun_ammo2;
	short num_m16_ammo;
	short num_mp5_ammo;
	short num_crossbow_ammo1;
	short num_crossbow_ammo2;
	short num_crossbow_ammo3;
	short num_harpoon_ammo;
	short num_grenade_ammo1;
	short num_grenade_ammo2;
	short num_grenade_ammo3;
	short num_rocket_ammo;
	short num_grappling_ammo;
	short num_snowmobile_ammo;
	char beetle_uses;
	char blindTimer;
	char location;
	char highest_location;
	char locationPad;

	Vector3 FromVelocityToVector3()
	{
		return Vector3(float(current_xvel), float(current_yvel), float(current_zvel));
	}

	BASS_3DVECTOR FromVelocityToBassVector()
	{
		return BASS_3DVECTOR(float(current_xvel), float(current_yvel), float(current_zvel));
	}
};

struct GAMEFLOW
{
	unsigned long CheatEnabled : 1;
	unsigned long LoadSaveEnabled : 1;
	unsigned long TitleEnabled : 1;
	unsigned long PlayAnyLevel : 1;
	unsigned long Language : 3;
	unsigned long DemoDisc : 1;
	unsigned long Unused : 24;
	unsigned long InputTimeout;
	unsigned char SecurityTag;
	unsigned char nLevels;
	unsigned char nFileNames;
	unsigned char Pad;
	unsigned short FileNameLen;
	unsigned short ScriptLen;
};

struct CAMERA_INFO
{
	GAME_VECTOR pos;
	GAME_VECTOR target;
	CAMERA_TYPES type;
	CAMERA_TYPES old_type;
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
	ITEM_INFO* item;
	ITEM_INFO* last_item;
	OBJECT_VECTOR* fixed;
	long mike_at_lara;
	PHD_VECTOR mike_pos;
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
	long radius;
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
	unsigned short slopes_are_walls : 2;
	unsigned short slopes_are_pits : 1;
	unsigned short lava_is_pit : 1;
	unsigned short enable_baddie_push : 1;
	unsigned short enable_spaz : 1;
	unsigned short hit_ceiling : 1;
};

struct OBJECT_INFO
{
	short nmeshes;
	short mesh_index;
	long bone_index;
	short* frame_base;
	void (*initialise)(short item_number);
	void (*control)(short item_number);
	void (*floor)(ITEM_INFO* item, long x, long y, long z, long* height);
	void (*ceiling)(ITEM_INFO* item, long x, long y, long z, long* height);
	void (*draw_routine)(ITEM_INFO* item);
	void (*collision)(short item_num, ITEM_INFO* laraitem, COLL_INFO* coll);
	short object_mip;
	short anim_index;
	short hit_points;
	short pivot_length;
	short radius;
	short shadow_size;
	unsigned short bite_offset;
	unsigned short loaded : 1;
	unsigned short intelligent : 1;
	unsigned short non_lot : 1;
	unsigned short save_position : 1;
	unsigned short save_hitpoints : 1;
	unsigned short save_flags : 1;
	unsigned short save_anim : 1;
	unsigned short semi_transparent : 1;
	unsigned short water_creature : 1;
	unsigned short using_drawanimating_item : 1;
	unsigned short HitEffect : 2;
	unsigned short undead : 1;
	unsigned short save_mesh : 1;
	void (*draw_routine_extra)(ITEM_INFO* item);
	unsigned long explodable_meshbits;
	unsigned long padfuck;
};

struct FLOOR_INFO
{
	unsigned short index;
	unsigned short fx : 4;
	unsigned short box : 11;
	unsigned short stopper : 1;
	unsigned char pit_room;
	char floor;
	unsigned char sky_room;
	char ceiling;
};

struct LIGHTINFO
{
	long x;
	long y;
	long z;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char Type;
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
	unsigned char Type;
	unsigned char Pad;
};

struct ROOM_INFO
{
	short* data;
	short* door;
	FLOOR_INFO* floor;
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
	unsigned char ReverbType;
	unsigned char FlipNumber;
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
	unsigned short flags;
	long nVerts;
	long nWaterVerts;
	long nShoreVerts;
	LPDIRECT3DVERTEXBUFFER SourceVB;
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

struct STATS
{
	unsigned long Timer;
	unsigned long Distance;
	unsigned long AmmoUsed;
	unsigned long AmmoHits;
	unsigned short Kills;
	unsigned char Secrets;
	unsigned char HealthUsed;
};

struct SAVEGAME_INFO
{
	LARA_INFO Lara;
	long cutscene_triggered;
	unsigned char HubLevels[10];	//saved level indices. highest one that isn't 0 is the one we are currently in
	unsigned short HubOffsets[10];	//offset of each level's data inside the savegame buffer
	unsigned short HubSizes[10];	//size of each level's data inside the savegame buffer
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
	unsigned char HubSavedLara : 1;	//flag that we saved Lara's data when we initialised hub, only set to 1 when InitialiseHub is called with 1
	unsigned char AutoTarget : 1;
	unsigned char HaveBikeBooster : 1;	//have the bike nitro thing
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
	long x = 0;
	long y = 0;
	long z = 0;
	short Xvel = 0;
	short Yvel = 0;
	short Zvel = 0;
	short Gravity = 0;
	short RotAng = 0;
	short Flags = 0;
	unsigned char sSize = 0;
	unsigned char dSize = 0;
	unsigned char Size = 0;
	unsigned char Friction = 0;
	unsigned char Scalar = 0;
	unsigned char Def = 0;
	char RotAdd = 0;
	char MaxYvel = 0;
	unsigned char On = 0;
	unsigned char sR = 0;
	unsigned char sG = 0;
	unsigned char sB = 0;
	unsigned char dR = 0;
	unsigned char dG = 0;
	unsigned char dB = 0;
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char ColFadeSpeed = 0;
	unsigned char FadeToBlack = 0;
	unsigned char sLife = 0;
	unsigned char Life = 0;
	unsigned char TransType = 0;
	unsigned char extras = 0;
	char Dynamic = 0;
	unsigned char FxObj = 0;
	unsigned char RoomNumber = 255;
	unsigned char NodeNumber = 0;

	SPARKS() {}
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
	unsigned long dwRenderWidth;
	unsigned long dwRenderHeight;
	RECT rViewport;
	RECT rScreen;
	long Flags;
	unsigned long WindowStyle;
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
	unsigned char rbpp;
	unsigned char gbpp;
	unsigned char bbpp;
	unsigned char rshift;
	unsigned char gshift;
	unsigned char bshift;
};

struct DXTEXTUREINFO
{
	DDPIXELFORMAT ddpf;
	unsigned long bpp;
	long bPalette;
	long bAlpha;
	unsigned char rbpp;
	unsigned char gbpp;
	unsigned char bbpp;
	unsigned char abpp;
	unsigned char rshift;
	unsigned char gshift;
	unsigned char bshift;
	unsigned char ashift;
};

struct DXZBUFFERINFO
{
	DDPIXELFORMAT ddpf;
	unsigned long bpp;
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
	unsigned short tpage;
	unsigned short offset;
	unsigned short width;
	unsigned short height;
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
	short* gt4;
	short ngt3;
	short* gt3;
	long* prelight;
	LPDIRECT3DVERTEXBUFFER SourceVB;
	D3DVECTOR* Normals;
};

struct TEXTURESTRUCT
{
	unsigned short drawtype = 0;
	unsigned short tpage = 0;
	unsigned short flag = 0;
	float u1 = 0;
	float v1 = 0;
	float u2 = 0;
	float v2 = 0;
	float u3 = 0;
	float v3 = 0;
	float u4 = 0;
	float v4 = 0;

	TEXTURESTRUCT() {}
};

struct LIGHTNING_STRUCT
{
	PHD_VECTOR Point[4];
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char Life;
	char Xvel1;
	char Yvel1;
	char Zvel1;
	char Xvel2;
	char Yvel2;
	char Zvel2;
	char Xvel3;
	char Yvel3;
	char Zvel3;
	unsigned char Size;
	unsigned char Flags;
	unsigned char Rand;
	unsigned char Segments;
	unsigned char Pad[3];
};

struct DYNAMIC
{
	long x = 0;
	long y = 0;
	long z = 0;
	unsigned char on = 0;
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned short falloff = 0;
	unsigned short flags = 0;
	long FalloffScale = 0;

	DYNAMIC() {}
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
	unsigned long meshbits;
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
	unsigned short yrot;
};

struct OBJLIST
{
	short invitem;
	unsigned short yrot;
	unsigned short bright;
};

struct INVDRAWITEM
{
	short xrot;
	short yrot;
	short zrot;
	short object_number;
	unsigned long mesh_bits;
};

enum INVOBJ_TYPES
{
	INV_NOITEM = -1,
	INV_UZI_ITEM = 0,
	INV_PISTOLS_ITEM,
	INV_SHOTGUN_ITEM,
	INV_REVOLVER_ITEM,
	INV_REVOLVER_LASER_ITEM,
	INV_CROSSBOW_ITEM,
	INV_CROSSBOW_LASER_ITEM,
	INV_GRENADEGUN_ITEM,
	INV_SHOTGUN_AMMO1_ITEM,
	INV_SHOTGUN_AMMO2_ITEM,
	INV_GRENADEGUN_AMMO1_ITEM,
	INV_GRENADEGUN_AMMO2_ITEM,
	INV_GRENADEGUN_AMMO3_ITEM,
	INV_CROSSBOW_AMMO1_ITEM,
	INV_CROSSBOW_AMMO2_ITEM,
	INV_CROSSBOW_AMMO3_ITEM,
	INV_REVOLVER_AMMO_ITEM,
	INV_UZI_AMMO_ITEM,
	INV_PISTOLS_AMMO_ITEM,
	INV_M16_ITEM,
	INV_M16_AMMO_ITEM,
	INV_MP5_ITEM,
	INV_MP5_LASER_ITEM,
	INV_MP5_AMMO_ITEM,
	INV_MAGNUMS_ITEM,
	INV_MAGNUMS_AMMO_ITEM,
	INV_AUTOPISTOLS_ITEM,
	INV_AUTOPISTOLS_AMMO_ITEM,
	INV_ROCKET_ITEM,
	INV_ROCKET_AMMO_ITEM,
	INV_HARPOON_ITEM,
	INV_HARPOON_AMMO_ITEM,
	INV_GRAPPLING_ITEM,
	INV_GRAPPLING_AMMO_ITEM,
	INV_DESERTEAGLE_ITEM,
	INV_DESERTEAGLE_AMMO_ITEM,
	INV_LASERSIGHT_ITEM,
	INV_BIGMEDI_ITEM,
	INV_SMALLMEDI_ITEM,
	INV_BINOCULARS_ITEM,
	INV_FLARE_INV_ITEM,
	INV_COMPASS_ITEM,
	INV_MEMCARD_LOAD_ITEM,
	INV_MEMCARD_SAVE_ITEM,
	INV_WATERSKIN1_EMPTY_ITEM,
	INV_WATERSKIN1_1_ITEM,
	INV_WATERSKIN1_2_ITEM,
	INV_WATERSKIN1_3_ITEM,
	INV_WATERSKIN2_EMPTY_ITEM,
	INV_WATERSKIN2_1_ITEM,
	INV_WATERSKIN2_2_ITEM,
	INV_WATERSKIN2_3_ITEM,
	INV_WATERSKIN2_4_ITEM,
	INV_WATERSKIN2_5_ITEM,
	INV_PUZZLE1_ITEM,
	INV_PUZZLE2_ITEM,
	INV_PUZZLE3_ITEM,
	INV_PUZZLE4_ITEM,
	INV_PUZZLE5_ITEM,
	INV_PUZZLE6_ITEM,
	INV_PUZZLE7_ITEM,
	INV_PUZZLE8_ITEM,
	INV_PUZZLE9_ITEM,
	INV_PUZZLE10_ITEM,
	INV_PUZZLE11_ITEM,
	INV_PUZZLE12_ITEM,
	INV_PUZZLE1_COMBO1_ITEM,
	INV_PUZZLE1_COMBO2_ITEM,
	INV_PUZZLE2_COMBO1_ITEM,
	INV_PUZZLE2_COMBO2_ITEM,
	INV_PUZZLE3_COMBO1_ITEM,
	INV_PUZZLE3_COMBO2_ITEM,
	INV_PUZZLE4_COMBO1_ITEM,
	INV_PUZZLE4_COMBO2_ITEM,
	INV_PUZZLE5_COMBO1_ITEM,
	INV_PUZZLE5_COMBO2_ITEM,
	INV_PUZZLE6_COMBO1_ITEM,
	INV_PUZZLE6_COMBO2_ITEM,
	INV_PUZZLE7_COMBO1_ITEM,
	INV_PUZZLE7_COMBO2_ITEM,
	INV_PUZZLE8_COMBO1_ITEM,
	INV_PUZZLE8_COMBO2_ITEM,
	INV_KEY1_ITEM,
	INV_KEY2_ITEM,
	INV_KEY3_ITEM,
	INV_KEY4_ITEM,
	INV_KEY5_ITEM,
	INV_KEY6_ITEM,
	INV_KEY7_ITEM,
	INV_KEY8_ITEM,
	INV_KEY9_ITEM,
	INV_KEY10_ITEM,
	INV_KEY11_ITEM,
	INV_KEY12_ITEM,
	INV_KEY1_COMBO1_ITEM,
	INV_KEY1_COMBO2_ITEM,
	INV_KEY2_COMBO1_ITEM,
	INV_KEY2_COMBO2_ITEM,
	INV_KEY3_COMBO1_ITEM,
	INV_KEY3_COMBO2_ITEM,
	INV_KEY4_COMBO1_ITEM,
	INV_KEY4_COMBO2_ITEM,
	INV_KEY5_COMBO1_ITEM,
	INV_KEY5_COMBO2_ITEM,
	INV_KEY6_COMBO1_ITEM,
	INV_KEY6_COMBO2_ITEM,
	INV_KEY7_COMBO1_ITEM,
	INV_KEY7_COMBO2_ITEM,
	INV_KEY8_COMBO1_ITEM,
	INV_KEY8_COMBO2_ITEM,
	INV_PICKUP1_ITEM,
	INV_PICKUP2_ITEM,
	INV_PICKUP3_ITEM,
	INV_PICKUP4_ITEM,
	INV_PICKUP1_COMBO1_ITEM,
	INV_PICKUP1_COMBO2_ITEM,
	INV_PICKUP2_COMBO1_ITEM,
	INV_PICKUP2_COMBO2_ITEM,
	INV_PICKUP3_COMBO1_ITEM,
	INV_PICKUP3_COMBO2_ITEM,
	INV_PICKUP4_COMBO1_ITEM,
	INV_PICKUP4_COMBO2_ITEM,
	INV_QUEST1_ITEM,
	INV_QUEST2_ITEM,
	INV_QUEST3_ITEM,
	INV_QUEST4_ITEM,
	INV_QUEST5_ITEM,
	INV_QUEST6_ITEM,
	INV_BURNING_TORCH_ITEM,
	INV_CROWBAR_ITEM,
	INV_CLOCKWORK_BEETLE_ITEM,
	INV_MECHANICAL_SCARAB_ITEM,
	INV_WINDING_KEY_ITEM,
	INV_EXAMINE1_ITEM,
	INV_EXAMINE2_ITEM,
	INV_EXAMINE3_ITEM,
	NUM_INVOBJ
};

struct RINGME
{
	OBJLIST current_object_list[NUM_INVOBJ];
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
	unsigned long length;
	unsigned long off;
	unsigned short counter;
	unsigned short data;
	unsigned char decodetype;
	unsigned char packmethod;
	unsigned short padfuck;
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
	unsigned long xlength;
	unsigned long ylength;
	unsigned long zlength;
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
	unsigned char random;
	unsigned char abs;
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

struct BITE_INFO
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
	unsigned long xoff;
	unsigned long yoff;
	unsigned long width;
	unsigned long height;
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
	unsigned long handle;
	unsigned long address;
};

struct DRIP_STRUCT
{
	long x = 0;
	long y = 0;
	long z = 0;
	unsigned char On = 0;
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	short Yvel = 0;
	unsigned char Gravity = 0;
	unsigned char Life = 0;
	short RoomNumber = 255;
	unsigned char Outside = 0;
	unsigned char Pad = 0;

	DRIP_STRUCT() {}
};

struct AI_INFO
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
	unsigned char sequence;
	unsigned char camera;
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
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char pad[3];
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
	unsigned char On;
	unsigned char flags;
};

struct DOORPOS_DATA
{
	FLOOR_INFO* floor;
	FLOOR_INFO data;
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
	unsigned char left;
	unsigned char right;
	unsigned char top;
	unsigned char bottom;
	short height;
	short overlap_index;
};

struct SMOKE_SPARKS
{
	long x = 0;
	long y = 0;
	long z = 0;
	short Xvel = 0;
	short Yvel = 0;
	short Zvel = 0;
	short Gravity = 0;
	short RotAng = 0;
	short Flags = 0;
	unsigned char sSize = 0;
	unsigned char dSize = 0;
	unsigned char Size = 0;
	unsigned char Friction = 0;
	unsigned char Scalar = 0;
	unsigned char Def = 0;
	char RotAdd = 0;
	char MaxYvel = 0;
	unsigned char On = 0;
	unsigned char sShade = 0;
	unsigned char dShade = 0;
	unsigned char Shade = 0;
	unsigned char ColFadeSpeed = 0;
	unsigned char FadeToBlack = 0;
	char sLife = 0;
	char Life = 0;
	unsigned char TransType = 0;
	unsigned char FxObj = 0;
	unsigned char NodeNumber = 0;
	unsigned char mirror = 0;

	SMOKE_SPARKS() {}
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
	unsigned char On;
	unsigned char XRot;
	unsigned char YRot;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char Pad[2];
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
	unsigned short tpage;
	unsigned short offset;
	unsigned short width;
	unsigned short height;
	short x1;
	short y1;
	short x2;
	short y2;
};

struct PHDTEXTURESTRUCT
{
	unsigned short drawtype;
	unsigned short tpage;
	unsigned short flag;
	unsigned short u1;
	unsigned short v1;
	unsigned short u2;
	unsigned short v2;
	unsigned short u3;
	unsigned short v3;
	unsigned short u4;
	unsigned short v4;
	unsigned long xoff;
	unsigned long yoff;
	unsigned long width;
	unsigned long height;
};

struct SAMPLE_INFO
{
	short number;
	unsigned char volume;
	unsigned char radius;
	unsigned char randomness;
	char pitch;
	short flags;
};

struct BUBBLE_STRUCT
{
	PHD_VECTOR pos;
	short room_number = 255;
	short speed = 0;
	short size = 0;
	short dsize = 0;
	unsigned char shade = 0;
	unsigned char vel = 0;
	short pad;
};

struct SHOCKWAVE_STRUCT
{
	long x = 0;
	long y = 0;
	long z = 0;
	short InnerRad = 0;
	short OuterRad = 0;
	short XRot = 0;
	short Flags = 0;
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char life = 0;
	short Speed = 0;
	short Temp = 0;

	SHOCKWAVE_STRUCT() {}
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
	unsigned char life;
};

struct RIPPLE_STRUCT
{
	long x;
	long y;
	long z;
	char flags;
	unsigned char life;
	unsigned char size;
	unsigned char init;
};

struct FIRE_SPARKS
{
	short x = 0;
	short y = 0;
	short z = 0;
	short Xvel = 0;
	short Yvel = 0;
	short Zvel = 0;
	short Gravity = 0;
	short RotAng = 0;
	short Flags = 0;
	unsigned char sSize = 0;
	unsigned char dSize = 0;
	unsigned char Size = 0;
	unsigned char Friction = 0;
	unsigned char Scalar = 0;
	unsigned char Def = 0;
	char RotAdd = 0;
	char MaxYvel = 0;
	unsigned char On = 0;
	unsigned char sR = 0;
	unsigned char sG = 0;
	unsigned char sB = 0;
	unsigned char dR = 0;
	unsigned char dG = 0;
	unsigned char dB = 0;
	unsigned char R = 0;
	unsigned char G = 0;
	unsigned char B = 0;
	unsigned char ColFadeSpeed = 0;
	unsigned char FadeToBlack = 0;
	unsigned char sLife = 0;
	unsigned char Life = 0;

	FIRE_SPARKS() {}
};

struct BLOOD_STRUCT
{
	long x = 0;
	long y = 0;
	long z = 0;
	short Xvel = 0;
	short Yvel = 0;
	short Zvel = 0;
	short Gravity = 0;
	short RotAng = 0;
	unsigned char sSize = 0;
	unsigned char dSize = 0;
	unsigned char Size = 0;
	unsigned char Friction = 0;
	char RotAdd = 0;
	unsigned char On = 0;
	unsigned char sShade = 0;
	unsigned char dShade = 0;
	unsigned char Shade = 0;
	unsigned char ColFadeSpeed = 0;
	unsigned char FadeToBlack = 0;
	char sLife = 0;
	char Life = 0;
	char Pad = 0;

	BLOOD_STRUCT() {}
};

struct WATER_DUST
{
	PHD_VECTOR pos;
	char xvel;
	unsigned char yvel;
	char zvel;
	unsigned char life;
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
	unsigned short nStrings;
	unsigned short nPSXStrings;
	unsigned short nPCStrings;
	unsigned short StringWadLen;
	unsigned short PSXStringWadLen;
	unsigned short PCStringWadLen;
};

struct GUNFLASH_STRUCT
{
	float mx[MATRIX_COUNT]{};
	short on = 0;

	GUNFLASH_STRUCT() {}
};

struct FIRE_LIST
{
	long x = 0;
	long y = 0;
	long z = 0;
	char on = 0;
	char size = 0;
	short room_number = 255;

	FIRE_LIST() {}
};

enum SOUND_STATES
{
	Idle,
	Ending,
	Ended,
};

struct SOUND_SLOT
{
	HCHANNEL channel;
	SOUND_STATES state;
	Vector3 origin;
	float volume;
	float pitch;
	float distance;
	bool loop;
	int sample_index;
};

struct WEAPON_INFO
{
	short lock_angles[4]{ 0, 0, 0, 0 };
	short left_angles[4]{ 0, 0, 0, 0 };
	short right_angles[4]{ 0, 0, 0, 0 };
	short aim_speed = 0;
	short shot_accuracy = 0;
	short gun_height = 0;
	short target_dist = 0;
	short damage = 0;
	short alternate_damage = 0;
	short recoil_frame = 0;
	short flash_time = 0;
	short draw_frame = 0;
	int sample_num;

	WEAPON_INFO() {}

	void setLockAngle(short _0, short _1, short _2, short _3) { lock_angles[0] = _0; lock_angles[1] = _1; lock_angles[2] = _2; lock_angles[3] = _3; }
	void setLeftAngle(short _0, short _1, short _2, short _3) { left_angles[0] = _0; left_angles[1] = _1; left_angles[2] = _2; left_angles[3] = _3; }
	void setRightAngle(short _0, short _1, short _2, short _3) { right_angles[0] = _0; right_angles[1] = _1; right_angles[2] = _2; right_angles[3] = _3; }
	void setAimSpeed(short speed) { aim_speed = speed; }
	void setShotAccuracy(short accuracy) { shot_accuracy = accuracy; }
	void setGunHeight(short height) { gun_height = height; }
	void setTargetDistance(short distance) { target_dist = distance; }
	void setDamage(short _dmg) { damage = _dmg; }
	void setAlternateDamage(short _dmg) { alternate_damage = _dmg; }
	void setRecoilFrame(short frame) { recoil_frame = frame; }
	void setFlashTime(short time) { flash_time = time; }
	void setDrawFrame(short frame) { draw_frame = frame; }
	void setShotSample(int sample) { sample_num = sample; }
};

struct SCARAB_STRUCT
{
	PHD_3DPOS pos;
	short room_number;
	short speed;
	short fallspeed;
	unsigned char On;
	unsigned char flags;
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

struct NODEOFFSET_INFO
{
	short x;
	short y;
	short z;
	char mesh_num;
	unsigned char GotIt;
};

struct TRAIN_STATIC
{
	short type;
	short zoff;
};

struct ROOM_DYNAMIC
{
	float x = 0;
	float y = 0;
	float z = 0;
	float r = 0;
	float g = 0;
	float b = 0;
	float falloff = 0;
	float sqr_falloff = 0;
	float inv_falloff = 0;

	ROOM_DYNAMIC() {}
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
	unsigned char abLeftRed[5];
	unsigned char abLeftGreen[5];
	unsigned char abLeftBlue[5];
	unsigned char abRightRed[5];
	unsigned char abRightGreen[5];
	unsigned char abRightBlue[5];
};

struct COLOR_BIT_MASKS
{
	unsigned long dwRBitMask;
	unsigned long dwGBitMask;
	unsigned long dwBBitMask;
	unsigned long dwRGBAlphaBitMask;
	unsigned long dwRBitDepth;
	unsigned long dwGBitDepth;
	unsigned long dwBBitDepth;
	unsigned long dwRGBAlphaBitDepth;
	unsigned long dwRBitOffset;
	unsigned long dwGBitOffset;
	unsigned long dwBBitOffset;
	unsigned long dwRGBAlphaBitOffset;
};

struct tomb4_options	//keep this at the bottom of the file, please
{
	bool footprints;
	unsigned long shadow_mode;			//1-> original, 2-> circle, 3-> PSX like circle, 4-> PSX sprite, 5-> dynamic
	bool crawltilt;
	bool flexible_crawling;
	bool fix_climb_up_delay;
	bool gameover;
	unsigned long bar_mode;				//1-> original, 2-> TR5, 3-> PSX
	unsigned long bars_pos;				//1-> original, 2-> improved, 3-> PSX
	bool enemy_bars;
	bool cutseq_skipper;
	bool cheats;
	bool loadingtxt;
	unsigned long inv_bg_mode;			//1-> original, 2->TR5, 3-> clear
	bool tr5_loadbar;
	bool look_transparency;
	bool ammo_counter;
	bool ammotype_hotkeys;
	bool combat_cam_tilt;
	bool hpbar_inv;
	bool static_lighting;
	unsigned long reverb;				//1-> off, 2-> Lara room, 3->camera room
	unsigned long distance_fog;			//value in blocks
};
#pragma pack(pop)