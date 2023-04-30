#pragma once
#include "../global/types.h"
#include "../game/sound.h"

constexpr auto SOUND_BASS_UNITS = 1.0f / 1024.0f; // TR->BASS distance unit coefficient
constexpr auto SOUND_MAX_PITCH_CHANGE = 0.09f;
constexpr auto SOUND_MAX_GAIN_CHANGE = 0.0625f;
constexpr auto SOUND_MAXVOL_RADIUS = 1024.0f;
constexpr auto SOUND_32BIT_SILENCE_LEVEL = 4.9e-04f;

bool DXDSCreate();
bool DXCreateSampleADPCM(char* data, long comp_size, long uncomp_size, long num);
void DXFreeSounds();
void S_SetReverbType(long reverb);
void DXDSClose();

extern HSAMPLE SamplePointer[NumSamples];
extern char* samples_buffer;
