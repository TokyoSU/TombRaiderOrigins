#include "pch.h"
#include "snowmobile.h"
#include "control.h"
#include "specific/function_stubs.h"
#include "specific/input.h"
#include "lara.h"
#include "effect2.h"
#include "effects.h"
#include "collide.h"
#include "lara_states.h"
#include "sphere.h"
#include "specific/3dmath.h"
#include "objects.h"
#include "sound.h"
#include "tomb4fx.h"
#include "items.h"
#include "draw.h"
#include "specific/output.h"
#include "laraflar.h"
#include "larafire.h"

enum skidoo_anims
{
	SKID_SIT,
	SKID_GETON,
	SKID_LEFT,
	SKID_RIGHT,
	SKID_FALL,
	SKID_HIT,
	SKID_GETONL,
	SKID_GETOFFL,
	SKID_STILL,
	SKID_GETOFF,
	SKID_LETGO,
	SKID_DEATH,
	SKID_FALLOFF
};

#define SKIDOO_GETON_ANIM 1
#define SKIDOO_GETONL_ANIM 18
#define SKIDOO_FALL_ANIM 8
#define SKIDOO_DEAD_ANIM 15
#define SKIDOO_HIT_LEFT 11
#define SKIDOO_HIT_RIGHT 12
#define SKIDOO_HIT_FRONT 13
#define SKIDOO_HIT_BACK 14
#define SKIDOO_GETOFF_DIST 330
#define SKIDOO_UNDO_TURN (ONE_DEGREE * 2)
#define SKIDOO_TURN ((ONE_DEGREE / 2) + SKIDOO_UNDO_TURN)
#define SKIDOO_MAX_TURN (ONE_DEGREE * 6)
#define SKIDOO_MOMENTUM_TURN (ONE_DEGREE * 3)
#define SKIDOO_MAX_MOM_TURN (ONE_DEGREE * 150)
#define SKIDOO_FAST_SPEED 150
#define SKIDOO_MAX_SPEED 100
#define SKIDOO_SLOW_SPEED 50
#define SKIDOO_MIN_SPEED 15
#define SKIDOO_ACCELERATION	10
#define SKIDOO_BRAKE 5
#define SKIDOO_SLOWDOWN 2
#define SKIDOO_REVERSE 5
#define SKIDOO_MAX_BACK -30
#define SKIDOO_MAX_KICK -80
#define SKIDOO_SLIP 100
#define SKIDOO_SLIP_SIDE 50
#define SKIDOO_FRONT 550
#define SKIDOO_SIDE	260
#define SKIDOO_RADIUS 500
#define SKIDOO_SNOW 500
#define SKIDOO_MAX_HEIGHT 256
#define SKIDOO_MIN_BOUNCE (SKIDOO_MAX_SPEED / 2)
#define SKIDOO_MESHTRACK_INDEX 1

static BITE_INFO left_gun = { 235, -71, 550, 0 };
static BITE_INFO right_gun = { -235, -71, 550, 0 };

static SNOWMOBILEINFO* GetSkidooData(ITEM_INFO* item)
{
	return (SNOWMOBILEINFO*)item->data;
}

void InitialiseSkidoo(short item_number)
{
	auto* skidoo = &items[item_number];
	skidoo->data = game_malloc(sizeof(SNOWMOBILEINFO));

	auto* skinfo = GetSkidooData(skidoo);
	skinfo->skidoo_turn = 0;
	skinfo->left_fallspeed = skinfo->right_fallspeed = 0;
	skinfo->momentum_angle = skidoo->pos.y_rot;
	skinfo->extra_rotation = 0;
	skinfo->track_mesh = 0;
	skinfo->pitch = 0;
	skinfo->armed = skidoo->trigger_flags & 1;
}

int SkidooCheckGetOn(short item_number, COLL_INFO* coll)
{
	if (!(input & IN_ACTION) || lara.gun_status != LG_NO_ARMS || lara_item->gravity_status)
		return 0;

	auto* skidoo = &items[item_number];
	short rot = skidoo->pos.y_rot - lara_item->pos.y_rot;
	short geton = 0;
	if (rot > 0x2000 && rot < 0x6000)
		geton = 1; //right
	else if (rot > -0x6000 && rot < -0x2000)
		geton = 2; //left
	else
		return 0;

	if (!TestBoundsCollide(skidoo, lara_item, coll->radius))
		return 0;
	if (!TestCollision(skidoo, lara_item))
		return 0;

	short room_number = skidoo->room_number;
	auto* floor = GetFloor(skidoo->pos.x_pos, skidoo->pos.y_pos, skidoo->pos.z_pos, &room_number);
	if (GetHeight(floor, skidoo->pos.x_pos, skidoo->pos.y_pos, skidoo->pos.z_pos) < -32000)
		return 0;
	return geton;
}

