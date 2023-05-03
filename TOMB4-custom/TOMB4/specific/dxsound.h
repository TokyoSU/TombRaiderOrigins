#pragma once
#include "../global/types.h"
#include "../game/sound.h"

constexpr auto SOUND_BASS_UNITS = 1.0f / 1024.0f; // TR->BASS distance unit coefficient
constexpr auto SOUND_MAX_PITCH_CHANGE = 0.09f;
constexpr auto SOUND_MAX_GAIN_CHANGE = 0.0625f;
constexpr auto SOUND_MAXVOL_RADIUS = 1024.0f;
constexpr auto SOUND_32BIT_SILENCE_LEVEL = 4.9e-04f;

enum SoundFilter
{
	SF_Reverb,
	SF_Compressor,
	SF_Lowpass,
	SF_Count
};

enum ReverbType
{
	RT_Outside,   // 0x00   no reverberation
	RT_Small,	  // 0x01   little reverberation
	RT_Medium,    // 0x02
	RT_Large,	  // 0x03
	RT_Pipe,	  // 0x04   highest reverberation, almost never used
	RT_Count
};

bool DXDSCreate();
bool DXCreateSampleADPCM(char* data, long comp_size, long uncomp_size, long num);
void DXFreeSounds();
void S_SetReverbType(long reverb);
void DXDSClose();

extern HSAMPLE SamplePointer[SFX_SAMPLES_COUNT];
extern char* samples_buffer;
