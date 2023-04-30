#pragma once
#include "../global/types.h"
#include "../game/sound.h"
#include "../tomb4/bass.h"

bool DXChangeOutputFormat(long nSamplesPerSec, bool force);
void DSChangeVolume(long num, long volume);
void DSAdjustPitch(long num, long pitch);
void DSAdjustPan(long num, long pan);
bool DXSetOutputFormat();
bool DXDSCreate();
bool DXCreateSampleADPCM(char* data, long comp_size, long uncomp_size, long num);
void DXStopSample(long num);
bool DSIsChannelPlaying(long num);
long DSGetFreeChannel();
long CalcVolume(long volume);
void S_SoundStopAllSamples();
void S_SoundStopSample(long num);
void DXFreeSounds();
long S_SoundSampleIsPlaying(long num);
void S_SoundSetPanAndVolume(long num, short pan, ushort volume);
void S_SoundSetPitch(long num, long pitch);
void S_SetReverbType(long reverb);
void DXDSClose();

extern HSAMPLE SamplePointer[NumSamples];
extern char* samples_buffer;