void SkidooCollision(short item_number, ITEM_INFO* litem, COLL_INFO* coll)
{
	if (litem->hit_points < 0 || lara.vehicle != NO_ITEM)
		return;
	int geton = SkidooCheckGetOn(item_number, coll);
	if (!geton)
	{
		ObjectCollision(item_number, litem, coll);
		return;
	}

	lara.vehicle = item_number;
	if (lara.gun_type == LG_FLARE)
	{
		CreateFlare(FLARE_ITEM, 0);
		undraw_flare_meshes();
		lara.flare_control_left = 0;
		lara.gun_type = LG_NO_ARMS;
		lara.request_gun_type = WEAPON_NONE;
		lara.flare_age = 0;
	}

	if (geton == 1)
		litem->anim_number = objects[SNOWMOBILE_ANIM].anim_index + SKIDOO_GETON_ANIM;
	else
		litem->anim_number = objects[SNOWMOBILE_ANIM].anim_index + SKIDOO_GETONL_ANIM;

	litem->frame_number = anims[litem->anim_number].frame_base;
	litem->current_anim_state = SKID_GETON;
	lara.gun_status = LG_HANDS_BUSY;

	auto* skidoo = &items[item_number];
	litem->pos.y_rot = skidoo->pos.y_rot;
	litem->pos.x_pos = skidoo->pos.x_pos;
	litem->pos.y_pos = skidoo->pos.y_pos;
	litem->pos.z_pos = skidoo->pos.z_pos;
	skidoo->hit_points = 1;
}

void SkidooBaddieCollision(ITEM_INFO* skidoo)
{

}

int SkidooTestHeight(ITEM_INFO* item, int z_off, int x_off, PHD_VECTOR* pos)
{
	pos->y = item->pos.y_pos - (z_off * phd_sin(item->pos.x_rot) >> W2V_SHIFT) +
		                       (x_off * phd_sin(item->pos.z_rot) >> W2V_SHIFT);
	short c = phd_cos(item->pos.y_rot);
	short s = phd_sin(item->pos.y_rot);
	pos->z = item->pos.z_pos + ((z_off * c - x_off * s) >> W2V_SHIFT);
	pos->x = item->pos.x_pos + ((z_off * s + x_off * c) >> W2V_SHIFT);
	short room_number = item->room_number;
	auto* floor = GetFloor(pos->x, pos->y, pos->z, &room_number);
	return GetHeight(floor, pos->x, pos->y, pos->z);
}

int SkidooDoShift(ITEM_INFO* skidoo, PHD_VECTOR* pos, PHD_VECTOR* old)
{
	int x, z, x_old, z_old, shift_x, shift_z;

	x = pos->x >> 10;
	z = pos->z >> 10;
	x_old = old->x >> 10;
	z_old = old->z >> 10;

	shift_x = pos->x & (1024 - 1);
	shift_z = pos->z & (1024 - 1);

	if (x == x_old)
	{
		if (z == z_old)
		{
			skidoo->pos.z_pos += (old->z - pos->z);
			skidoo->pos.x_pos += (old->x - pos->x);
		}
		else if (z > z_old)
		{
			skidoo->pos.z_pos -= shift_z + 1;
			return (pos->x - skidoo->pos.x_pos);
		}
		else
		{
			skidoo->pos.z_pos += 1024 - shift_z;
			return (skidoo->pos.x_pos - pos->x);
		}
	}
	else if (z == z_old)
	{
		if (x > x_old)
		{
			skidoo->pos.x_pos -= shift_x + 1;
			return (skidoo->pos.z_pos - pos->z);
		}
		else
		{
			skidoo->pos.x_pos += 1024 - shift_x;
			return (pos->z - skidoo->pos.z_pos);
		}
	}
	else
	{
		x = 0;
		z = 0;

		short room_number = skidoo->room_number;
		auto* floor = GetFloor(old->x, pos->y, pos->z, &room_number);
		int height = GetHeight(floor, old->x, pos->y, pos->z);
		if (height < old->y - 256)
		{
			if (pos->z > old->z)
				z = -shift_z - 1;
			else
				z = 1024 - shift_z;
		}

		room_number = skidoo->room_number;
		floor = GetFloor(pos->x, pos->y, old->z, &room_number);
		height = GetHeight(floor, pos->x, pos->y, old->z);
		if (height < old->y - 256)
		{
			if (pos->x > old->x)
				x = -shift_x - 1;
			else
				x = 1024 - shift_x;
		}

		if (x && z)
		{
			skidoo->pos.z_pos += z;
			skidoo->pos.x_pos += x;
		}
		else if (z)
		{
			skidoo->pos.z_pos += z;
			if (z > 0)
				return (skidoo->pos.x_pos - pos->x);
			else
				return (pos->x - skidoo->pos.x_pos);
		}
		else if (x)
		{
			skidoo->pos.x_pos += x;
			if (x > 0)
				return (pos->z - skidoo->pos.z_pos);
			else
				return (skidoo->pos.z_pos - pos->z);
		}
		else
		{
			skidoo->pos.z_pos += (old->z - pos->z);
			skidoo->pos.x_pos += (old->x - pos->x);
		}
	}

	return 0;
}

int SkidooDoDynamics(long height, int fallspeed, long* y)
{
	long kick;
	if (height > *y)
	{
		*y += fallspeed;
		if (*y > height - SKIDOO_MIN_BOUNCE)
		{
			*y = height;
			fallspeed = 0;
		}
		else
			fallspeed += 6;
	}
	else
	{
		kick = (height - *y) << 2;
		if (kick < SKIDOO_MAX_KICK)
			kick = SKIDOO_MAX_KICK;
		fallspeed += ((kick - fallspeed) >> 3);
		if (*y > height)
			*y = height;
	}
	return fallspeed;
}

