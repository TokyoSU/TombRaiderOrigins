#include "pch.h"
#include "effect_spark.h"
#include "3dmath.h"
#include "delstuff.h"
#include "draw.h"
#include "control.h"
#include "effect2.h"
#include "effects.h"
#include "lara.h"
#include "tomb4fx.h"
#include "traps.h"
#include "sphere.h"
#include "specificfx.h"
#include "function_stubs.h"

void CEffectSparks::Detatch(long num, long type)
{
	if (m_array.empty()) return;

	for (auto& sptr : m_array)
	{
		if (sptr.On && (sptr.Flags & type) && sptr.FxObj == num)
		{
			if (type == SP_FX)
			{
				auto* fx = &effects[num];
				sptr.x += fx->pos.x_pos;
				sptr.y += fx->pos.y_pos;
				sptr.z += fx->pos.z_pos;
				sptr.Flags &= ~SP_FX;
			}
			else if (type == SP_ITEM)
			{
				auto* item = &items[num];
				sptr.x += item->pos.x_pos;
				sptr.y += item->pos.y_pos;
				sptr.z += item->pos.z_pos;
				sptr.Flags &= ~SP_ITEM;
			}
		}
	}
}

void CEffectSparks::Update()
{
	if (m_array.empty()) return;

	std::array<long, 6> DeadlyBounds{};
	short* bounds;
	long fade, uw, rad, rnd, x, y, z, r, g, b, falloff;

	bounds = GetBoundsAccurate(lara_item);
	DeadlyBounds[0] = lara_item->pos.x_pos + bounds[0];
	DeadlyBounds[1] = lara_item->pos.x_pos + bounds[1];
	DeadlyBounds[2] = lara_item->pos.y_pos + bounds[2];
	DeadlyBounds[3] = lara_item->pos.y_pos + bounds[3];
	DeadlyBounds[4] = lara_item->pos.z_pos + bounds[4];
	DeadlyBounds[5] = lara_item->pos.z_pos + bounds[5];

	for (auto& spark : m_array)
	{
		if (!spark.On)
			continue;

		spark.Life--;
		if (!spark.Life)
		{
			spark.On = 0;
			continue;
		}

		if (spark.sLife - spark.Life < spark.ColFadeSpeed)
		{
			fade = ((spark.sLife - spark.Life) << 16) / spark.ColFadeSpeed;
			spark.R = unsigned char(spark.sR + ((fade * (spark.dR - spark.sR)) >> 16));
			spark.G = unsigned char(spark.sG + ((fade * (spark.dG - spark.sG)) >> 16));
			spark.B = unsigned char(spark.sB + ((fade * (spark.dB - spark.sB)) >> 16));
		}
		else if (spark.Life < spark.FadeToBlack)
		{
			fade = ((spark.Life - spark.FadeToBlack) << 16) / spark.FadeToBlack + 0x10000;
			spark.R = unsigned char((spark.dR * fade) >> 16);
			spark.G = unsigned char((spark.dG * fade) >> 16);
			spark.B = unsigned char((spark.dB * fade) >> 16);

			if (spark.R < 8 && spark.G < 8 && spark.B < 8)
			{
				spark.On = 0;
				continue;
			}
		}
		else
		{
			spark.R = spark.dR;
			spark.G = spark.dG;
			spark.B = spark.dB;
		}

		if (spark.Life == spark.FadeToBlack && spark.Flags & SP_UNDERWEXP)
			spark.dSize >>= 2;

		if (spark.Flags & SP_ROTATE)
			spark.RotAng = (spark.RotAng + spark.RotAdd) & 0xFFF;

		if (spark.sLife - spark.Life == spark.extras >> 3 && spark.extras & 7)
		{
			if (spark.Flags & SP_UNDERWEXP)
				uw = 1;
			else if (spark.Flags & SP_PLASMAEXP)
				uw = 2;
			else
				uw = 0;

			for (int j = 0; j < (spark.extras & 7); j++)
			{
				TriggerExplosionSparks(spark.x, spark.y, spark.z, (spark.extras & 7) - 1, spark.Dynamic, uw, spark.RoomNumber);
				spark.Dynamic = -1;
			}

			if (spark.Flags & SP_UNDERWEXP)
				TriggerExplosionBubble(spark.x, spark.y, spark.z, spark.RoomNumber);

			spark.extras = 0;
		}

		fade = ((spark.sLife - spark.Life) << 16) / spark.sLife;
		spark.Yvel += spark.Gravity;

		if (spark.MaxYvel)
		{
			if (spark.Yvel < 0 && spark.Yvel < spark.MaxYvel << 5 || spark.Yvel > 0 && spark.Yvel > spark.MaxYvel << 5)
				spark.Yvel = spark.MaxYvel << 5;
		}

		if (spark.Friction & 0xF)
		{
			spark.Xvel -= spark.Xvel >> (spark.Friction & 0xF);
			spark.Zvel -= spark.Zvel >> (spark.Friction & 0xF);
		}

		if (spark.Friction & 0xF0)
			spark.Yvel -= spark.Yvel >> (spark.Friction >> 4);

		spark.x += spark.Xvel >> 5;
		spark.y += spark.Yvel >> 5;
		spark.z += spark.Zvel >> 5;

		if (spark.Flags & SP_WIND)
		{
			spark.x += SmokeWindX >> 1;
			spark.z += SmokeWindZ >> 1;
		}

		spark.Size = unsigned char(spark.sSize + ((fade * (spark.dSize - spark.sSize)) >> 16));

		if (spark.Flags & SP_FIRE && (!lara.burn || spark.Flags & SP_USEFXOBJPOS))
		{
			rad = spark.Size << spark.Scalar >> 1;

			if (spark.x + rad > DeadlyBounds[0] && spark.x - rad < DeadlyBounds[1] &&
				spark.y + rad > DeadlyBounds[2] && spark.y - rad < DeadlyBounds[3] &&
				spark.z + rad > DeadlyBounds[4] && spark.z - rad < DeadlyBounds[5])
			{
				if (spark.Flags & SP_FIRE)
					LaraBurn();
				else
					lara_item->hit_points -= 2;
			}
		}
	}

	m_array.erase(
		std::remove_if(
			m_array.begin(),
			m_array.end(),
			[](SPARKS& spr) {
				return spr.On == 0;
			}
		),
		m_array.end()
	);

	for (auto& spark : m_array)
	{
		if (spark.On == 0 || spark.Dynamic == -1)
			continue;

		for (auto& dyn : Dynamics)
		{
			if (dyn.flags & 3)
			{
				rnd = GetRandomControl();
				x = spark.x + 16 * (rnd & 0xF);
				y = spark.y + (rnd & 0xF0);
				z = spark.z + ((rnd >> 4) & 0xF0);
				falloff = spark.sLife - spark.Life - 1;

				if (falloff < 2)
				{
					if (dyn.falloff < 28)
						dyn.falloff += 6;

					r = 255 - (falloff << 3) - (rnd & 0x1F);
					g = 255 - (falloff << 4) - (rnd & 0x1F);
					b = 255 - (falloff << 6) - (rnd & 0x1F);
				}
				else if (falloff < 4)
				{
					if (dyn.falloff < 28)
						dyn.falloff += 6;

					r = 255 - (falloff << 3) - (rnd & 0x1F);
					g = 128 - (falloff << 3);
					b = 128 - (falloff << 5);

					if (b < 0)
						b = 0;
				}
				else
				{
					if (dyn.falloff > 0)
						dyn.falloff--;

					r = 224 + (rnd & 0x1F);
					g = 128 + ((rnd >> 4) & 0x1F);
					b = (rnd >> 8) & 0x3F;
				}

				if (spark.Flags & SP_PLASMAEXP)
					TriggerDynamic(x, y, z, dyn.falloff > 31 ? 31 : dyn.falloff, b, r, g);
				else
					TriggerDynamic(x, y, z, dyn.falloff > 31 ? 31 : dyn.falloff, r, g, b);
			}
		}
	}
}

