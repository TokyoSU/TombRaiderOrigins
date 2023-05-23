#pragma once
#include "global/types.h"
#include "effect.h"

enum SPRITE_EFFECTS
{
	SP_FIRE = 0x1,
	SP_SCALE = 0x2,
	SP_BLOOD = 0x4,
	SP_DEF = 0x8,
	SP_ROTATE = 0x10,
	SP_EXPLOSION = 0x20,
	SP_FX = 0x40,
	SP_ITEM = 0x80,
	SP_WIND = 0x100,
	SP_EXPDEF = 0x200,
	SP_USEFXOBJPOS = 0x400,
	SP_UNDERWEXP = 0x800,
	SP_NODEATTACH = 0x1000,
	SP_PLASMAEXP = 0x2000
};

class CEffectSparks : public IEffect<SPARKS>
{
public:
	CEffectSparks() = default;
	~CEffectSparks() = default;
public:
	void Detatch(long num, long type);
	void Update() override;
	void Draw() override;
public:
	bool operator==(const CEffectSparks& other) const
	{
		return false;
	}
};

