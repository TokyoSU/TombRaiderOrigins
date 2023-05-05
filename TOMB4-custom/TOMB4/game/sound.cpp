#include "pch.h"
#include "sound.h"
#include "camera.h"
#include "control.h"
#include "lara.h"
#include "../specific/file.h"
#include "../specific/function_stubs.h"
#include "../specific/LoadSave.h"
#include "../specific/winmain.h"
#include "effects.h"

const BASS_BFX_FREEVERB BASS_ReverbTypes[RT_Count] =    // Reverb presets
{ // Dry Mix | Wet Mix |  Size   |  Damp   |  Width  |  Mode  | Channel
  {  1.0f,     0.20f,     0.05f,    0.90f,    0.7f,     0,      -1     },	// 0 = Outside
  {  1.0f,     0.20f,     0.35f,    0.15f,    0.8f,     0,      -1     },	// 1 = Small room
  {  1.0f,     0.25f,     0.55f,    0.20f,    1.0f,     0,      -1     },	// 2 = Medium room
  {  1.0f,     0.25f,     0.80f,    0.50f,    1.0f,     0,      -1     },	// 3 = Large room
  {  1.0f,     0.25f,     0.90f,    1.00f,    1.0f,     0,      -1     }	// 4 = Pipe
};

SoundSystem Sound;
SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::Init()
{
	// Initialise BASS (Will take DirectSound in priority since App.hWnd is used, except if its null)
	BASS_Init(-1, 44100, BASS_DEVICE_3D, App.hWnd, NULL);
	if (CheckBASSError("Initializing BASS sound device", true))
	{
		App.SoundDisabled = true;
		return;
	}

	// Initialize BASS_FX plugin
	BASS_FX_GetVersion();
	if (CheckBASSError("Initializing FX plugin", true))
		return;

	// Set 3D world parameters.
	// Rolloff is lessened since we have own attenuation implementation.
	BASS_Set3DFactors(SOUND_BASS_UNITS, 1.5f, 0.5f);
	BASS_SetConfig(BASS_CONFIG_3DALGORITHM, BASS_3DALG_FULL);

	// Set minimum latency and 2 threads for updating.
	// Most of modern PCs already have multi-core CPUs, so why not parallelize updating?
	BASS_SetConfig(BASS_CONFIG_UPDATETHREADS, 2);
	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 10);

	// Create 3D mixdown channel and make it play forever.
	// For realtime mixer channels, we need minimum buffer latency. It shouldn't affect reliability.
	BASS_SetConfig(BASS_CONFIG_BUFFER, 40);
	BASS_3D_Mixdown = BASS_StreamCreate(44100, 2, BASS_SAMPLE_FLOAT, STREAMPROC_DEVICE_3D, NULL);
	BASS_ChannelPlay(BASS_3D_Mixdown, false);

	// Reset buffer back to normal value.
	BASS_SetConfig(BASS_CONFIG_BUFFER, 300);

	if (CheckBASSError("Starting 3D mixdown", true))
		return;

	// Initialize channels and tracks array
	//ZeroMemory(BASS_Soundtrack, (sizeof(HSTREAM) * (int)SoundTrackType::Count));
	ZeroMemory(SoundSlot, sizeof(SOUND_SLOT) * SOUND_MAX_CHANNELS);

	// Attach reverb effect to 3D channel
	BASS_FXHandler[SF_Reverb] = BASS_ChannelSetFX(BASS_3D_Mixdown, BASS_FX_BFX_FREEVERB, 0);
	BASS_FXSetParameters(BASS_FXHandler[SF_Reverb], &BASS_ReverbTypes[RT_Outside]);

	if (CheckBASSError("Attaching environmental FX", true))
		return;

	// Apply slight compression to 3D channel
	BASS_FXHandler[SF_Compressor] = BASS_ChannelSetFX(BASS_3D_Mixdown, BASS_FX_BFX_COMPRESSOR2, 1);
	auto comp = BASS_BFX_COMPRESSOR2{ 4.0f, -18.0f, 1.5f, 10.0f, 100.0f, -1 };
	BASS_FXSetParameters(BASS_FXHandler[SF_Compressor], &comp);

	CheckBASSError("Attaching compressor", true);

	App.SoundDisabled = false;
}