int SkidooGetCollisionAnim(ITEM_INFO* skidoo, PHD_VECTOR* moved)
{
	int c, s, front, side;

	moved->x = skidoo->pos.x_pos - moved->x;
	moved->z = skidoo->pos.z_pos - moved->z;
	if (moved->x || moved->z)
	{
		c = phd_cos(skidoo->pos.y_rot);
		s = phd_sin(skidoo->pos.y_rot);
		front = (moved->z * c + moved->x * s) >> W2V_SHIFT;
		side = (-moved->z * s + moved->x * c) >> W2V_SHIFT;
		if (abs(front) > abs(side))
		{
			if (front > 0)
				return SKIDOO_HIT_BACK;
			else
				return SKIDOO_HIT_FRONT;
		}
		else
		{
			if (side > 0)
				return SKIDOO_HIT_LEFT;
			else
				return SKIDOO_HIT_RIGHT;
		}
	}

	return 0;
}

void SkidooDoSnowEffect(ITEM_INFO* skidoo)
{
}

int SkidooDynamics(ITEM_INFO* skidoo)
{
	PHD_VECTOR moved, fl, fr, br, bl;
	PHD_VECTOR old, fl_old, fr_old, bl_old, br_old;
	int hfl, hfr, hbr, hbl;
	FLOOR_INFO* floor;
	int height, slip, collide;
	short room_number, rot;
	int newspeed;

	auto* skinfo = GetSkidooData(skidoo);
	int hfl_old = SkidooTestHeight(skidoo, SKIDOO_FRONT, -SKIDOO_SIDE, &fl_old);
	int hfr_old = SkidooTestHeight(skidoo, SKIDOO_FRONT, SKIDOO_SIDE, &fr_old);
	int hbl_old = SkidooTestHeight(skidoo, -SKIDOO_FRONT, -SKIDOO_SIDE, &bl_old);
	int hbr_old = SkidooTestHeight(skidoo, -SKIDOO_FRONT, SKIDOO_SIDE, &br_old);
	old.x = skidoo->pos.x_pos;
	old.y = skidoo->pos.y_pos;
	old.z = skidoo->pos.z_pos;

	if (bl_old.y > hbl_old)
		bl_old.y = hbl_old;
	if (br_old.y > hbr_old)
		br_old.y = hbr_old;
	if (fl_old.y > hfl_old)
		fl_old.y = hfl_old;
	if (fr_old.y > hfr_old)
		fr_old.y = hfr_old;

	if (skidoo->pos.y_pos > skidoo->floor - 256)
	{
		if (skinfo->skidoo_turn < -SKIDOO_UNDO_TURN)
			skinfo->skidoo_turn += SKIDOO_UNDO_TURN;
		else if (skinfo->skidoo_turn > SKIDOO_UNDO_TURN)
			skinfo->skidoo_turn -= SKIDOO_UNDO_TURN;
		else
			skinfo->skidoo_turn = 0;
		skidoo->pos.y_rot += skinfo->skidoo_turn + skinfo->extra_rotation;

		rot = skidoo->pos.y_rot - skinfo->momentum_angle;
		if (rot < -SKIDOO_MOMENTUM_TURN)
		{
			if (rot < -SKIDOO_MAX_MOM_TURN)
			{
				rot = -SKIDOO_MAX_MOM_TURN;
				skinfo->momentum_angle = skidoo->pos.y_rot - rot;
			}
			else
				skinfo->momentum_angle -= SKIDOO_MOMENTUM_TURN;
		}
		else if (rot > SKIDOO_MOMENTUM_TURN)
		{
			if (rot > SKIDOO_MAX_MOM_TURN)
			{
				rot = SKIDOO_MAX_MOM_TURN;
				skinfo->momentum_angle = skidoo->pos.y_rot - rot;
			}
			else
				skinfo->momentum_angle += SKIDOO_MOMENTUM_TURN;
		}
		else
			skinfo->momentum_angle = skidoo->pos.y_rot;
	}
	else
		skidoo->pos.y_rot += skinfo->skidoo_turn + skinfo->extra_rotation;

	skidoo->pos.z_pos += skidoo->speed * phd_cos(skinfo->momentum_angle) >> W2V_SHIFT;
	skidoo->pos.x_pos += skidoo->speed * phd_sin(skinfo->momentum_angle) >> W2V_SHIFT;

	slip = SKIDOO_SLIP * phd_sin(skidoo->pos.x_rot) >> W2V_SHIFT;
	if (abs(slip) > SKIDOO_SLIP / 2)
	{
		skidoo->pos.z_pos -= slip * phd_cos(skidoo->pos.y_rot) >> W2V_SHIFT;
		skidoo->pos.x_pos -= slip * phd_sin(skidoo->pos.y_rot) >> W2V_SHIFT;
	}

	slip = SKIDOO_SLIP_SIDE * phd_sin(skidoo->pos.z_rot) >> W2V_SHIFT;
	if (abs(slip) > SKIDOO_SLIP_SIDE / 2)
	{
		skidoo->pos.z_pos -= slip * phd_sin(skidoo->pos.y_rot) >> W2V_SHIFT;
		skidoo->pos.x_pos += slip * phd_cos(skidoo->pos.y_rot) >> W2V_SHIFT;
	}

	moved.x = skidoo->pos.x_pos;
	moved.z = skidoo->pos.z_pos;
	if (!(skidoo->flags & IFL_SWITCH_ONESHOT))
		SkidooBaddieCollision(skidoo);

	rot = 0;
	hbl = SkidooTestHeight(skidoo, -SKIDOO_FRONT, -SKIDOO_SIDE, &bl);
	if (hbl < bl_old.y - 256)
		rot = SkidooDoShift(skidoo, &bl, &bl_old);

	hbr = SkidooTestHeight(skidoo, -SKIDOO_FRONT, SKIDOO_SIDE, &br);
	if (hbr < br_old.y - 256)
		rot += SkidooDoShift(skidoo, &br, &br_old);

	hfl = SkidooTestHeight(skidoo, SKIDOO_FRONT, -SKIDOO_SIDE, &fl);
	if (hfl < fl_old.y - 256)
		rot += SkidooDoShift(skidoo, &fl, &fl_old);

	hfr = SkidooTestHeight(skidoo, SKIDOO_FRONT, SKIDOO_SIDE, &fr);
	if (hfr < fr_old.y - 256)
		rot += SkidooDoShift(skidoo, &fr, &fr_old);

	room_number = skidoo->room_number;
	floor = GetFloor(skidoo->pos.x_pos, skidoo->pos.y_pos, skidoo->pos.z_pos, &room_number);
	height = GetHeight(floor, skidoo->pos.x_pos, skidoo->pos.y_pos, skidoo->pos.z_pos);
	if (height < skidoo->pos.y_pos - 256)
		SkidooDoShift(skidoo, (PHD_VECTOR*)&skidoo->pos, &old);

	skinfo->extra_rotation = rot;

	collide = SkidooGetCollisionAnim(skidoo, &moved);
	if (collide)
	{
		newspeed = ((skidoo->pos.z_pos - old.z) * phd_cos(skinfo->momentum_angle) + (skidoo->pos.x_pos - old.x) * phd_sin(skinfo->momentum_angle)) >> W2V_SHIFT;
		auto* check_skidoo_item = &items[lara.vehicle];
		if (check_skidoo_item == skidoo && skidoo->speed > SKIDOO_MAX_SPEED + SKIDOO_ACCELERATION && newspeed < skidoo->speed - 10)
		{
			lara_item->hit_points -= (skidoo->speed - newspeed) >> 1;
			lara_item->hit_status = 1;
		}

		if (skidoo->speed > 0 && newspeed < skidoo->speed)
			skidoo->speed = (newspeed < 0) ? 0 : newspeed;
		else if (skidoo->speed < 0 && newspeed > skidoo->speed)
			skidoo->speed = (newspeed > 0) ? 0 : newspeed;

		if (skidoo->speed < SKIDOO_MAX_BACK)
			skidoo->speed = SKIDOO_MAX_BACK;
	}

	return collide;
}

