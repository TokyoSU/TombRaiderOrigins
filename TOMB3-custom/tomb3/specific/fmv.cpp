#include "pch.h"
#include "fmv.h"
#include "specific.h"
#include "winmain.h"
#include "file.h"
#include "input.h"
#include "audio.h"
#include "../tomb3/tomb3.h"
#include "newstuff/pl_mpeg.h"

long fmv_playing = 0;
plm_t* plm = NULL;
unsigned char* rgb_data = NULL;

static void fmv_video(plm_t* self, plm_frame_t* frame, void* user)
{
	//plm_frame_to_rgb(frame, rgb_data, frame->width * 3);

}

static void fmv_audio(plm_t* self, plm_samples_t* samples, void* user)
{

}

long FMV_Play(char* name)
{
	fmv_playing = 1;
	S_CDStop();
	ShowCursor(0);
	WinFreeDX(0);
	WinPlayFMV(GetFullPath(name), 1);
	WinStopFMV(1);
	fmv_playing = 0;

	if (!GtWindowClosed)
		WinDXInit(&App.DeviceInfo, &App.DXConfig, 0);

	ShowCursor(1);
	return GtWindowClosed;
}

long FMV_PlayIntro(char* name1, char* name2)
{
	fmv_playing = 1;
	ShowCursor(0);
	WinFreeDX(0);
	WinPlayFMV(GetFullPath(name1), 1);
	WinStopFMV(1);
	WinPlayFMV(GetFullPath(name2), 1);
	WinStopFMV(1);
	fmv_playing = 0;

	if (!GtWindowClosed)
		WinDXInit(&App.DeviceInfo, &App.DXConfig, 0);

	ShowCursor(1);
	return GtWindowClosed;
}

void WinPlayFMV(const char* name, bool play)
{
	if (!DoFileExist(name))
		return;
	auto* plm = plm_create_with_filename(name);
	int samplerate = plm_get_samplerate(plm);
	plm_set_video_decode_callback(plm, fmv_video, NULL);
	plm_set_audio_decode_callback(plm, fmv_audio, NULL);
	plm_set_loop(plm, FALSE);
	plm_set_audio_enabled(plm, TRUE);
	plm_set_audio_stream(plm, 0);

	if (plm_get_num_audio_streams(plm) > 0) {

	}

	int num_pixels = plm_get_width(plm) * plm_get_height(plm);
	rgb_data = (unsigned char*)malloc(num_pixels * 3);

	while (!plm_has_ended(plm) && !GtWindowClosed)
	{
		plm_decode(plm, WinFrameRate());
		if (S_UpdateInput())
			break;
		if (input & IN_OPTION)
			break;
	}
}

void WinStopFMV(bool play)
{
	if (plm != NULL)
	{
		plm_destroy(plm);
		plm = NULL;
	}
	
	if (rgb_data != NULL)
	{
		free(rgb_data);
		rgb_data = NULL;
	}
}