void SoundSystem::Release()
{
	if (!App.SoundDisabled)
	{
		Log(1, "Shutting down BASS...");
		if (BASS_3D_Mixdown != NULL)
		{
			BASS_StreamFree(BASS_3D_Mixdown);
			BASS_3D_Mixdown = NULL;
		}
		BASS_Free();
	}
}

int SoundSystem::PlayEffect(int soundFX, PHD_3DPOS* pos, SFX_OPTIONS option, float pitchMult, float gainMult)
{
	if (App.SoundDisabled) return false;
	if (BASS_GetDevice() == -1) return false;

	// TODO: check for sound environment !

	int sampleIndex = samples_maps[soundFX];
	// -1 means no such effect exists in level file.
	// We set it to -2 afterwards to prevent further debug message firings.
	if (sampleIndex == -1)
	{
		Log(2, "Non present effect: %d", soundFX);
		samples_maps[soundFX] = -2;
		return false;
	}
	else if (sampleIndex == -2)
		return false;

	SAMPLE_INFO* sampleInfo = &sample_infos[sampleIndex];
	if (sampleInfo->number < 0)
	{
		Log(2, "No valid samples count for effect: %d", sampleIndex);
		return false;
	}

	// Effect's chance to play.
	if ((sampleInfo->randomness) && ((GetRandomControl() & UCHAR_MAX) > sampleInfo->randomness))
		return false;

	// Set & randomize volume (if needed)
	float gain = (static_cast<float>(sampleInfo->volume) / UCHAR_MAX) * std::clamp(gainMult, SOUND_MIN_PARAM_MULTIPLIER, SOUND_MAX_PARAM_MULTIPLIER);
	if ((sampleInfo->flags & SOUND_FLAG_RND_GAIN))
		gain -= (static_cast<float>(GetRandomControl()) / static_cast<float>(RAND_MAX)) * SOUND_MAX_GAIN_CHANGE;

	// Set and randomize pitch and additionally multiply by provided value (for vehicles etc)
	float pitch = (1.0f + static_cast<float>(sampleInfo->pitch) / 127.0f) * std::clamp(pitchMult, SOUND_MIN_PARAM_MULTIPLIER, SOUND_MAX_PARAM_MULTIPLIER);

	// Randomize pitch (if needed)
	if ((sampleInfo->flags & SOUND_FLAG_RND_PITCH))
		pitch += ((static_cast<float>(GetRandomControl()) / static_cast<float>(RAND_MAX)) - 0.5f) * SOUND_MAX_PITCH_CHANGE * 2.0f;

	// Calculate sound radius and distance to sound
	float radius = (float)(sampleInfo->radius) * 1024.0f;
	float distance = DistanceToListener(pos);

	// Don't play sound if it's too far from listener's position.
	if (distance > radius)
		return false;

	// Get final volume of a sound.
	float volume = Attenuate(gain, distance, radius);

	// Get existing index, if any, of sound which is playing.
	int existingChannel = EffectIsPlaying(soundFX, pos);

	// Select behaviour based on effect playback type (bytes 0-1 of flags field)
	auto playType = (SFX_FLAGS)(sampleInfo->flags & 3);
	switch (playType)
	{
	case SFXF_NORMAL:
		break;

	case SFXF_WAIT:
		if (existingChannel != -1) // Don't play until stopped
			return false;
		break;

	case SFXF_RESTART:
		if (existingChannel != -1) // Stop existing and continue
			FreeSlot(existingChannel, SOUND_XFADETIME_CUTSOUND);
		break;

	case SFXF_LOOPED:
		if (existingChannel != -1) // Just update parameters and return, if already playing
		{
			UpdateEffectPosition(existingChannel, pos);
			UpdateEffectAttributes(existingChannel, pitch, volume);
			return false;
		}
		break;
	}

	// Randomly select arbitrary sample from the list, if more than one is present
	int sampleToPlay = 0;
	int numSamples = (sampleInfo->flags >> 2) & 15;
	if (numSamples == 1)
		sampleToPlay = sampleInfo->number;
	else
		sampleToPlay = sampleInfo->number + (int)((GetRandomControl() * numSamples) >> 15);

	// Get free channel to play sample
	int freeSlot = GetFreeSlot();
	if (freeSlot == -1)
	{
		Log(2, "No free channel slot available !");
		return false;
	}

	// Create sample's stream and reset buffer back to normal value.
	HSTREAM channel = BASS_SampleGetChannel(SamplePointer[sampleToPlay], true);

	if (CheckBASSError("Trying to create channel for sample %d", false, sampleToPlay))
		return false;

	// Finally ready to play sound, assign it to sound slot.
	SoundSlot[freeSlot].state = Idle;
	SoundSlot[freeSlot].sample_index = soundFX;
	SoundSlot[freeSlot].channel = channel;
	SoundSlot[freeSlot].volume = gain;
	SoundSlot[freeSlot].origin = pos ? Vector3((float)pos->x_pos, (float)pos->y_pos, (float)pos->z_pos) : SOUND_OMNIPRESENT_ORIGIN;

	if (CheckBASSError("Applying pitch/gain attribs on channel %x, sample %d", false, channel, sampleToPlay))
		return false;

	// Set looped flag, if necessary
	if (playType == SFXF_LOOPED)
		BASS_ChannelFlags(channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);

	// Play channel
	BASS_ChannelPlay(channel, false);

	if (CheckBASSError("Queuing channel %x on sample mixer", false, freeSlot))
		return false;

	// Set attributes
	BASS_ChannelSet3DAttributes(channel, pos != NULL ? BASS_3DMODE_NORMAL : BASS_3DMODE_OFF, SOUND_MAXVOL_RADIUS, radius, 360, 360, 0.0f);
	UpdateEffectPosition(freeSlot, pos, true);
	UpdateEffectAttributes(freeSlot, pitch, volume);

	if (CheckBASSError("Applying 3D attribs on channel %x, sound %d", false, channel, soundFX))
		return false;

	return true;
}