int SkidooUserControl(ITEM_INFO* skidoo, int height, int* pitch)
{
	int drive = 0, max_speed;
	auto* skinfo = GetSkidooData(skidoo);

	if (skidoo->pos.y_pos >= height - 256)
	{
		*pitch = skidoo->speed + (height - skidoo->pos.y_pos);

		if (skidoo->speed == 0 && (input & IN_LOOK))
			LookUpDown();

		if (((input & IN_LEFT) && !(input & IN_BACK)) || ((input & IN_RIGHT) && (input & IN_BACK)))
		{
			skinfo->skidoo_turn -= SKIDOO_TURN;
			if (skinfo->skidoo_turn < -SKIDOO_MAX_TURN)
				skinfo->skidoo_turn = -SKIDOO_MAX_TURN;
		}

		if (((input & IN_RIGHT) && !(input & IN_BACK)) || ((input & IN_LEFT) && (input & IN_BACK)))
		{
			skinfo->skidoo_turn += SKIDOO_TURN;
			if (skinfo->skidoo_turn > SKIDOO_MAX_TURN)
				skinfo->skidoo_turn = SKIDOO_MAX_TURN;
		}

		if (input & IN_BACK)
		{
			if (skidoo->speed > 0)
				skidoo->speed -= SKIDOO_BRAKE;
			else
			{
				if (skidoo->speed > SKIDOO_MAX_BACK)
					skidoo->speed -= SKIDOO_REVERSE;
				drive = 1;
			}
		}
		else if (input & IN_FORWARD)
		{
			if ((input & IN_ACTION) && !skinfo->armed) // red skidoo can go faster than bandit ones
				max_speed = SKIDOO_FAST_SPEED;
			else if (input & IN_WALK)
				max_speed = SKIDOO_SLOW_SPEED;
			else
				max_speed = SKIDOO_MAX_SPEED;

			if (skidoo->speed < max_speed)
				skidoo->speed += SKIDOO_ACCELERATION / 2 + SKIDOO_ACCELERATION * skidoo->speed / (2 * max_speed);
			else if (skidoo->speed > max_speed + SKIDOO_SLOWDOWN)
				skidoo->speed -= SKIDOO_SLOWDOWN;
			drive = 1;
		}
		else if (skidoo->speed >= 0 && skidoo->speed < SKIDOO_MIN_SPEED && (input & (IN_LEFT | IN_RIGHT)))
		{
			skidoo->speed = SKIDOO_MIN_SPEED; // If user wants to turn, skidoo will move forward
			drive = 1;
		}
		else if (skidoo->speed > SKIDOO_SLOWDOWN)
		{
			skidoo->speed -= SKIDOO_SLOWDOWN;
			if ((GetRandomDraw() & 0x7f) < skidoo->speed)
				drive = 1;
		}
		else
			skidoo->speed = 0;
	}
	else if (input & (IN_FORWARD | IN_BACK))
	{
		drive = 1;
		*pitch = skinfo->pitch + 50;
	}

	return drive;
}

