#include "../tomb4/pch.h"
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

SAMPLE_INFO* sample_infos;
SOUND_SLOT LaSlot[SOUND_MAX_CHANNELS];
short* samples_maps;
long sound_active = 0;
PHD_3DPOS SOUND_Pos2D = PHD_3DPOS{ 0, 0, 0, 0, 0, 0 };

float SOUND_DistanceToListener(PHD_3DPOS* pos)
{
	// 2D Pos ?
	if (pos == NULL)
		return 0.0f;
	return phd_GetDistanceFrom3DPosAndGameVector(pos, &camera.pos);
}

float SOUND_DistanceToListener(PHD_VECTOR* pos)
{
	// 2D Pos ?
	if (pos == NULL)
		return 0.0f;
	return phd_GetDistanceFromVectorAndGameVector(pos, &camera.pos);
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

	auto& slot = LaSlot[index];
	if (slot.channel != NULL && BASS_ChannelIsActive(slot.channel))
	{
		if (fadeout > 0)
			BASS_ChannelSlideAttribute(slot.channel, BASS_ATTRIB_VOL, -1.0f, fadeout);
		else
			BASS_ChannelStop(slot.channel);
	}
	slot.channel = NULL;
	slot.state = SoundState::SS_IsEnded;
	slot.nSampleInfo = -1;
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
		auto& slot = LaSlot[i];
		if (slot.channel == NULL || !BASS_ChannelIsActive(slot.channel))
			return i;
	}

	// No free slot, get a old one !
	int minDistance = 0;
	int farSlot = -1;

	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto& slot = LaSlot[i];
		int distance = phd_GetDistanceFromVectorAndGameVector(&slot.pos, &camera.pos);
		if (distance > minDistance)
		{
			minDistance = distance;
			farSlot = i;
		}
	}

	SOUND_FreeSlot(farSlot, 0);
	return farSlot;
}

int SOUND_EffectIsPlaying(int index, PHD_3DPOS* pos)
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		auto& slot = LaSlot[i];
		if (slot.nSampleInfo == index)
		{
			if (slot.channel == NULL)	// Free channel
				continue;

			if (BASS_ChannelIsActive(slot.channel))
			{
				// Only check position on 3D samples. 2D samples stop immediately.
				BASS_CHANNELINFO info;
				BASS_ChannelGetInfo(slot.channel, &info);
				if (!(info.flags & BASS_SAMPLE_3D) || pos == NULL)
					return i;

				// Check if effect origin is equal OR in nearest possible hearing range.
				if (phd_GetDistanceFrom3DPosAndVector(pos, &slot.pos) < SOUND_MAXVOL_RADIUS)
					return i;
			}
			else
			{
				slot.channel = NULL;
			}
		}
	}
	return -1;
}

bool SOUND_EffectIsPlaying(int index)
{
	int channelIndex = SOUND_EffectIsPlaying(index, nullptr);
	if (channelIndex == -1)
		return false;
	return LaSlot[channelIndex].nSampleInfo == index;
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
		auto& slot = LaSlot[i];
		if (slot.nSampleInfo == index)
			SOUND_FreeSlot(i);
	}
}