void SoundSystem::StopEffect(int soundFX)
{
	for (int slot = 0; slot < SOUND_MAX_CHANNELS; slot++)
	{
		if (SoundSlot[slot].channel != NULL && SoundSlot[slot].sample_index == soundFX && BASS_ChannelIsActive(SoundSlot[slot].channel) == BASS_ACTIVE_PLAYING)
			FreeSlot(slot, SOUND_XFADETIME_CUTSOUND);
	}
}

bool SoundSystem::IsSoundEffectPlaying(int soundFX)
{
	int channelIndex = EffectIsPlaying(soundFX, nullptr);
	if (channelIndex == -1)
		return false;
	return (SoundSlot[channelIndex].sample_index == soundFX);
}

void SoundSystem::StopAllEffects()
{
	for (int slot = 0; slot < SOUND_MAX_CHANNELS; slot++)
		FreeSlot(slot, SOUND_XFADETIME_CUTSOUND);
	ZeroMemory(SoundSlot, sizeof(SOUND_SLOT) * SOUND_MAX_CHANNELS);
}

void SoundSystem::PauseAllSounds()
{
	BASS_Pause();
}

void SoundSystem::ResumeAllSounds()
{
	BASS_Start();
}

int SoundSystem::EffectIsPlaying(int soundFX, PHD_3DPOS* position)
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		if (SoundSlot[i].sample_index == soundFX)
		{
			if (SoundSlot[i].channel == NULL)	// Free channel
				continue;

			if (BASS_ChannelIsActive(SoundSlot[i].channel))
			{
				// Only check position on 3D samples. 2D samples stop immediately.

				BASS_CHANNELINFO info;
				BASS_ChannelGetInfo(SoundSlot[i].channel, &info);
				if (!(info.flags & BASS_SAMPLE_3D) || position == NULL)
					return i;

				// Check if effect origin is equal OR in nearest possible hearing range.
				Vector3 origin = Vector3(position->x_pos, position->y_pos, position->z_pos);
				if (Vector3::Distance(origin, SoundSlot[i].origin) < SOUND_MAXVOL_RADIUS)
					return i;
			}
			else
				SoundSlot[i].channel = NULL; // WTF, let's clean this up
		}
	}
	return -1;
}