int SkidooCheckGetOffOK(int direction)
{
	auto* skidoo = &items[lara.vehicle];
	short angle = skidoo->pos.y_rot;
	if (direction == SKID_GETOFFL)
		angle += 0x4000;
	else
		angle -= 0x4000;

	int x = skidoo->pos.x_pos - (SKIDOO_GETOFF_DIST * phd_sin(angle) >> W2V_SHIFT);
	int y = skidoo->pos.y_pos;
	int z = skidoo->pos.z_pos - (SKIDOO_GETOFF_DIST * phd_cos(angle) >> W2V_SHIFT);

	short room_number = skidoo->room_number;
	auto* floor = GetFloor(x, y, z, &room_number);
	int height = GetHeight(floor, x, y, z);
	
	if (height_type == BIG_SLOPE || height == NO_HEIGHT || height_type == DIAGONAL)
		return 0;
	if (abs(height - skidoo->pos.y_pos) > 512)
		return 0;

	int ceiling = GetCeiling(floor, x, y, z);
	if (ceiling - skidoo->pos.y_pos > -762 || height - ceiling < 762)
		return 0;
	return 1;
}

void SkidooAnimation(ITEM_INFO* skidoo, int collide, int dead)
{
	auto* skinfo = GetSkidooData(skidoo);

	if (skidoo->pos.y_pos != skidoo->floor && skidoo->fallspeed > 0 && lara_item->current_anim_state != SKID_FALL && !dead)
	{
		lara_item->anim_number = objects[SNOWMOBILE_ANIM].anim_index + SKIDOO_FALL_ANIM;
		lara_item->frame_number = anims[lara_item->anim_number].frame_base;
		lara_item->current_anim_state = lara_item->goal_anim_state = SKID_FALL;
	}
	else if (collide && !dead && lara_item->current_anim_state != SKID_FALL)
	{
		if (lara_item->current_anim_state != SKID_HIT)
		{
			if (collide == SKIDOO_HIT_FRONT)
				Sound.PlayEffect(SFX_BIKE_FRONT_IMPACT, &skidoo->pos);
			else
				Sound.PlayEffect(SFX_BIKE_SIDE_IMPACT, &skidoo->pos);
			lara_item->anim_number = objects[SNOWMOBILE_ANIM].anim_index + collide;
			lara_item->frame_number = anims[lara_item->anim_number].frame_base;
			lara_item->current_anim_state = lara_item->goal_anim_state = SKID_HIT;
		}
	}
	else
	{
		switch (lara_item->current_anim_state)
		{
		case SKID_STILL:
			if (dead)
			{
				lara_item->goal_anim_state = SKID_DEATH;
				break;
			}
			lara_item->goal_anim_state = SKID_STILL;

			if (input & IN_JUMP)
			{
				if ((input & IN_RIGHT) && SkidooCheckGetOffOK(SKID_GETOFF))
				{
					lara_item->goal_anim_state = SKID_GETOFF;
					skidoo->speed = 0;
				}
				else if ((input & IN_LEFT) && SkidooCheckGetOffOK(SKID_GETOFFL))
				{
					lara_item->goal_anim_state = SKID_GETOFFL;
					skidoo->speed = 0;
				}
			}
			else if (input & IN_LEFT)
				lara_item->goal_anim_state = SKID_LEFT;
			else if (input & IN_RIGHT)
				lara_item->goal_anim_state = SKID_RIGHT;
			else if (input & (IN_FORWARD | IN_BACK))
				lara_item->goal_anim_state = SKID_SIT;
			break;

		case SKID_SIT:
			if (skidoo->speed == 0)
				lara_item->goal_anim_state = SKID_STILL;

			if (dead)
				lara_item->goal_anim_state = SKID_FALLOFF;
			else if (input & IN_LEFT)
				lara_item->goal_anim_state = SKID_LEFT;
			else if (input & IN_RIGHT)
				lara_item->goal_anim_state = SKID_RIGHT;
			break;

		case SKID_LEFT:
			if (!(input & IN_LEFT))
				lara_item->goal_anim_state = SKID_SIT;
			break;

		case SKID_RIGHT:
			if (!(input & IN_RIGHT))
				lara_item->goal_anim_state = SKID_SIT;
			break;

		case SKID_FALL:
			if (skidoo->fallspeed <= 0 || skinfo->left_fallspeed <= 0 || skinfo->right_fallspeed <= 0)
			{
				Sound.PlayEffect(SFX_SNOWMOBILE_MOVING, &skidoo->pos);
				lara_item->goal_anim_state = SKID_SIT;
			}
			else if (skidoo->fallspeed > DAMAGE_START + DAMAGE_LENGTH)
				lara_item->goal_anim_state = SKID_LETGO;
			break;
		}
	}
}

void SkidooExplode(ITEM_INFO* skidoo)
{
	auto* skinfo = GetSkidooData(skidoo);

	TriggerExplosionSparks(skidoo->pos.x_pos, skidoo->pos.y_pos - 768, skidoo->pos.z_pos, 3, -2, 0, skidoo->room_number);
	for (int i = 0; i < 2; i++)
		TriggerExplosionSparks(skidoo->pos.x_pos, skidoo->pos.y_pos - 768, skidoo->pos.z_pos, 3, -1, 0, skidoo->room_number);

	Sound.PlayEffect(SFX_EXPLOSION1, &skidoo->pos);
	Sound.PlayEffect(SFX_EXPLOSION2, &skidoo->pos);

	if (skinfo != NULL)
		ExplodingDeath2(skidoo->index, -1, 256, skinfo->armed ? SNOWMOBILE_GUN_SKIN : SNOWMOBILE);
	else
		ExplodingDeath2(skidoo->index, -1, 256);

	KillItem(lara.vehicle);
	lara.vehicle = NO_ITEM;
}

