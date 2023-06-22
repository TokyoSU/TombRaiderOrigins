#pragma once

enum lara_anim_state
{
	AS_WALK = 0,
	AS_RUN = 1,
	AS_STOP = 2,
	AS_FORWARDJUMP = 3,
	AS_POSE = 4,
	AS_FASTBACK = 5,
	AS_TURN_R = 6,
	AS_TURN_L = 7,
	AS_DEATH = 8,
	AS_FASTFALL = 9,
	AS_HANG = 10,
	AS_REACH = 11,
	AS_SPLAT = 12,
	AS_TREAD = 13,
	AS_LAND = 14,
	AS_COMPRESS = 15,
	AS_BACK = 16,
	AS_SWIM = 17,
	AS_GLIDE = 18,
	AS_NULL = 19,
	AS_FASTTURN = 20,
	AS_STEPRIGHT = 21,
	AS_STEPLEFT = 22,
	AS_HIT = 23,
	AS_SLIDE = 24,
	AS_BACKJUMP = 25,
	AS_RIGHTJUMP = 26,
	AS_LEFTJUMP = 27,
	AS_UPJUMP = 28,
	AS_FALLBACK = 29,
	AS_HANGLEFT = 30,
	AS_HANGRIGHT = 31,
	AS_SLIDEBACK = 32,
	AS_SURFTREAD = 33,
	AS_SURFSWIM = 34,
	AS_DIVE = 35,
	AS_PUSHBLOCK = 36,
	AS_PULLBLOCK = 37,
	AS_PPREADY = 38,
	AS_PICKUP = 39,
	AS_SWITCHON = 40,
	AS_SWITCHOFF = 41,
	AS_USEKEY = 42,
	AS_USEPUZZLE = 43,
	AS_UWDEATH = 44,
	AS_ROLL = 45,
	AS_SPECIAL = 46,
	AS_SURFBACK = 47,
	AS_SURFLEFT = 48,
	AS_SURFRIGHT = 49,
	AS_USEMIDAS = 50,
	AS_DIEMIDAS = 51,
	AS_SWANDIVE = 52,
	AS_FASTDIVE = 53,
	AS_GYMNAST = 54,
	AS_WATEROUT = 55,
	AS_CLIMBSTNC = 56,
	AS_CLIMBING = 57,
	AS_CLIMBLEFT = 58,
	AS_CLIMBEND = 59,
	AS_CLIMBRIGHT = 60,
	AS_CLIMBDOWN = 61,
	AS_LARATEST1 = 62,
	AS_LARATEST2 = 63,
	AS_LARATEST3 = 64,
	AS_WADE = 65,
	AS_WATERROLL = 66,
	AS_FLAREPICKUP = 67,
	AS_TWIST = 68,
	AS_KICK = 69,
	AS_DEATHSLIDE = 70,
	AS_DUCK = 71,
	AS_DUCKROLL = 72,
	AS_DASH = 73,
	AS_DASHDIVE = 74,
	AS_HANG2 = 75,
	AS_MONKEYSWING = 76,
	AS_MONKEYL = 77,
	AS_MONKEYR = 78,
	AS_MONKEY180 = 79,
	AS_ALL4S = 80,
	AS_CRAWL = 81,
	AS_HANGTURNL = 82,
	AS_HANGTURNR = 83,
	AS_ALL4TURNL = 84,
	AS_ALL4TURNR = 85,
	AS_CRAWLBACK = 86,
	AS_HANG2DUCK = 87,
	AS_CRAWL2HANG = 88,
	AS_CONTROLLED = 89,
	AS_ROPELEFT = 90,
	AS_ROPERIGHT = 91,
	AS_BLOCKSWITCH = 92,
	AS_LIFTTRAP = 93,
	AS_PULLTRAP = 94,
	AS_TURNSWITCH = 95,
	AS_COGSWITCH = 96,
	AS_RAILSWITCH = 97,
	AS_HIDDENPICKUP = 98,
	AS_POLESTAT = 99,
	AS_POLEUP = 100,
	AS_POLEDOWN = 101,
	AS_POLELEFT = 102,
	AS_POLERIGHT = 103,
	AS_PULLEY = 104,
	AS_DUCKROTL = 105,
	AS_DUCKROTR = 106,
	AS_CORNEREXTL = 107,
	AS_CORNEREXTR = 108,
	AS_CORNERINTL = 109,
	AS_CORNERINTR = 110,
	AS_ROPE = 111,
	AS_CLIMBROPE = 112,
	AS_SLIDEROPE = 113,
	AS_ROPEFWD = 114,
	AS_ROPEBACK = 115,
	AS_MOVE = 116,
	AS_TWODOOR = 117,