void SoundSystem::SetReverbType(REVERB_TYPES reverb)
{
	static REVERB_TYPES currentReverb = RT_Outside;
	if (currentReverb == RT_Outside || reverb != currentReverb)
	{
		currentReverb = reverb;
		if (currentReverb < RT_Count)
			BASS_FXSetParameters(BASS_FXHandler[SF_Reverb], &BASS_ReverbTypes[(int)currentReverb]);
	}

}

void SoundSystem::PlaySoundSources()
{
	if (App.SoundDisabled) return;

	constexpr int PLAY_BASE_ROOM = 0x80;
	constexpr int PLAY_FLIP_ROOM = 0x40;

	for (long i = 0; i < number_sound_effects; i++)
	{
		const auto& sound = sound_effects[i];
		if (!flip_status && (sound.flags & PLAY_FLIP_ROOM))
			continue;
		else if (flip_status && (sound.flags & PLAY_BASE_ROOM))
			continue;
		PlayEffect(sound.data, (PHD_3DPOS*)&sound);
	}

	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		if (SoundSlot[i].sample_index < 0)
			continue;

		if ((sample_infos[SoundSlot[i].sample_index].flags & 3) != SFXF_LOOPED)
		{
			if (!EffectIsPlaying(i, NULL))
				SoundSlot[i].sample_index = -1;
			else
				UpdateEffectAttributes(i, SoundSlot[i].pitch, SoundSlot[i].volume);
		}
		else
		{
			if (SoundSlot[i].volume)
			{
				UpdateEffectAttributes(i, SoundSlot[i].pitch, SoundSlot[i].volume);
				SoundSlot[i].volume = 0;
			}
			else
			{
				FreeSlot(i);
				SoundSlot[i].sample_index = -1;
			}
		}
	}
}

bool SoundSystem::LoadSample(CHAR* buffer, int comp_size, int uncomp_size, int current_index)
{
	if (current_index >= SFX_SAMPLES_COUNT)
	{
		Log(2, "Sample index %d is larger than max. amount of samples", current_index);
		return 0;
	}

	if (buffer == nullptr || comp_size <= 0)
	{
		Log(2, "Sample size or memory address is incorrect for index %d", current_index);
		return 0;
	}

	// Load and uncompress sample to 32-bit float format.
	HSAMPLE sample = BASS_SampleLoad(true, buffer, 0, comp_size, 1, SOUND_SAMPLE_FLAGS);
	if (!sample)
	{
		Log(2, "Error loading sample %d", current_index);
		return false;
	}

	// Try to free sample before allocating new one.
	FreeSample(current_index);

	BASS_SAMPLE info;
	BASS_SampleGetInfo(sample, &info);
	int finalLength = info.length + 44;	// uncompSize is invalid after 16->32 bit conversion

	if (info.freq != 22050 || info.chans != 1)
	{
		Log(2, "Wrong sample parameters, must be 22050 Hz Mono");
		return false;
	}

	// Generate RIFF/WAV header to simplify loading sample data to stream. In case if RIFF/WAV header
	// exists, stream could be completely created just by calling BASS_StreamCreateFile().
	char* uncompBuffer = new char[finalLength];
	ZeroMemory(uncompBuffer, finalLength);
	memcpy(uncompBuffer, "RIFF\0\0\0\0WAVEfmt \20\0\0\0", 20);
	memcpy(uncompBuffer + 36, "data\0\0\0\0", 8);

	WAVEFORMATEX* wf = (WAVEFORMATEX*)(uncompBuffer + 20);
	wf->wFormatTag = 3;
	wf->nChannels = info.chans;
	wf->wBitsPerSample = 32;
	wf->nSamplesPerSec = info.freq;
	wf->nBlockAlign = wf->nChannels * wf->wBitsPerSample / 8;
	wf->nAvgBytesPerSec = wf->nSamplesPerSec * wf->nBlockAlign;

	// Copy raw PCM data from temporary sample buffer to actual buffer which will be used by engine.
	BASS_SampleGetData(sample, uncompBuffer + 44);
	BASS_SampleFree(sample);

	// Cut off trailing silence from samples to prevent gaps in looped playback
	int cleanLength = info.length;
	for (DWORD i = 4; i < info.length; i += 4)
	{
		float* currentSample = reinterpret_cast<float*>(uncompBuffer + finalLength - i);
		if (*currentSample > SOUND_32BIT_SILENCE_LEVEL || *currentSample < -SOUND_32BIT_SILENCE_LEVEL)
		{
			int alignment = i % wf->nBlockAlign;
			cleanLength -= (i - alignment);
			break;
		}
	}

	// Put data size to header
	*(DWORD*)(uncompBuffer + 4) = cleanLength + 44 - 8;
	*(DWORD*)(uncompBuffer + 40) = cleanLength;

	// Create actual sample
	SamplePointer[current_index] = BASS_SampleLoad(true, uncompBuffer, 0, cleanLength + 44, 65535, SOUND_SAMPLE_FLAGS | BASS_SAMPLE_3D);
	delete[] uncompBuffer;

	return true;
}