int SkidooCheckGetOff()
{
	auto* skidoo = &items[lara.vehicle];

	if ((lara_item->current_anim_state == SKID_GETOFF || lara_item->current_anim_state == SKID_GETOFFL) &&
		 lara_item->frame_number == anims[lara_item->anim_number].frame_end)
	{
		if (lara_item->current_anim_state == SKID_GETOFFL)
			lara_item->pos.y_rot += 0x4000;
		else
			lara_item->pos.y_rot -= 0x4000;

		lara_item->anim_number = ANIM_STOP;
		lara_item->frame_number = anims[lara_item->anim_number].frame_base;
		lara_item->current_anim_state = lara_item->goal_anim_state = AS_STOP;
		lara_item->pos.x_pos -= SKIDOO_GETOFF_DIST * phd_sin(lara_item->pos.y_rot) >> W2V_SHIFT;
		lara_item->pos.z_pos -= SKIDOO_GETOFF_DIST * phd_cos(lara_item->pos.y_rot) >> W2V_SHIFT;
		lara_item->pos.x_rot = lara_item->pos.z_rot = 0;
		lara.vehicle = NO_ITEM;
		lara.gun_status = LG_NO_ARMS;
	}
	else if (lara_item->current_anim_state == SKID_LETGO && (skidoo->pos.y_pos == skidoo->floor || lara_item->frame_number == anims[lara_item->anim_number].frame_end))
	{
		lara_item->anim_number = ANIM_FASTFALL;
		lara_item->frame_number = anims[lara_item->anim_number].frame_base;
		lara_item->current_anim_state = AS_FASTFALL;

		if (skidoo->pos.y_pos == skidoo->floor)
		{
			lara_item->goal_anim_state = AS_DEATH;
			lara_item->fallspeed = DAMAGE_START + DAMAGE_LENGTH;
			lara_item->speed = 0;
			SkidooExplode(skidoo);
		}
		else
		{
			lara_item->goal_anim_state = AS_FASTFALL;
			lara_item->pos.y_pos -= 200;
			lara_item->fallspeed = skidoo->fallspeed;
			lara_item->speed = skidoo->speed;
			Sound.PlayEffect(SFX_LARA_FALL, &lara_item->pos);
		}

		lara_item->pos.x_rot = lara_item->pos.z_rot = 0;
		lara_item->gravity_status = 1;
		lara.gun_status = LG_NO_ARMS;
		lara.move_angle = skidoo->pos.y_rot;
		skidoo->flags |= IFL_SWITCH_ONESHOT;
		skidoo->collidable = FALSE;
		return 0;
	}

	return 1;
}

void SkidooGuns()
{
	auto* winfo = &weapons[WEAPON_SNOWMOBILEGUN];
	LaraGetNewTarget(winfo);
	AimWeapon(winfo, &lara.right_arm);
	auto* skidoo = &items[lara.vehicle];

	if (input & IN_ACTION && skidoo->item_flags[3] == 0)
	{
		short angles[2]
		{
			lara.right_arm.y_rot + lara_item->pos.y_rot,
			lara.right_arm.x_rot
		};

		if (FireWeapon(WEAPON_SNOWMOBILEGUN, lara.target, lara_item, angles))
		{
			Sound.PlayEffect(winfo->sample_num, &lara_item->pos);
			TriggerDynamic(
				lara_item->pos.x_pos + (phd_sin(lara_item->pos.y_rot) >> (W2V_SHIFT - 10)),
				lara_item->pos.y_pos - 512,
				lara_item->pos.z_pos + (phd_cos(lara_item->pos.y_rot) >> (W2V_SHIFT - 10)),
				12,
				255, 128, 32
			);
			skidoo->fired_weapon = 1;
			skidoo->item_flags[3] = 2;
		}
	}
}

void SnowmobileStart(ITEM_INFO* skidoo, ITEM_INFO* laraitem)
{
	auto* skinfo = GetSkidooData(skidoo);
	lara.gun_status = LG_HANDS_BUSY;
	lara.hit_direction = -1;
	laraitem->current_anim_state = 0;
	laraitem->goal_anim_state = 0;
	laraitem->anim_number = objects[SNOWMOBILE_ANIM].anim_index;
	laraitem->frame_number = anims[laraitem->anim_number].frame_base;
	skidoo->anim_number = laraitem->anim_number + objects[SNOWMOBILE].anim_index - objects[SNOWMOBILE_ANIM].anim_index;
	skidoo->frame_number = laraitem->frame_number + anims[skidoo->anim_number].frame_base - anims[laraitem->anim_number].frame_base;
	skidoo->flags |= IFL_TRIGGERED;
	skidoo->hit_points = 1;
}

