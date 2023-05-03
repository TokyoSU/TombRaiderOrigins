#include "pch.h"
#include "sound.h"
#include "../game/lara.h"
#include "../specific/3dmath.h"
#include "../specific/dxsound.h"
#include "../specific/function_stubs.h"
#include "camera.h"
#include "gameflow.h"
#include "control.h"
#include "../specific/LoadSave.h"
#include "../specific/d3dmatrix.h"
#include <algorithm>

constexpr auto SOUND_MIN_PARAM_MULTIPLIER = 0.05f;
constexpr auto SOUND_MAX_PARAM_MULTIPLIER = 5.0f;

SAMPLE_INFO* sample_infos;
SOUND_SLOT LaSlot[SOUND_MAX_CHANNELS];
short* samples_maps;
long sound_active = 0;

float SOUND_DistanceToListener(PHD_3DPOS* pos)
{
	// 2D Pos ?
	if (pos == NULL)
		return 0.0f;
	return (pos->ToVector3() - camera.mike_pos.ToVector3()).Length();
}

float SOUND_DistanceToListener(PHD_VECTOR* pos)
{
	// 2D Pos ?
	if (pos == NULL)
		return 0.0f;
	return (pos->ToVector3() - camera.mike_pos.ToVector3()).Length();
}

float SOUND_Attenuate(float gain, float distance, float radius)
{
	float result = gain * (1.0f - (distance / radius));
	result = result < 0 ? 0.0f : (result > 1.0f ? 1.0f : result);
	return result * (float(SFXVolume) / 100.0f);
}

void SOUND_FreeSlot(int index, unsigned int fadeout)
{
	if (index > SOUND_MAX_CHANNELS || index < 0)
		return;

	auto* slot = &LaSlot[index];
	if (slot->channel != NULL && BASS_ChannelIsActive(slot->channel) == BASS_ACTIVE_PLAYING)
	{
		if (fadeout > 0)
			BASS_ChannelSlideAttribute(slot->channel, BASS_ATTRIB_VOL, -1.0f, fadeout);
		else
			BASS_ChannelStop(slot->channel);
	}
	slot->channel = NULL;
	slot->state = SOUND_STATES::Idle;
	slot->sample_index = NO_SAMPLES;
}

void SOUND_FreeSample(int index)
{
	auto& sample = SamplePointer[index];
	if (sample != NULL)
	{
		BASS_SampleFree(sample);
		sample = NULL;
	}
}

int SOUND_GetFreeSlot()
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto* slot = &LaSlot[i];
		if (slot->channel == NULL || BASS_ChannelIsActive(slot->channel) == BASS_ACTIVE_STOPPED)
			return i;
	}

	// No free slot, get a old one !
	float minDistance = 0.0f;
	int farSlot = NO_SAMPLES;
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto* slot = &LaSlot[i];
		float distance = Vector3::Distance(slot->origin.ToVector3(), camera.mike_pos.ToVector3());
		if (distance > minDistance)
		{
			minDistance = distance;
			farSlot = i;
		}
	}

	SOUND_FreeSlot(farSlot, SOUND_XFADETIME_HIJACKSOUND);
	return farSlot;
}

int SOUND_EffectIsPlaying(int index, PHD_3DPOS* pos)
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto* slot = &LaSlot[i];
		if (slot->sample_index == index)
		{
			if (slot->channel == NULL)	// Free channel
				continue;

			if (BASS_ChannelIsActive(slot->channel) == BASS_ACTIVE_PLAYING)
			{
				// Only check position on 3D samples. 2D samples stop immediately.
				BASS_CHANNELINFO info;
				BASS_ChannelGetInfo(slot->channel, &info);
				if (!(info.flags & BASS_SAMPLE_3D) || pos == NULL)
					return i;

				// Check if effect origin is equal OR in nearest possible hearing range.
				if (Vector3::Distance(pos->ToVector3(), slot->origin.ToVector3()) < SOUND_MAXVOL_RADIUS)
					return i;
			}
			else
			{
				slot->channel = NULL;
			}
		}
	}
	return NO_SAMPLES;
}

bool SOUND_EffectIsPlaying(int index)
{
	int slot = SOUND_EffectIsPlaying(index, nullptr);
	if (slot == NO_SAMPLES)
		return false;
	return LaSlot[slot].sample_index == index;
}

void SOUND_PauseAll()
{
	BASS_Pause();
}

void SOUND_ResumeAll()
{
	BASS_Start();
}

void SOUND_Stop(int index)
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto* slot = &LaSlot[i];
		if (slot->sample_index == index && slot->channel != NULL && BASS_ChannelIsActive(slot->channel) == BASS_ACTIVE_PLAYING)
			SOUND_FreeSlot(i, SOUND_XFADETIME_CUTSOUND);
	}
}

void SOUND_Init()
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
		SOUND_FreeSlot(i);
	ZeroMemory(LaSlot, sizeof(SOUND_SLOT) * SOUND_MAX_CHANNELS);
	sound_active = 1;
}