void SoundSystem::FreeSamples()
{
	StopAllEffects();
	for (int index = 0; index < SFX_SAMPLES_COUNT; index++)
		FreeSample(index);
}

void SoundSystem::SayNo()
{
	// TODO: check for language (japan and french too!)
	PlayEffect(SFX_LARA_NO, nullptr, SFXO_ALWAYS);
}

void SoundSystem::UpdateScene()
{
	if (App.SoundDisabled) return;
	for (int slot = 0; slot < SOUND_MAX_CHANNELS; slot++)
	{
		if ((SoundSlot[slot].channel != NULL) && (BASS_ChannelIsActive(SoundSlot[slot].channel) == BASS_ACTIVE_PLAYING))
		{
			SAMPLE_INFO* sampleInfo = &sample_infos[samples_maps[SoundSlot[slot].sample_index]];
			// Stop and clean up sounds which were in ending state in previous frame.
			// In case sound is looping, make it ending unless they are re-fired in next frame.

			if (SoundSlot[slot].state == Ending)
			{
				SoundSlot[slot].state = Ended;
				FreeSlot(slot, SOUND_XFADETIME_CUTSOUND);
				continue;
			}
			else if ((SFX_FLAGS)(sampleInfo->flags & 3) == SFXF_LOOPED)
				SoundSlot[slot].state = Ending;

			// Calculate attenuation and clean up sounds which are out of listener range (only for 3D sounds).

			if (SoundSlot[slot].origin != SOUND_OMNIPRESENT_ORIGIN)
			{
				float radius = (float)(sampleInfo->radius) * 1024.0f;
				float distance = DistanceToListener(SoundSlot[slot].origin);
				if (distance > radius)
				{
					FreeSlot(slot);
					continue;
				}
				else
					BASS_ChannelSetAttribute(SoundSlot[slot].channel, BASS_ATTRIB_VOL, Attenuate(SoundSlot[slot].volume, distance, radius));
			}
		}
	}

	Vector3 at = Vector3(camera.target.x, camera.target.y, camera.target.z) -
		         Vector3(camera.mike_pos.x, camera.mike_pos.y, camera.mike_pos.z);
	at.Normalize();
	auto mikePos = BASS_3DVECTOR(					// Pos
		camera.mike_pos.x,
		camera.mike_pos.y,
		camera.mike_pos.z);
	auto laraVel = BASS_3DVECTOR(					// Vel
		lara.current_xvel,
		lara.current_yvel,
		lara.current_zvel);
	auto atVec = BASS_3DVECTOR(at.x, at.y, at.z);	// At
	auto upVec = BASS_3DVECTOR(0.0f, 1.0f, 0.0f);	// Up
	BASS_Set3DPosition(&mikePos,
		&laraVel,
		&atVec,
		&upVec);
	BASS_Apply3D();
}