void SOUND_Init()
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
		SOUND_FreeSlot(i);
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
		auto& slot = LaSlot[i];
		if (slot.channel != NULL && (BASS_ChannelIsActive(slot.channel) == BASS_ACTIVE_PLAYING))
		{
			auto* sampleInfo = &sample_infos[samples_maps[slot.nSampleInfo]];

			// Stop and clean up sounds which were in ending state in previous frame.
			// In case sound is looping, make it ending unless they are re-fired in next frame.

			if (slot.state == SoundState::SS_IsEnding)
			{
				slot.state = SoundState::SS_IsEnded;
				SOUND_FreeSlot(i, 0);
				continue;
			}
			else if ((sampleInfo->flags & 3) == 3)
				slot.state = SoundState::SS_IsEnding;

			// Calculate attenuation and clean up sounds which are out of listener range (only for 3D sounds).

			if (slot.pos.x == 0 && slot.pos.y == 0 && slot.pos.z == 0)
			{
				float radius = float(sampleInfo->radius) * 1024.0f;
				float distance = SOUND_DistanceToListener(&slot.pos);
				if (distance > radius)
				{
					SOUND_FreeSlot(i);
					continue;
				}
				else
					BASS_ChannelSetAttribute(slot.channel, BASS_ATTRIB_VOL, SOUND_Attenuate(slot.nVolume, distance, radius));
			}
		}
	}

	// Apply current listener position.
	D3DVECTOR vec1 = D3DVECTOR(camera.target.x, camera.target.y, camera.target.z);
	D3DVECTOR vec2 = D3DVECTOR(camera.mike_pos.x, camera.mike_pos.y, camera.mike_pos.z);
	D3DVECTOR result = vec2 - vec1;
	D3DNormalise(&result);
	auto mikePos = BASS_3DVECTOR(camera.mike_pos.x, camera.mike_pos.y, camera.mike_pos.z);
	auto laraVel = BASS_3DVECTOR(lara.current_xvel, lara.current_yvel, lara.current_zvel);
	auto atVec = BASS_3DVECTOR(result.x, result.y, result.z);
	auto upVec = BASS_3DVECTOR(0.0f, 1.0f, 0.0f);
	BASS_Set3DPosition(&mikePos, &laraVel, &atVec, &upVec);
	BASS_Apply3D();
}

void SOUND_StopAll()
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
		SOUND_FreeSlot(i);
}

// Update sound position in a level.
bool SOUND_UpdateEffectPosition(int index, PHD_3DPOS* pos, bool force)
{
	if (index > SOUND_MAX_CHANNELS || index < 0)
		return false;

	auto& slot = LaSlot[index];
	if (pos)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(slot.channel, &info);
		if (info.flags & BASS_SAMPLE_3D)
		{
			slot.pos.x = pos->x_pos;
			slot.pos.y = pos->y_pos;
			slot.pos.z = pos->z_pos;

			auto position = BASS_3DVECTOR(pos->x_pos, pos->y_pos, pos->z_pos);
			auto rotation = BASS_3DVECTOR(pos->x_rot, pos->y_rot, pos->z_rot);
			BASS_ChannelSet3DPosition(slot.channel, &position, &rotation, NULL);
			BASS_Apply3D();
		}
	}

	// Reset activity flag, important for looped samples
	if (BASS_ChannelIsActive(slot.channel))
		slot.state = SoundState::SS_IsPlaying;

	return true;
}

// Update gain and pitch.
bool SOUND_UpdateEffectAttributes(int index, float pitch, float gain)
{
	if (index > SOUND_MAX_CHANNELS || index < 0)
		return false;
	BASS_ChannelSetAttribute(LaSlot[index].channel, BASS_ATTRIB_FREQ, 22050.0f * pitch);
	BASS_ChannelSetAttribute(LaSlot[index].channel, BASS_ATTRIB_VOL, gain);
	return true;
}