void SkidooControl(short item_number)
{
	PHD_VECTOR fl, fr;
	int drive;
	short x_rot, z_rot;
	int dead = 0;

	auto* skidoo = &items[item_number];
	if (skidoo->item_flags[3] != 0)
		skidoo->item_flags[3]--;

	auto* skinfo = GetSkidooData(skidoo);
	int collide = SkidooDynamics(skidoo);

	int hfl = SkidooTestHeight(skidoo, SKIDOO_FRONT, -SKIDOO_SIDE, &fl);
	int hfr = SkidooTestHeight(skidoo, SKIDOO_FRONT, SKIDOO_SIDE, &fr);

	short room_number = skidoo->room_number;
	auto* floor = GetFloor(skidoo->pos.x_pos, skidoo->pos.y_pos, skidoo->pos.z_pos, &room_number);
	int height = GetHeight(floor, skidoo->pos.x_pos, skidoo->pos.y_pos, skidoo->pos.z_pos);
	TestTriggers(trigger_index, 0, FALSE);

	if (lara_item->hit_points <= 0)
	{
		input &= ~(IN_LEFT | IN_RIGHT | IN_BACK | IN_FORWARD);
		dead = 1;
	}
	else if (lara_item->current_anim_state == SKID_LETGO)
	{
		dead = 1;
		collide = 0;
	}

	int pitch = 0;
	if (skidoo->flags & IFL_SWITCH_ONESHOT)
	{
		drive = 0;
		collide = 0;
	}
	else
	{
		switch (lara_item->current_anim_state)
		{
		case SKID_GETON:
		case SKID_GETOFF:
		case SKID_GETOFFL:
		case SKID_LETGO:
			drive = -1;
			collide = 0;
			break;

		default:
			drive = SkidooUserControl(skidoo, height, &pitch);
			break;
		}
	}

	if (drive > 0)
	{
		skinfo->track_mesh = skinfo->track_mesh == 1 ? 2 : 1;
		skinfo->pitch += (pitch - skinfo->pitch) / 4;
		auto pitch = std::clamp(0.5f + (float)abs(skinfo->pitch) / (float)SKIDOO_MAX_SPEED, 0.6f, 1.2f);
		Sound.PlayEffect(skinfo->pitch ? SFX_SNOWMOBILE_MOVING : SFX_SNOWMOBILE_ACCEL, &skidoo->pos, SFXO_ALWAYS, pitch);
	}
	else
	{
		skinfo->track_mesh = 0;
		if (!drive)
			Sound.PlayEffect(SFX_SNOWMOBILE_IDLE, &skidoo->pos);
		skinfo->pitch = 0;
	}

	skidoo->floor = height;
	skinfo->left_fallspeed = SkidooDoDynamics(hfl, skinfo->left_fallspeed, &fl.y);
	skinfo->right_fallspeed = SkidooDoDynamics(hfr, skinfo->right_fallspeed, &fr.y);
	skidoo->fallspeed = SkidooDoDynamics(height, skidoo->fallspeed, &skidoo->pos.y_pos);

	height = (fl.y + fr.y) >> 1;
	x_rot = (short)phd_atan(SKIDOO_FRONT, skidoo->pos.y_pos - height);
	z_rot = (short)phd_atan(SKIDOO_SIDE, height - fl.y);

	skidoo->pos.x_rot += (x_rot - skidoo->pos.x_rot) >> 1;
	skidoo->pos.z_rot += (z_rot - skidoo->pos.z_rot) >> 1;

	if (skidoo->flags & IFL_SWITCH_ONESHOT)
	{
		if (room_number != skidoo->room_number)
		{
			ItemNewRoom(lara.vehicle, room_number);
			ItemNewRoom(lara.item_number, room_number);
		}
		if (skidoo->pos.y_pos == skidoo->floor)
			SkidooExplode(skidoo);
		return;
	}

	SkidooAnimation(skidoo, collide, dead);

	if (room_number != skidoo->room_number)
	{
		ItemNewRoom(lara.vehicle, room_number);
		ItemNewRoom(lara.item_number, room_number);
	}

	if (lara_item->current_anim_state != SKID_FALLOFF)
	{
		lara_item->pos.x_pos = skidoo->pos.x_pos;
		lara_item->pos.y_pos = skidoo->pos.y_pos;
		lara_item->pos.z_pos = skidoo->pos.z_pos;
		lara_item->pos.y_rot = skidoo->pos.y_rot;
		if (drive >= 0)
		{
			lara_item->pos.x_rot = skidoo->pos.x_rot;
			lara_item->pos.z_rot = skidoo->pos.z_rot;
		}
		else
			lara_item->pos.x_rot = lara_item->pos.z_rot = 0;
	}
	else
		lara_item->pos.x_rot = lara_item->pos.z_rot = 0;

	AnimateItem(lara_item);

	if (!dead && drive >= 0 && skinfo->armed)
		SkidooGuns();

	if (!dead)
	{
		skidoo->anim_number = objects[SNOWMOBILE].anim_index + (lara_item->anim_number - objects[SNOWMOBILE_ANIM].anim_index);
		skidoo->frame_number = anims[skidoo->anim_number].frame_base + (lara_item->frame_number - anims[lara_item->anim_number].frame_base);
	}
	else
	{
		skidoo->anim_number = objects[SNOWMOBILE].anim_index + SKIDOO_DEAD_ANIM;
		skidoo->frame_number = anims[skidoo->anim_number].frame_base;
	}

	if (skidoo->speed && skidoo->floor == skidoo->pos.y_pos)
	{
		SkidooDoSnowEffect(skidoo);
		if (skidoo->speed < 50)
			SkidooDoSnowEffect(skidoo);
	}

	SkidooCheckGetOff();
}

static void DrawMesh(ITEM_INFO* item, int clip, int mesh_bits, short** meshpp, bool isInterpolate)
{
	if (isInterpolate)
	{
		if (mesh_bits & item->meshswap_meshbits)
			phd_PutPolygons_I(meshpp[1], clip);
		else
			phd_PutPolygons_I(meshpp[0], clip);
	}
	else
	{
		if (mesh_bits & item->meshswap_meshbits)
			phd_PutPolygons(meshpp[1], clip);
		else
			phd_PutPolygons(meshpp[0], clip);
	}
}