	NUM_LARA_STATES
};

enum lara_anim
{
	ANIM_RUN = 0,
	ANIM_WALK_STOP_LEFT = 2,
	ANIM_WALK_STOP_RIGHT = 3,
	ANIM_STARTRUN = 6,
	ANIM_STOP = 11,
	ANIM_FASTFALL = 23,
	ANIM_LANDFAR = 24,
	ANIM_STOPHANG = 28,
	ANIM_LAND = 31,
	ANIM_FASTSPLAT = 32,
	ANIM_FALLDOWN = 34,
	ANIM_VAULT3 = 42,
	ANIM_VAULT2 = 50,
	ANIM_HITWALLLEFT = 53,
	ANIM_HITWALLRIGHT = 54,
	ANIM_RUNSTEPUP_RIGHT = 55,
	ANIM_RUNSTEPUP_LEFT = 56,
	ANIM_WALKSTEPUP_LEFT = 57,
	ANIM_WALKSTEPUP_RIGHT = 58,
	ANIM_WALKSTEPD_RIGHT = 59,
	ANIM_WALKSTEPD_LEFT = 60,
	ANIM_BACKSTEPD_LEFT = 61,
	ANIM_BACKSTEPD_RIGHT = 62,
	ANIM_SWITCHOFF = 63,
	ANIM_SWITCHON = 64,
	ANIM_SLIDE = 70,
	ANIM_SWIMCHEAT = 87,
	ANIM_FALLBACK = 93,
	ANIM_GRABLOOP = 95,
	ANIM_GRABLEDGE = 96,
	ANIM_BREATH = 103,
	ANIM_SLIDEBACK = 104,
	ANIM_TREAD = 108,
	ANIM_SURFCLIMB = 111,
	ANIM_SURFDIVE = 119,
	ANIM_PPREADY = 120,
	ANIM_PULL = 122,
	ANIM_PUSH = 123,
	ANIM_SPAZ_FORWARD = 125,
	ANIM_SPAZ_BACK = 126,
	ANIM_SPAZ_RIGHT = 127,
	ANIM_SPAZ_LEFT = 128,
	ANIM_PICKUP_UW = 130,
	ANIM_USEKEY = 131,
	ANIM_USEPUZZLE = 134,
	ANIM_PICKUP = 135,
	ANIM_FBLOCK_DEATH = 138,
	ANIM_RBALL_DEATH = 139,
	ANIM_ROLL = 146,
	ANIM_SPIKED = 149,
	ANIM_HANG2STOP = 150,
	ANIM_CLIMBING = 161,
	ANIM_CLIMBSTNC = 164,
	ANIM_CLIMBDOWN = 168,
	ANIM_WADE = 177,
	ANIM_HANGUP = 187,
	ANIM_HANGDOWN = 188,
	ANIM_THROWFLARE = 189,
	ANIM_SURF2WADE1 = 190,
	ANIM_SURF2STND = 191,
	ANIM_SWIM2QSTND = 192,
	ANIM_SURF2QSTND = 193,
	ANIM_GENERATORSW_OFF = 195,
	ANIM_GENERATORSW_ON = 196,
	ANIM_ONEHANDPUSHSW = 197,
	ANIM_GRABRIGHT = 201,
	ANIM_GRABLEFT = 202,
	ANIM_WATERROLL = 203,
	ANIM_PICKUPF = 204,
	ANIM_PICKUPF_UW = 206,
	ANIM_DUCKBREATHE = 222,
	ANIM_UPJUMPGRAB = 233,
	ANIM_MONKEYHANG = 234,
	ANIM_DUCK_TO_ALL4S = 258,
	ANIM_ALL4S = 263,
	ANIM_ALL4S2 = 264,
	ANIM_ALL4TURNL = 269,
	ANIM_ALL4TURNR = 270,
	ANIM_DUCKPICKUP = 291,
	ANIM_SPAZ_DUCKB = 293,
	ANIM_SPAZ_DUCKF = 294,
	ANIM_SPAZ_DUCKR = 295,
	ANIM_SPAZ_DUCKL = 296,
	ANIM_CRAWL_TO_HANG = 302,
	ANIM_DUCKPICKUPF = 312,
	ANIM_PUSHDOOR = 313,
	ANIM_PULLDOOR = 314,
	ANIM_KICKDOOR = 315,
	ANIM_BLOCKSWITCH = 316,
	ANIM_LIFTTRAP = 317,
	ANIM_PULLTRAP = 318,
	ANIM_TURNSWITCHC = 319,
	ANIM_TURNSWITCHA = 320,
	ANIM_COGSWITCHL = 321,
	ANIM_COGSWITCHS = 322,
	ANIM_COGSWITCHE = 323,
	ANIM_RAILSWITCH = 324,
	ANIM_HIDDENPICKUP = 325,
	ANIM_STAT2POLE = 326,
	ANIM_POLESTAT = 328,
	ANIM_REACH2POLE = 331,
	ANIM_POLESLIDEE = 336,
	ANIM_JUMP2POLE = 337,
	ANIM_STAT2PULLEY = 339,
	ANIM_PULLEY = 340,
	ANIM_PULLEY2STAT = 341,
	ANIM_TURNSWITCHCB = 347,
	ANIM_TURNSWITCHCC = 348,
	ANIM_TURNSWITCHCD = 349,
	ANIM_TURNSWITCHAB = 350,
	ANIM_TURNSWITCHAC = 351,
	ANIM_TURNSWITCHAD = 352,
	ANIM_EXTCORNERL_END = 356,
	ANIM_EXTCORNERR_END = 358,
	ANIM_INTCORNERL_END = 360,
	ANIM_INTCORNERR_END = 362,
	ANIM_EXTCLIMBL = 363,
	ANIM_EXTCLIMBL_END = 364,
	ANIM_EXTCLIMBR = 365,
	ANIM_EXTCLIMBR_END = 366,
	ANIM_INTCLIMBL = 367,
	ANIM_INTCLIMBL_END = 368,
	ANIM_INTCLIMBR = 369,
	ANIM_INTCLIMBR_END = 370,
	ANIM_UPJUMP2ROPE = 371,
	ANIM_TRAINDEATH = 372,
	ANIM_SWING2HANG = 373,
	ANIM_ROPESLIDEL = 384,
	ANIM_ROPEKICK = 387,
	ANIM_SWINGFWD = 394,
	ANIM_POURWATERSKIN = 400,
	ANIM_FILLWATERSKIN = 401,
	ANIM_FILLSCALE = 402,
	ANIM_CROWBARDOOR = 403,
	ANIM_TWODOOR = 412,
	ANIM_SMALLSWITCH = 413,
	ANIM_LEAPSWITCH = 414,
	ANIM_WATERSWITCH = 415,
	ANIM_WATERDOOR = 416,
	ANIM_CROWBAR = 419,
	ANIM_CROWSWITCH = 420,
	ANIM_CRAWLJUMP = 421,
	ANIM_PLAYHARP = 422,
	ANIM_PLINTHHI = 424,
	ANIM_PLINTHLO = 425,
	ANIM_THROWSTIX = 426,
	ANIM_LIGHT_TORCH1 = 427,
	ANIM_LIGHT_TORCH2 = 428,
	ANIM_LIGHT_TORCH3 = 429,
	ANIM_LIGHT_TORCH4 = 430,
	ANIM_LIGHT_TORCH5 = 431,
	ANIM_MINEDETECT = 432,
	ANIM_MINEDEATH = 438,
	ANIM_SARCOPHAGUS = 439,
	ANIM_DRAGSAS = 440,
	ANIM_BINOCS = 441,
	ANIM_SETHDEATH = 443,
	ANIM_USEBEETLE = 444,
	NUM_LARA_ANIMS
};