int SOUND_PlayEffect(int index, PHD_3DPOS* pos, int flags)
{
	if (index >= NumSamples)
		return -1;
	if (!sound_active)
		return -1;
	if (BASS_GetDevice() == -1)
		return -1;

	if (index == SFX_LARA_NO)
	{
		switch (Gameflow->Language)
		{
		case 1:
			index = SFX_LARA_NO_FRENCH;
			break;

		case 2:
		case 3:
		case 4:
			index = SFX_LARA_NO;
			break;

		case 6:
			index = SFX_LARA_NO_JAPAN;
			break;
		}
	}

	if (flags & SFX_WATER)
	{
		if (!(room[camera.pos.room_number].flags & ROOM_UNDERWATER))
			return -1;
	}

	auto lut = samples_maps[index];
	if (lut == -1)
	{
		Log(1, "Non present samples id: %d", index);
		samples_maps[index] = -2;
		return -1;
	}
	else if (lut == -2)
		return -1;

	auto* info = &sample_infos[lut];
	if (info->number < 0)
	{
		Log(1, "No valid samples id for effect: %d", lut);
		return -1;
	}
	
	// Check for effect randomness, if it's true then return and avoid playing it !
	if (info->randomness && (GetRandomDraw() & 0xFF) > info->randomness)
		return -1;

	DWORD sampleFlags = BASS_SAMPLE_MONO|BASS_SAMPLE_FLOAT;
	if (pos != NULL)
		sampleFlags |= BASS_SAMPLE_3D;

	// Set & randomize volume (if needed)
	float gain = (static_cast<float>(info->volume << 8) / UCHAR_MAX)/* * std::clamp(gainMultiplier, SOUND_MIN_PARAM_MULTIPLIER, SOUND_MAX_PARAM_MULTIPLIER)*/;
	if (info->flags & 0x4000)
		gain -= (static_cast<float>(GetRandomControl()) / static_cast<float>(RAND_MAX)) * SOUND_MAX_GAIN_CHANGE;

	// Set and randomize pitch and additionally multiply by provided value (for vehicles etc)
	float pitch = (1.0f + static_cast<float>(info->pitch) / 127.0f)/* * std::clamp(pitchMultiplier, SOUND_MIN_PARAM_MULTIPLIER, SOUND_MAX_PARAM_MULTIPLIER)*/;
	// Randomize pitch (if needed)
	if (info->flags & 0x2000)
		pitch += ((static_cast<float>(GetRandomControl()) / static_cast<float>(RAND_MAX)) - 0.5f) * SOUND_MAX_PITCH_CHANGE * 2.0f;


	float radius = float(info->radius) * 1024.0f;
	float distance = SOUND_DistanceToListener(pos);
	float volume = SOUND_Attenuate(gain, distance, radius);
	int freeSlot = SOUND_GetFreeSlot();
	if (freeSlot == -1)
	{
		Log(1, "No free channel slot available !");
		return -1;
	}

	int existingChannel = SOUND_EffectIsPlaying(index, pos);
	auto flag = info->flags & 3;
	switch (flag)
	{
	case 1:
		if (existingChannel != -1)
			return freeSlot;
		break;
	case 2:
		if (existingChannel != -1)
			SOUND_FreeSlot(existingChannel, 0);
		break;
	case 3:
		if (existingChannel != -1)
		{
			SOUND_UpdateEffectPosition(existingChannel, pos, false);
			SOUND_UpdateEffectAttributes(existingChannel, pitch, volume);
			return freeSlot;
		}
		sampleFlags |= BASS_SAMPLE_LOOP;
		break;
	}

	int sampleToPlay = 0;
	int numSamples = (info->flags >> 2) & 15;
	if (numSamples == 1)
		sampleToPlay = info->number;
	else
		sampleToPlay = info->number + (int)((GetRandomControl() * numSamples) >> 15);

	HSTREAM channel = BASS_SampleGetChannel(SamplePointer[sampleToPlay], flag == 3);
	LaSlot[freeSlot].OrigVolume = info->volume;
	LaSlot[freeSlot].nVolume = volume;
	LaSlot[freeSlot].nPan = gain;
	LaSlot[freeSlot].nPitch = pitch;
	LaSlot[freeSlot].nSampleInfo = index;
	LaSlot[freeSlot].distance = distance;
	if (pos == NULL)
	{
		LaSlot[freeSlot].pos.x = 0;
		LaSlot[freeSlot].pos.y = 0;
		LaSlot[freeSlot].pos.z = 0;
	}
	else
	{
		LaSlot[freeSlot].pos.x = pos->x_pos;
		LaSlot[freeSlot].pos.y = pos->y_pos;
		LaSlot[freeSlot].pos.z = pos->z_pos;
	}
	LaSlot[freeSlot].channel = channel;
	LaSlot[freeSlot].state = SS_IsPlaying;
	if (flag == 3)
		BASS_ChannelFlags(channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
	BASS_ChannelPlay(channel, false);
	BASS_ChannelSet3DAttributes(channel, pos ? BASS_3DMODE_NORMAL : BASS_3DMODE_OFF, SOUND_MAXVOL_RADIUS, radius, 360, 360, 0.0f);
	SOUND_UpdateEffectPosition(freeSlot, pos, true);
	SOUND_UpdateEffectAttributes(freeSlot, pitch, volume);
	return freeSlot;
}

void SOUND_SayNo()
{
	SOUND_PlayEffect(SFX_LARA_NO, NULL, SFX_ALWAYS);
}