void SOUND_UpdateScene()
{
	if (!sound_active)
		return;

	// Apply environmental effects

	//static int currentReverb = -1;
	//auto roomReverb = g_Configuration.EnableReverb ? (int)g_Level.Rooms[Camera.pos.RoomNumber].reverbType : (int)ReverbType::Small;

	/*if (currentReverb == -1 || roomReverb != currentReverb)
	{
		currentReverb = roomReverb;
		if (currentReverb < (int)ReverbType::Count)
			BASS_FXSetParameters(BASS_FXHandler[(int)SoundFilter::Reverb], &BASS_ReverbTypes[(int)currentReverb]);
	}*/

	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto* slot = &LaSlot[i];
		if (slot->channel != NULL && (BASS_ChannelIsActive(slot->channel) == BASS_ACTIVE_PLAYING))
		{
			auto* sampleInfo = &sample_infos[samples_maps[slot->sample_index]];

			// Stop and clean up sounds which were in ending state in previous frame.
			// In case sound is looping, make it ending unless they are re-fired in next frame.

			if (slot->state == SOUND_STATES::Ending)
			{
				slot->state = SOUND_STATES::Ended;
				SOUND_FreeSlot(i, SOUND_XFADETIME_CUTSOUND);
				continue;
			}
			else if ((sampleInfo->flags & 3) == SFX_LOOPED)
				slot->state = SOUND_STATES::Ending;

			// Calculate attenuation and clean up sounds which are out of listener range (only for 3D sounds).
			if (slot->origin.ToVector3() != Vector3::Zero)
			{
				float radius = float(sampleInfo->radius) * 1024.0f;
				float distance = SOUND_DistanceToListener(&slot->origin);
				if (distance > radius)
				{
					SOUND_FreeSlot(i);
					continue;
				}
				else
					BASS_ChannelSetAttribute(slot->channel, BASS_ATTRIB_VOL, SOUND_Attenuate(slot->volume, distance, radius));
			}
		}
	}

	// Apply current listener position.

	Vector3 result = camera.target.ToVector3() - camera.mike_pos.ToVector3();
	result.Normalize();
	auto pos = camera.mike_pos.ToBassVector();
	auto laraVel = lara.FromVelocityToBassVector();
	auto atVec = BASS_3DVECTOR(result.x, result.y, result.z);
	auto upVec = BASS_3DVECTOR(0.0f, 1.0f, 0.0f);
	BASS_Set3DPosition(&pos, &laraVel, &atVec, &upVec);
	BASS_Apply3D();
}

void SOUND_StopAll()
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
		SOUND_FreeSlot(i);
	ZeroMemory(LaSlot, sizeof(SOUND_SLOT) * SOUND_MAX_CHANNELS);
}

// Update sound position in a level.
bool SOUND_UpdateEffectPosition(int index, PHD_3DPOS* pos, bool force)
{
	if (index > SOUND_MAX_CHANNELS || index < 0)
		return false;

	auto* slot = &LaSlot[index];
	if (pos)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(slot->channel, &info);
		if (info.flags & BASS_SAMPLE_3D)
		{
			slot->origin = *pos;
			auto position = pos->FromPositionToBassVector();
			auto rotation = pos->FromRotationToBassVector();
			BASS_ChannelSet3DPosition(slot->channel, &position, &rotation, NULL);
			BASS_Apply3D();
		}
	}

	// Reset activity flag, important for looped samples
	if (BASS_ChannelIsActive(slot->channel) == BASS_ACTIVE_PLAYING)
		slot->state = SOUND_STATES::Idle;

	return true;
}

// Update gain and pitch.
bool SOUND_UpdateEffectAttributes(int index, float pitch, float volume)
{
	if (index > SOUND_MAX_CHANNELS || index < 0)
		return false;
	auto* slot = &LaSlot[index];
	BASS_ChannelSetAttribute(slot->channel, BASS_ATTRIB_FREQ, 22050.0f * pitch);
	BASS_ChannelSetAttribute(slot->channel, BASS_ATTRIB_VOL, volume);
	return true;
}