static void DrawGunflashMesh(ITEM_INFO* item, BITE_INFO* bite, int clip, bool isInterpolate)
{
	if (item->fired_weapon != 0)
	{
		int random = GetRandomDraw();
		if (isInterpolate)
		{
			phd_PushMatrix_I();
			phd_TranslateRel_I(bite->x, bite->y, bite->z);
			phd_RotYXZ_I(0, 0, short((random << W2V_SHIFT) + (random >> 2) - 4096));
			mInterpolateMatrix();
			phd_PutPolygons(meshes[objects[GUN_FLASH2].mesh_index], clip);
			phd_PopMatrix_I();
		}
		else
		{
			phd_PushMatrix();
			phd_TranslateRel(bite->x, bite->y, bite->z);
			phd_RotYXZ(0, 0, short((random << W2V_SHIFT) + (random >> 2) - 4096));
			phd_PutPolygons(meshes[objects[GUN_FLASH2].mesh_index], clip);
			phd_PopMatrix();
		}
	}
}

void DrawSkidoo(ITEM_INFO* item)
{
	OBJECT_INFO* obj;
	short** meshpp, **track;
	long* bone;
	short* frm[2];
	short* rot, *rot2;
	long frac, rate, clip, bit;

	auto* skinfo = GetSkidooData(item);
	frac = GetFrames(item, frm, &rate);

	// only change skin if the object is loaded !
	if (skinfo != NULL && skinfo->armed && objects[SNOWMOBILE_GUN_SKIN].loaded)
		obj = &objects[SNOWMOBILE_GUN_SKIN];
	else
		obj = &objects[item->object_number];

	phd_PushMatrix();
	phd_TranslateAbs(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
	phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);

	if (item->object_number < ENEMY_JEEP || item->object_number > SETHA)
	{
		calc_animating_item_clip_window(item, frm[0]);
	}
	else
	{
		phd_left = 0;
		phd_right = phd_winwidth;
		phd_top = 0;
		phd_bottom = phd_winheight;
	}

	clip = S_GetObjectBounds(frm[0]);
	if (clip)
	{
		CalculateObjectLighting(item, frm[0]);
		bit = 1;
		meshpp = &meshes[obj->mesh_index];
		bone = &bones[obj->bone_index];

		track = NULL;
		if (skinfo != NULL && skinfo->track_mesh == 1)
			track = &meshes[objects[SNOWMOBILE_TRACK].mesh_index + 1 * 2];
		else if (skinfo != NULL && skinfo->track_mesh == 2)
			track = &meshes[objects[SNOWMOBILE_TRACK].mesh_index + 7 * 2];

		if (frac != 0)
		{
			InitInterpolate(frac, rate);
			phd_TranslateRel_ID(frm[0][6], frm[0][7], frm[0][8], frm[1][6], frm[1][7], frm[1][8]);
			rot = frm[0] + 9;
			rot2 = frm[1] + 9;
			gar_RotYXZsuperpack_I(&rot, &rot2, 0);
			if (item->mesh_bits & 1)
				DrawMesh(item, clip, 1, meshpp, false);
			DrawGunflashMesh(item, &left_gun, clip, true);
			DrawGunflashMesh(item, &right_gun, clip, true);

			meshpp += 2;
			for (int i = 0; i < obj->nmeshes - 1; i++, bone += 4, meshpp += 2)
			{
				if (bone[0] & 1)
					phd_PopMatrix_I();
				if (bone[0] & 2)
					phd_PushMatrix_I();

				phd_TranslateRel_I(bone[1], bone[2], bone[3]);
				gar_RotYXZsuperpack_I(&rot, &rot2, 0);

				bit <<= 1;
				if (bit & item->mesh_bits)
				{
					if (i == (SKIDOO_MESHTRACK_INDEX-1)) // track mesh.
					{
						if (track)
							DrawMesh(item, clip, bit, track, true);
						else
							DrawMesh(item, clip, bit, meshpp, true);
					}
					else
					{
						DrawMesh(item, clip, bit, meshpp, true);
					}
				}
			}
		}
		else
		{
			phd_TranslateRel(frm[0][6], frm[0][7], frm[0][8]);
			rot = frm[0] + 9;
			gar_RotYXZsuperpack(&rot, 0);
			if (item->mesh_bits & 1)
				DrawMesh(item, clip, 1, meshpp, false);
			DrawGunflashMesh(item, &left_gun, clip, false);
			DrawGunflashMesh(item, &right_gun, clip, false);

			meshpp += 2;
			for (int i = 0; i < obj->nmeshes - 1; i++, bone += 4, meshpp += 2)
			{
				if (bone[0] & 1)
					phd_PopMatrix();
				if (bone[0] & 2)
					phd_PushMatrix();

				phd_TranslateRel(bone[1], bone[2], bone[3]);
				gar_RotYXZsuperpack(&rot, 0);

				bit <<= 1;
				if (bit & item->mesh_bits)
				{
					if (i == (SKIDOO_MESHTRACK_INDEX-1)) // track mesh.
					{
						if (track)
							DrawMesh(item, clip, bit, track, false);
						else
							DrawMesh(item, clip, bit, meshpp, false);
					}
					else
					{
						DrawMesh(item, clip, bit, meshpp, false);
					}
				}
			}
		}
	}

	if (item->fired_weapon != 0)
		item->fired_weapon--;

	phd_left = 0;
	phd_top = 0;
	phd_right = phd_winwidth;
	phd_bottom = phd_winheight;
	phd_PopMatrix();
}

void DrawSkidooExtras(ITEM_INFO* item)
{

}