void CEffectSparks::Draw()
{
	if (m_array.empty()) return;

	static PHD_VECTOR NodeVectors[16];
	FX_INFO* fx;
	ITEM_INFO* item;
	PHD_VECTOR pos;
	long* XY;
	long* Z;
	long* offsets;
	float perspz;
	long x, y, z, smallest_size = 0;

	for (int i = 0; i < 16; i++)
		NodeOffsets[i].GotIt = 0;

	phd_PushMatrix();
	phd_TranslateAbs(lara_item->pos.x_pos, lara_item->pos.y_pos, lara_item->pos.z_pos);
	XY = (long*)&tsv_buffer[0];
	Z = (long*)&tsv_buffer[1024];
	offsets = (long*)&tsv_buffer[2048];

	for (auto& sptr : m_array)
	{
		if (!sptr.On)
			continue;

		if (sptr.Flags & SP_FX)
		{
			fx = &effects[sptr.FxObj];
			x = sptr.x + fx->pos.x_pos;
			y = sptr.y + fx->pos.y_pos;
			z = sptr.z + fx->pos.z_pos;

			if (sptr.sLife - sptr.Life > (GetRandomDraw() & 7) + 4)
			{
				sptr.x = x;
				sptr.y = y;
				sptr.z = z;
				sptr.Flags &= ~SP_FX;
			}
		}
		else if (sptr.Flags & SP_ITEM)
		{
			item = &items[sptr.FxObj];

			if (sptr.Flags & SP_NODEATTACH)
			{
				if (NodeOffsets[sptr.NodeNumber].GotIt)
				{
					pos.x = NodeVectors[sptr.NodeNumber].x;
					pos.y = NodeVectors[sptr.NodeNumber].y;
					pos.z = NodeVectors[sptr.NodeNumber].z;
				}
				else
				{
					pos.x = NodeOffsets[sptr.NodeNumber].x;
					pos.y = NodeOffsets[sptr.NodeNumber].y;
					pos.z = NodeOffsets[sptr.NodeNumber].z;

					if (NodeOffsets[sptr.NodeNumber].mesh_num < 0)
						GetLaraJointPos(&pos, -NodeOffsets[sptr.NodeNumber].mesh_num);
					else
						GetJointAbsPosition(item, &pos, NodeOffsets[sptr.NodeNumber].mesh_num);

					NodeOffsets[sptr.NodeNumber].GotIt = 1;
					NodeVectors[sptr.NodeNumber].x = pos.x;
					NodeVectors[sptr.NodeNumber].y = pos.y;
					NodeVectors[sptr.NodeNumber].z = pos.z;
				}

				x = sptr.x + pos.x;
				y = sptr.y + pos.y;
				z = sptr.z + pos.z;

				if (sptr.sLife - sptr.Life > (GetRandomDraw() & 3) + 8)
				{
					sptr.x = x;
					sptr.y = y;
					sptr.z = z;
					sptr.Flags &= ~SP_NODEATTACH;
				}
			}
			else
			{
				x = sptr.x + item->pos.x_pos;
				y = sptr.y + item->pos.y_pos;
				z = sptr.z + item->pos.z_pos;
			}
		}
		else
		{
			x = sptr.x;
			y = sptr.y;
			z = sptr.z;
		}

		x -= lara_item->pos.x_pos;
		y -= lara_item->pos.y_pos;
		z -= lara_item->pos.z_pos;

		if (x < -0x5000 || x > 0x5000 || y < -0x5000 || y > 0x5000 || z < -0x5000 || z > 0x5000)
		{
			sptr.On = 0;
			continue;
		}

		offsets[0] = x;
		offsets[1] = y;
		offsets[2] = z;
		FVECTOR fPos;
		fPos.x = mMXPtr[M00] * offsets[0] + mMXPtr[M01] * offsets[1] + mMXPtr[M02] * offsets[2] + mMXPtr[M03];
		fPos.y = mMXPtr[M10] * offsets[0] + mMXPtr[M11] * offsets[1] + mMXPtr[M12] * offsets[2] + mMXPtr[M13];
		fPos.z = mMXPtr[M20] * offsets[0] + mMXPtr[M21] * offsets[1] + mMXPtr[M22] * offsets[2] + mMXPtr[M23];
		perspz = f_persp / fPos.z;
		XY[0] = long(fPos.x * perspz + f_centerx);
		XY[1] = long(fPos.y * perspz + f_centery);
		Z[0] = (long)fPos.z;

		if (sptr.Flags & SP_DEF)
		{
			if (sptr.Flags & SP_SCALE)
				smallest_size = 4;
		}
		else
		{
			offsets[0] = x - (sptr.Xvel >> 4);
			offsets[1] = y - (sptr.Yvel >> 4);
			offsets[2] = z - (sptr.Zvel >> 4);
			fPos.x = mMXPtr[M00] * offsets[0] + mMXPtr[M01] * offsets[1] + mMXPtr[M02] * offsets[2] + mMXPtr[M03];
			fPos.y = mMXPtr[M10] * offsets[0] + mMXPtr[M11] * offsets[1] + mMXPtr[M12] * offsets[2] + mMXPtr[M13];
			fPos.z = mMXPtr[M20] * offsets[0] + mMXPtr[M21] * offsets[1] + mMXPtr[M22] * offsets[2] + mMXPtr[M23];
			perspz = f_persp / fPos.z;
			XY[2] = long(fPos.x * perspz + f_centerx);
			XY[3] = long(fPos.y * perspz + f_centery);
			Z[1] = (long)fPos.z;
		}

		S_DrawDrawSparks(sptr, smallest_size, XY, Z);
	}

	phd_PopMatrix();
}