int SOUND_PlayEffect(int index, PHD_3DPOS* pos, int flags, float pitchMultiplier, float gainMultiplier)
{
	if (!sound_active)
		return NO_SAMPLES;
	if (index >= SFX_SAMPLES_COUNT)
		return NO_SAMPLES;
	if (BASS_GetDevice() == -1)
		return NO_SAMPLES;

	if (index == SFX_LARA_NO)
	{
		switch (Gameflow->Language)
		{
		case 1:
			index = SFX_LARA_NO_FRENCH;
			break;
		case 6:
			index = SFX_LARA_NO_JAPAN;
			break;
		}
	}

	if ((flags & SFX_ALWAYS) != SFX_ALWAYS)
	{
		auto condition = rooms[camera.pos.room_number].flags & ROOM_UNDERWATER ? SFX_LAND : SFX_WATER;
		if ((flags & condition) != condition)
			return 0;
	}

	auto lut = samples_maps[index];
	if (lut == NO_SAMPLES)
	{
		Log(1, "Non present samples id: %d", index);
		samples_maps[index] = -2;
		return 0;
	}
	else if (lut == -2)
		return 0;

	auto* info = &sample_infos[lut];
	if (info->number < 0)
	{
		Log(1, "No valid samples id for effect: %d", lut);
		return NO_SAMPLES;
	}
	
	// Check for effect randomness, if it's true then return and avoid playing it !
	if (info->randomness && (GetRandomDraw() & 0xFF) > info->randomness)
		return NO_SAMPLES;

	// Set & randomize volume (if needed)
	float gain = (static_cast<float>(info->volume << 8) / UCHAR_MAX) * std::clamp(gainMultiplier, SOUND_MIN_PARAM_MULTIPLIER, SOUND_MAX_PARAM_MULTIPLIER);
	// Randomize volume (if needed)
	if (info->flags & 0x4000)
		gain -= (static_cast<float>(GetRandomControl()) / static_cast<float>(RAND_MAX)) * SOUND_MAX_GAIN_CHANGE;

	// Set and randomize pitch and additionally multiply by provided value (for vehicles etc)
	float pitch = (1.0f + static_cast<float>(info->pitch) / 127.0f) * std::clamp(pitchMultiplier, SOUND_MIN_PARAM_MULTIPLIER, SOUND_MAX_PARAM_MULTIPLIER);
	// Randomize pitch (if needed)
	if (info->flags & 0x2000)
		pitch += ((static_cast<float>(GetRandomControl()) / static_cast<float>(RAND_MAX)) - 0.5f) * SOUND_MAX_PITCH_CHANGE * 2.0f;

	float radius = float(info->radius) * 1024.0f;
	float distance = SOUND_DistanceToListener(pos);
	//if (distance > radius)
	//	return 0;
	float volume = SOUND_Attenuate(gain, distance, radius);

	int slotIndexExisting = SOUND_EffectIsPlaying(index, pos);
	auto flag = info->flags & 3;
	switch (flag)
	{
	case SFX_NORMAL:
		break;

	case SFX_WAIT:
		if (slotIndexExisting != NO_SAMPLES)
			return 0;
		break;
	case SFX_RESTART:
		if (slotIndexExisting != NO_SAMPLES)
			SOUND_FreeSlot(slotIndexExisting, SOUND_XFADETIME_CUTSOUND);
		break;
	case SFX_LOOPED:
		if (slotIndexExisting != NO_SAMPLES)
		{
			SOUND_UpdateEffectPosition(slotIndexExisting, pos);
			SOUND_UpdateEffectAttributes(slotIndexExisting, pitch, volume);
			return 0;
		}
		break;
	}

	int sampleToPlay = 0;
	int numSamples = (info->flags >> 2) & 15;
	if (numSamples == 1)
		sampleToPlay = info->number;
	else
		sampleToPlay = info->number + (int)((GetRandomControl() * numSamples) >> 15);

	int freeSlot = SOUND_GetFreeSlot();
	if (freeSlot == NO_SAMPLES)
	{
		Log(1, "No free channel slot available !");
		return NO_SAMPLES;
	}

	auto* slot = &LaSlot[freeSlot];
	auto channel = BASS_SampleGetChannel(SamplePointer[sampleToPlay], 1);
	if (flags == SFX_LOOPED)
		BASS_ChannelFlags(channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
	slot->sample_index = index;
	slot->pitch = pitch;
	slot->volume = volume;
	slot->distance = distance;
	slot->origin = pos != NULL ? *pos : Phd3DPosEmpty;
	slot->channel = channel;
	slot->state = Idle;

	BASS_ChannelPlay(channel, FALSE);
	BASS_ChannelSet3DAttributes(channel, pos ? BASS_3DMODE_NORMAL : BASS_3DMODE_OFF, SOUND_MAXVOL_RADIUS, radius, 360, 360, 0.0f);
	SOUND_UpdateEffectPosition(freeSlot, pos, true);
	SOUND_UpdateEffectAttributes(freeSlot, pitch, volume);

	return 1;
}

void SOUND_SayNo()
{
	SOUND_PlayEffect(SFX_LARA_NO, NULL, SFX_ALWAYS);
}

void SOUND_EndScene()
{
	if (!sound_active) return;

	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto* slot = &LaSlot[i];
		if (slot->sample_index < 0)
			continue;

		if ((sample_infos[slot->sample_index].flags & 3) != SFX_LOOPED)
		{
			if (!SOUND_EffectIsPlaying(i))
			{
				slot->sample_index = -1;
			}
		}
		else
		{
			if (!slot->volume)
			{
				SOUND_FreeSlot(i);
				slot->sample_index = NO_SAMPLES;
			}
			else
			{
				SOUND_UpdateEffectPosition(i, &slot->origin);
				slot->volume = 0;
			}
		}
	}
}
