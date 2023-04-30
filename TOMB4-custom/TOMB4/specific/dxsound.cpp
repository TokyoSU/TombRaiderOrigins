#include "../tomb4/pch.h"
#include "dxsound.h"
#include "dxshell.h"
#include "function_stubs.h"
#include "audio.h"
#include "../game/sound.h"
#include "LoadSave.h"
#include "winmain.h"
#include "../tomb4/tomb4.h"

#define SOUND_32BIT_SILENCE_LEVEL 4.9e-04f

char* samples_buffer;
HSAMPLE SamplePointer[NumSamples];

bool DXChangeOutputFormat(long nSamplesPerSec, bool force)
{
	return 1;
}

void DSChangeVolume(long num, long volume)
{
}

void DSAdjustPitch(long num, long pitch)
{
}

void DSAdjustPan(long num, long pan)
{
}

bool DXSetOutputFormat()
{
	Log(1, "Can't Get Primary Sound Buffer");
	return 0;
}

bool DXDSCreate()
{
	Log(2, "DXDSCreate");
	sound_active = BASS_Init(-1, 22050, BASS_DEVICE_16BITS|BASS_DEVICE_FREQ, App.hWnd, NULL);
	Log(2, sound_active ? "DXDSCreate Success" : "DXDSCreate Failed");
	return 1;
}

bool DXCreateSampleADPCM(char* data, long comp_size, long uncomp_size, long num)
{
	if (num >= NumSamples)
	{
		Log(1, "DXCreateSampleADPCM failed, sample %d is more than the max samples supported: %d", num, NumSamples);
		return 0;
	}
	if (data == nullptr || comp_size <= 0)
	{
		Log(1, "DXCreateSampleADPCM failed, sample %d data is null or comp_size is <= 0", num);
		return 0;
	}

	HSAMPLE sample = BASS_SampleLoad(true, data, 0, comp_size, 1, BASS_SAMPLE_MONO|BASS_SAMPLE_FLOAT);
	if (!sample)
	{
		Log(1, "DXCreateSampleADPCM failed, failed to load sample: %d", num);
		return 0;
	}

	BASS_SAMPLE info;
	BASS_SampleGetInfo(sample, &info);
	int finalLength = info.length + 44;
	if (info.freq != 22050 || info.chans != 1)
	{
		Log(1, "DXCreateSampleADPCM failed, failed to load sample: %d, Wrong sample parameters, must be 22050 Hz Mono", num);
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
	BASS_SampleGetData(sample, uncompBuffer + 44);
	BASS_SampleFree(sample);

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
	SamplePointer[num] = BASS_SampleLoad(true, uncompBuffer, 0, cleanLength + 44, 65535, BASS_SAMPLE_MONO|BASS_SAMPLE_FLOAT|BASS_SAMPLE_3D);
	delete[] uncompBuffer;

	return 1;
}

void DXStopSample(long num)
{
	
}

bool DSIsChannelPlaying(long num)
{
	return 0;
}

long DSGetFreeChannel()
{
	return -1;
}

long CalcVolume(long volume)
{
	long result = 8000 - long(float(0x7FFF - volume) * 0.30518511F);
	if (result > 0)
		result = 0;
	else if (result < -10000)
		result = -10000;

	result -= (100 - SFXVolume) * 50;

	if (result > 0)
		result = 0;

	if (result < -10000)
		result = -10000;

	return result;
}

void S_SoundStopAllSamples()
{
	for (int i = 0; i < 32; i++)
		DXStopSample(i);
}

void S_SoundStopSample(long num)
{
	DXStopSample(num);
}

void DXFreeSounds()
{
	S_SoundStopAllSamples();

	for (int i = 0; i < NumSamples; i++)
	{
		BASS_SampleFree(SamplePointer[i]);
	}
}

long S_SoundSampleIsPlaying(long num)
{
	if (sound_active && DSIsChannelPlaying(num))
		return 1;
	return 0;
}

void S_SoundSetPanAndVolume(long num, short pan, ushort volume)
{
	if (sound_active)
	{
		DSChangeVolume(num, CalcVolume(volume));
		DSAdjustPan(num, pan);
	}
}

void S_SoundSetPitch(long num, long pitch)
{
	if (sound_active)
		DSAdjustPitch(num, pitch);
}

void S_SetReverbType(long reverb)
{
	if (App.SoundDisabled)
		return;

}

void DXDSClose()
{
	if (App.SoundDisabled)
		return;
	Log(2, "DXDSClose");
	sound_active = !BASS_Free();
}
