#include "pch.h"
#include "fmv.h"
#include "dxshell.h"
#include "audio.h"
#include "lighting.h"
#include "function_table.h"
#include "winmain.h"
#include "input.h"
#include "3dmath.h"
#include "../game/text.h"
#include "d3dmatrix.h"
#include "dxsound.h"
#include "../game/control.h"
#include "cmdline.h"
#include "gamemain.h"
#include "LoadSave.h"
#include "function_stubs.h"
#include "output.h"
#include "time.h"
#pragma warning(push)
#pragma warning(disable: 4244)
#define PL_MPEG_IMPLEMENTATION
#include "../tomb4/pl_mpeg.h"
#pragma warning(pop)
#include <sokol_time.h>

static unsigned char* FmvData;
static HSTREAM FmvStream = 0;
static uint64_t timer;

static void FmvVideo(plm_t* self, plm_frame_t* frame, void* user)
{
	plm_frame_to_rgb(frame, FmvData, frame->width * 3);

}

static void FmvAudio(plm_t* self, plm_samples_t* samples, void* user)
{
	if (FmvStream != NULL)
		BASS_StreamPutData(FmvStream, samples->interleaved, sizeof(float) * samples->count * 2);
}

long PlayFmvNow(long num)
{
	if (MainThread.ended)
		return 0;
	if (fmvs_disabled)
		return 0;
	if ((1 << num) & FmvSceneTriggered)
		return 1;
	FmvSceneTriggered |= 1 << num;
	S_CDStop();

	char name[80];
	char path[80];
	sprintf(name, "fmv/fmv%02d.mpg", num);
	memset(path, 0, sizeof(path));
	strcat(path, name);

	Log(2, "Fmv path: %s", path);
	plm_t* file = plm_create_with_filename(path);
	if (file == NULL)
		return 0;
	int samplerate = plm_get_samplerate(file);
	plm_set_video_decode_callback(file, FmvVideo, NULL);
	plm_set_audio_decode_callback(file, FmvAudio, NULL);
	plm_set_loop(file, FALSE);
	plm_set_audio_enabled(file, TRUE);
	plm_set_audio_stream(file, 0);

	if (plm_get_num_audio_streams(file) > 0)
	{
		FmvStream = BASS_StreamCreate(44100, 2, BASS_SAMPLE_FLOAT, STREAMPROC_PUSH, NULL);
		if (FmvStream != NULL)
		{
			BASS_ChannelPlay(FmvStream, FALSE);
		}
		else
		{
			Log(1, "PlayFmvNow, failed to setup a sound stream !");
		}
		plm_set_audio_lead_time(file, 4096.0 / (double)samplerate);
	}

	int num_pixels = plm_get_width(file) * plm_get_height(file);
	FmvData = (unsigned char*)malloc(num_pixels * 3);

	HWInitialise();
	ClearSurfaces();

	App.fmv = true;
	S_UpdateInput();
	while (!plm_has_ended(file))
	{
		if (input & IN_OPTION || MainThread.ended)
			break;
		double frame = stm_sec(stm_laptime(&timer));
		plm_decode(file, frame);
		S_UpdateInput();
		S_DumpScreen();
	}

	if (file != NULL)
	{
		plm_destroy(file);
		file = NULL;
	}

	if (FmvData != NULL)
	{
		free(FmvData);
		FmvData = NULL;
	}

	if (FmvStream != NULL)
	{
		BASS_StreamFree(FmvStream);
		FmvStream = NULL;
	}

	HWInitialise();
	ClearSurfaces();
	App.fmv = false;
	return 0;
}