bool SoundSystem::UpdateEffectPosition(int slot, PHD_3DPOS* position, bool force)
{
	if (slot > SOUND_MAX_CHANNELS || slot < 0)
		return false;

	if (position)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(SoundSlot[slot].channel, &info);
		if (info.flags & BASS_SAMPLE_3D)
		{
			SoundSlot[slot].origin = Vector3(position->x_pos, position->y_pos, position->z_pos);
			auto pos = BASS_3DVECTOR(position->x_pos, position->y_pos, position->z_pos);
			auto rot = BASS_3DVECTOR(position->x_rot, position->y_rot, position->z_rot);
			BASS_ChannelSet3DPosition(SoundSlot[slot].channel, &pos, &rot, NULL);
			BASS_Apply3D();
		}
	}

	// Reset activity flag, important for looped samples
	if (BASS_ChannelIsActive(SoundSlot[slot].channel))
		SoundSlot[slot].state = Idle;

	return true;
}

bool SoundSystem::UpdateEffectAttributes(int slot, float pitch, float gain)
{
	if (slot > SOUND_MAX_CHANNELS || slot < 0)
		return false;

	BASS_ChannelSetAttribute(SoundSlot[slot].channel, BASS_ATTRIB_FREQ, 22050.0f * pitch);
	BASS_ChannelSetAttribute(SoundSlot[slot].channel, BASS_ATTRIB_VOL, gain);

	return true;
}

bool SoundSystem::CheckBASSError(const char* message, bool verbose, ...)
{
	va_list argptr;
	static char data[4096];

	int bassError = BASS_ErrorGetCode();
	if (verbose || bassError)
	{
		va_start(argptr, verbose);
		int32_t written = vsprintf(data, (char*)message, argptr);	// @TODO: replace with debug/console/message output later...
		va_end(argptr);
		snprintf(data + written, sizeof(data) - written, bassError ? ": error #%d" : ": success", bassError);
		Log(bassError ? 2 : 1, data);
	}
	return bassError != 0;
}

int SoundSystem::GetFreeSlot()
{
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		if (SoundSlot[i].channel == NULL || !BASS_ChannelIsActive(SoundSlot[i].channel))
			return i;
	}

	// No free slots, hijack now.
	float minDistance = 0;
	int farSlot = -1;
	for (int i = 0; i < SOUND_MAX_CHANNELS; i++)
	{
		float distance = Vector3(SoundSlot[i].origin - Vector3(camera.mike_pos.x, camera.mike_pos.y, camera.mike_pos.z)).Length();
		if (distance > minDistance)
		{
			minDistance = distance;
			farSlot = i;
		}
	}

	Log(1, "Hijacking sound effect slot: %d", farSlot);
	FreeSlot(farSlot, SOUND_XFADETIME_HIJACKSOUND);
	return farSlot;
}

void SoundSystem::FreeSlot(int slot, unsigned int fade)
{
	if (slot > SOUND_MAX_CHANNELS || slot < 0)
		return;

	if (SoundSlot[slot].channel != NULL && BASS_ChannelIsActive(SoundSlot[slot].channel))
	{
		if (fade > 0)
			BASS_ChannelSlideAttribute(SoundSlot[slot].channel, BASS_ATTRIB_VOL, -1.0f, fade);
		else
			BASS_ChannelStop(SoundSlot[slot].channel);
	}

	SoundSlot[slot].channel = NULL;
	SoundSlot[slot].state = Idle;
	SoundSlot[slot].sample_index = -1;
}

void SoundSystem::FreeSample(int index)
{
	if (SamplePointer[index] != NULL)
	{
		BASS_SampleFree(SamplePointer[index]);
		SamplePointer[index] = NULL;
	}
}

float SoundSystem::DistanceToListener(PHD_3DPOS* position)
{
	if (position == NULL)
		return 0.0f;
	return DistanceToListener(Vector3(position->x_pos, position->y_pos, position->z_pos));
}

float SoundSystem::DistanceToListener(Vector3 position)
{
	return Vector3(Vector3(camera.mike_pos.x, camera.mike_pos.y, camera.mike_pos.z) - position).Length();
}

float SoundSystem::Attenuate(float gain, float distance, float radius)
{
	float result = gain * (1.0f - (distance / radius));
	result = result < 0 ? 0.0f : (result > 1.0f ? 1.0f : result);
	return result * ((float)SFXVolume / 100.0f);
}
