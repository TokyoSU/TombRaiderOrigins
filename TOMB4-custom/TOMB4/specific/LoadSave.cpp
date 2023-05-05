#include "pch.h"
#include "LoadSave.h"
#include "../game/text.h"
#include "../game/sound.h"
#include "audio.h"
#include "input.h"
#include "function_table.h"
#include "drawroom.h"
#include "polyinsert.h"
#include "winmain.h"
#include "output.h"
#include "../game/gameflow.h"
#include "../game/savegame.h"
#include "gamemain.h"
#include "specificfx.h"
#include "time.h"
#include "dxshell.h"
#include "function_stubs.h"
#include "texture.h"
#include "../game/newinv.h"
#include "../game/camera.h"
#include "3dmath.h"
#include "../game/control.h"
#include "../game/lara.h"
#include "../tomb4/tomb4.h"
#include "../tomb4/troyestuff.h"
#include "drawbars.h"

long MusicVolume = 40;
long SFXVolume = 80;
long ControlMethod;
char MonoScreenOn;

static MONOSCREEN_STRUCT MonoScreen[5];
static SAVEFILE_INFO SaveGames[15];

void DoOptions()
{
	JOYINFOEX joy;
	char** keyboard_buttons;
	char* txt;
	static long menu;
	static unsigned long sel = 1;	//selection
	static unsigned long sel2;		//selection for when mapping keys
	static long mSliderCol = 0xFF3F3F3F;
	static long sSliderCol = 0xFF3F3F3F;
	static long sfx_bak;
	static long sfx_quality_bak;
	static long sfx_breath_db = -1;
	unsigned long nMask;
	long f, y, i, jread, jx, jy, lp;
	static char sfx_backup_flag;	//have we backed sfx stuff up?
	static bool waiting_for_key = 0;

	if (!(sfx_backup_flag & 1))
	{
		sfx_backup_flag |= 1;
		sfx_bak = SFXVolume;
	}

	f = font_height - 4;

	if (menu)	//controls menu
	{
		if (menu == 200)
		{
			TroyeMenu(f, menu, sel);
			return;
		}

		if (Gameflow->Language == GERMAN)
			keyboard_buttons = (char**)GermanKeyboard;
		else
			keyboard_buttons = (char**)KeyboardButtons;

		nMask = 17;
		small_font = 1;
		PrintString(phd_centerx >> 2, f, sel & 1 ? 1 : 2, SCRIPT_TEXT(TXT_Control_Method), 0);

		y = 1;
		i = 1;

		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, "\x18", 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, "\x1A", 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, "\x19", 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, "\x1B", 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Duck), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Dash), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Walk), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Jump), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Action), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Draw_Weapon), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Use_Flare), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Look), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Roll), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Inventory), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Step_Left), 0);
		PrintString(phd_centerx >> 2, f + y++ * font_height, sel & (1 << i++) ? 1 : 2, SCRIPT_TEXT(TXT_Step_Right), 0);

		if (!ControlMethod)
			PrintString(phd_centerx + (phd_centerx >> 2), f, sel2 & 1 ? 1 : 6, SCRIPT_TEXT(TXT_Keyboard), 0);
		else if (ControlMethod == 1)
			PrintString(phd_centerx + (phd_centerx >> 2), f, sel2 & 1 ? 1 : 6, SCRIPT_TEXT(TXT_Joystick), 0);
		else if (ControlMethod == 2)
			PrintString(phd_centerx + (phd_centerx >> 2), f, sel2 & 1 ? 1 : 6, SCRIPT_TEXT(TXT_Reset), 0);

		y = 1;
		i = 1;

		for (lp = 0; lp < 16; lp++)
		{
			txt = (waiting_for_key && sel2 & (1 << i)) ? SCRIPT_TEXT(TXT_Waiting) : keyboard_buttons[layout[1][lp]];
			PrintString(phd_centerx + (phd_centerx >> 2), f + y++ * font_height, sel2 & (1 << i++) ? 1 : 6, txt, 0);
		}

		small_font = 0;

		if (ControlMethod < 2 && !waiting_for_key)
		{
			if (dbinput & IN_FORWARD)
			{
				Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
				sel >>= 1;
			}

			if (dbinput & IN_BACK)
			{
				Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
				sel <<= 1;
			}
		}

		if (waiting_for_key)
		{
			i = 0;

			if (keymap[DIK_ESCAPE])
			{
				Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
				sel2 = 0;
				dbinput = 0;
				waiting_for_key = 0;
				return;
			}

			for (lp = 0; lp < 255; lp++)
			{
				if (keymap[lp] && keyboard_buttons[lp])
				{
					if (lp != DIK_RETURN && lp != DIK_LEFT && lp != DIK_RIGHT && lp != DIK_UP && lp != DIK_DOWN)
					{
						waiting_for_key = 0;

						sel2 >>= 2;

						while (sel2)
						{
							i++;
							sel2 >>= 1;
						}

						sel2 = 0;
						layout[1][i] = (short)lp;
					}
				}
			}

			if (ControlMethod == 1)
			{
				jread = ReadJoystick(jx, jy);

				if (jread)
				{
					i = 0;
					sel2 >>= 2;

					while (sel2)
					{
						i++;
						sel2 >>= 1;
					}

					sel2 = 0;
					lp = 0;

					while (jread)
					{
						jread >>= 1;
						lp++;
					}

					layout[1][i] = short(lp + 255);
					waiting_for_key = 0;
				}
			}

			CheckKeyConflicts();
			dbinput = 0;
		}

		if (dbinput & IN_SELECT && sel > 1 && ControlMethod < 2)
		{
			Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
			sel2 = sel;
			waiting_for_key = 1;
			memset(keymap, 0, sizeof(keymap));
		}

		if (dbinput & IN_SELECT && ControlMethod == 2)
		{
			Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
			ControlMethod = 0;
			memcpy(layout[1], layout, 72);
		}

		if (sel & 1)
		{
			if (dbinput & IN_LEFT)
			{
				Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
				ControlMethod--;
			}

			if (dbinput & IN_RIGHT)
			{
				Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
				ControlMethod++;
			}

			if (ControlMethod > 2)
				ControlMethod = 2;

			if (ControlMethod < 0)
				ControlMethod = 0;

			if (ControlMethod == 1)
			{
				joy.dwSize = sizeof(JOYINFOEX);

				if (joyGetPosEx(0, &joy) == JOYERR_UNPLUGGED)
				{
					if (dbinput & IN_LEFT)
						ControlMethod = 0;

					if (dbinput & IN_RIGHT)
						ControlMethod = 2;
				}
			}
		}

		if (!sel)
			sel = 1;

		if (sel > unsigned long(1 << (nMask - 1)))
			sel = 1 << (nMask - 1);

		if (dbinput & IN_DESELECT)
		{
			Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);

			if (ControlMethod < 2)
				menu = 0;

			dbinput = 0;
			sel = 1;
		}
	}
	else	//'main' menu
	{
		nMask = 6;
		f = 3 * font_height;
		PrintString(phd_centerx, f, 6, SCRIPT_TEXT(TXT_Options), FF_CENTER);
		PrintString(phd_centerx, f + font_height + (font_height >> 1), sel & 1 ? 1 : 2, SCRIPT_TEXT(TXT_Control_Configuration), FF_CENTER);
		PrintString(phd_centerx >> 2, f + 3 * font_height, sel & 0x2 ? 1 : 2, SCRIPT_TEXT(TXT_Music_Volume), 0);
		PrintString(phd_centerx >> 2, f + 4 * font_height, sel & 0x4 ? 1 : 2, SCRIPT_TEXT(TXT_SFX_Volume), 0);
		PrintString(phd_centerx >> 2, f + 5 * font_height, sel & 0x8 ? 1 : 2, SCRIPT_TEXT(TXT_Sound_Quality), 0);
		PrintString(phd_centerx >> 2, f + 6 * font_height, sel & 0x10 ? 1 : 2, SCRIPT_TEXT(TXT_Targeting), 0);
		DoSlider(400, 3 * font_height - (font_height >> 1) + f + 4, 200, 16, MusicVolume, 0xFF1F1F1F, 0xFF3F3FFF, mSliderCol);
		DoSlider(400, f + 4 * font_height + 4 - (font_height >> 1), 200, 16, SFXVolume, 0xFF1F1F1F, 0xFF3F3FFF, sSliderCol);

		if (App.AutoTarget)
			PrintString(phd_centerx + (phd_centerx >> 2), f + 6 * font_height, sel & 0x10 ? 1 : 6, SCRIPT_TEXT(TXT_Automatic), 0);
		else
			PrintString(phd_centerx + (phd_centerx >> 2), f + 6 * font_height, sel & 0x10 ? 1 : 6, SCRIPT_TEXT(TXT_Manual), 0);

		PrintString(phd_centerx, (font_height >> 1) + f + 7 * font_height, sel & 0x20 ? 1 : 2, "tomb4 options", FF_CENTER);

		if (dbinput & IN_FORWARD)
		{
			Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
			sel >>= 1;
		}

		if (dbinput & IN_BACK)
		{
			Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
			sel <<= 1;
		}

		if (dbinput & IN_SELECT && sel & 1)
		{
			Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
			menu = 1;
		}

		if (dbinput & IN_SELECT && sel & 0x20)
		{
			Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
			sel = 1;
			menu = 200;
		}

		if (!sel)
			sel = 1;

		if (sel > unsigned long(1 << (nMask - 1)))
			sel = 1 << (nMask - 1);

		mSliderCol = 0xFF3F3F3F;
		sSliderCol = 0xFF3F3F3F;

		if (sel & 2)
		{
			sfx_bak = SFXVolume;

			if (linput & IN_LEFT)
				MusicVolume--;

			if (linput & IN_RIGHT)
				MusicVolume++;

			if (MusicVolume > 100)
				MusicVolume = 100;
			else if (MusicVolume < 0)
				MusicVolume = 0;

			sSliderCol = 0xFF3F3F3F;
			mSliderCol = 0xFF7F7F7F;
			ACMSetVolume();
		}
		else if (sel & 4)
		{
			if (linput & IN_LEFT)
				SFXVolume--;
			else if (linput & IN_RIGHT)
				SFXVolume++;

			if (SFXVolume > 100)
				SFXVolume = 100;
			else if (SFXVolume < 0)
				SFXVolume = 0;

			if (SFXVolume != sfx_bak)
			{
				// TODO: fix the sound here !
				if (sfx_breath_db == -1/* || !SOUND_EffectIsPlaying(0)*/)
				{
					Sound.StopAllEffects();
					sfx_bak = SFXVolume;
					sfx_breath_db = Sound.PlayEffect(SFX_LARA_BREATH, NULL, SFXO_ALWAYS);
				}
				//else if (sfx_breath_db != -1 && SOUND_EffectIsPlaying(0))
					//DSChangeVolume(0, -100 * ((100 - SFXVolume) >> 1));
			}

			mSliderCol = 0xFF3F3F3F;
			sSliderCol = 0xFF7F7F7F;
		}
		else if (sel & 16)
		{
			if (dbinput & IN_LEFT)
			{
				if (App.AutoTarget)
					App.AutoTarget = 0;

				Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
			}

			if (dbinput & IN_RIGHT)
			{
				if (!App.AutoTarget)
					App.AutoTarget = 1;

				Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
			}

			savegame.AutoTarget = App.AutoTarget;
		}
	}
}

void DisplayStatsUCunt()
{
	long sec, days, hours, min, y;
	char buf[40];

	y = phd_centery - (font_height << 2);
	PrintString(phd_centerx, y, 6, SCRIPT_TEXT(TXT_Statistics), FF_CENTER);
	PrintString(phd_centerx, y + 2 * font_height, 2, SCRIPT_TEXT(gfLevelNames[gfCurrentLevel]), FF_CENTER);
	PrintString(phd_centerx >> 2, y + 3 * font_height, 2, SCRIPT_TEXT(TXT_Time_Taken), 0);
	PrintString(phd_centerx >> 2, y + 4 * font_height, 2, SCRIPT_TEXT(TXT_Distance_Travelled), 0);
	PrintString(phd_centerx >> 2, y + 5 * font_height, 2, SCRIPT_TEXT(TXT_Ammo_Used), 0);
	PrintString(phd_centerx >> 2, y + 6 * font_height, 2, SCRIPT_TEXT(TXT_Health_Packs_Used), 0);
	PrintString(phd_centerx >> 2, y + 7 * font_height, 2, SCRIPT_TEXT(TXT_Secrets_Found), 0);

	sec = GameTimer / 30;
	days = sec / 86400;
	hours = (sec % 86400) / 3600;
	min = (sec / 60) % 60;
	sec = (sec % 60);

	sprintf(buf, "%02d:%02d:%02d", (days * 24) + hours, min, sec);
	PrintString(phd_centerx + (phd_centerx >> 2), y + 3 * font_height, 6, buf, 0);

	sprintf(buf, "%dm", savegame.Game.Distance / 419);
	PrintString(phd_centerx + (phd_centerx >> 2), y + 4 * font_height, 6, buf, 0);

	sprintf(buf, "%d", savegame.Game.AmmoUsed);
	PrintString(phd_centerx + (phd_centerx >> 2), y + 5 * font_height, 6, buf, 0);

	sprintf(buf, "%d", savegame.Game.HealthUsed);
	PrintString(phd_centerx + (phd_centerx >> 2), y + 6 * font_height, 6, buf, 0);

	sprintf(buf, "%d / 70", savegame.Game.Secrets);
	PrintString(phd_centerx + (phd_centerx >> 2), y + 7 * font_height, 6, buf, 0);
}

long S_DisplayPauseMenu(long reset)
{
	static long menu, selection = 1;
	long y;

	if (!menu)
	{
		if (reset)
		{
			selection = reset;
			menu = 0;
		}
		else
		{
			y = phd_centery - font_height;
			PrintString(phd_centerx, y - ((3 * font_height) >> 1), 6, SCRIPT_TEXT(TXT_Paused), FF_CENTER);
			PrintString(phd_centerx, y, selection & 1 ? 1 : 2, SCRIPT_TEXT(TXT_Statistics), FF_CENTER);
			PrintString(phd_centerx, y + font_height, selection & 2 ? 1 : 2, SCRIPT_TEXT(TXT_Options), FF_CENTER);
			PrintString(phd_centerx, y + 2 * font_height, selection & 4 ? 1 : 2, SCRIPT_TEXT(TXT_Exit_to_Title), FF_CENTER);

			if (dbinput & IN_FORWARD)
			{
				if (selection > 1)
					selection >>= 1;

				Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
			}

			if (dbinput & IN_BACK)
			{
				if (selection < 4)
					selection <<= 1;

				Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
			}

			if (dbinput & IN_DESELECT)
			{
				Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
				return 1;
			}

			if (dbinput & IN_SELECT && !keymap[DIK_LALT])
			{
				Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);

				if (selection & 1)
					menu = 2;
				else if (selection & 2)
					menu = 1;
				else if (selection & 4)
					return 8;
			}
		}
	}
	else if (menu == 1)
	{
		DoOptions();

		if (dbinput & IN_DESELECT)
		{
			menu = 0;
			Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
		}
	}
	else if (menu == 2)
	{
		DisplayStatsUCunt();

		if (dbinput & IN_DESELECT)
		{
			menu = 0;
			Sound.PlayEffect(SFX_MENU_SELECT, NULL, SFXO_ALWAYS);
		}
	}

	return 0;
}

long DoLoadSave(long LoadSave)
{
	SAVEFILE_INFO* pSave;
	static long selection;
	long txt, l;
	unsigned char color;
	char string[80];
	char name[41];

	if (LoadSave & IN_SAVE)
		txt = TXT_Save_Game;
	else
		txt = TXT_Load_Game;

	PrintString(phd_centerx, font_height, 6, SCRIPT_TEXT(txt), FF_CENTER);

	for (int i = 0; i < 15; i++)
	{
		pSave = &SaveGames[i];
		color = 2;

		if (i == selection)
			color = 1;

		memset(name, ' ', 40);
		l = strlen(pSave->name);

		if (l > 40)
			l = 40;

		strncpy(name, pSave->name, l);
		name[40] = 0;
		small_font = 1;

		if (pSave->valid)
		{
			wsprintf(string, "%03d", pSave->num);
			PrintString(phd_centerx - long((float)phd_winwidth / 640.0F * 310.0), font_height + font_height * (i + 2), color, string, 0);
			PrintString(phd_centerx - long((float)phd_winwidth / 640.0F * 270.0), font_height + font_height * (i + 2), color, name, 0);
			wsprintf(string, "%d %s %02d:%02d:%02d", pSave->days, SCRIPT_TEXT(TXT_days), pSave->hours, pSave->minutes, pSave->seconds);
			PrintString(phd_centerx - long((float)phd_winwidth / 640.0F * -135.0), font_height + font_height * (i + 2), color, string, 0);
		}
		else
		{
			wsprintf(string, "%s", pSave->name);
			PrintString(phd_centerx, font_height + font_height * (i + 2), color, string, FF_CENTER);
		}

		small_font = 0;
	}

	if (dbinput & IN_FORWARD)
	{
		selection--;
		Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
	}

	if (dbinput & IN_BACK)
	{
		selection++;
		Sound.PlayEffect(SFX_MENU_CHOOSE, NULL, SFXO_ALWAYS);
	}

	if (selection < 0)
		selection = 0;
	else if (selection > 14)
		selection = 14;

	if (dbinput & IN_SELECT)
	{
		if (SaveGames[selection].valid || LoadSave == IN_SAVE)
			return selection;
		Sound.SayNo();
	}

	return -1;
}

long S_LoadSave(long load_or_save, long mono, long inv_active)
{
	long fade, ret;

	fade = 0;

	if (!mono)
		CreateMonoScreen();

	GetSaveLoadFiles();

	if (!inv_active)
		InventoryActive = 1;

	while (1)
	{
		S_InitialisePolyList();

		if (fade)
			dbinput = 0;
		else
			S_UpdateInput();

		SetDebounce = 1;
		S_DisplayMonoScreen();
		ret = DoLoadSave(load_or_save);
		UpdatePulseColour();
		S_OutputPolyList();
		S_DumpScreen(G_dxptr->rViewport);

		if (ret >= 0)
		{
			if (load_or_save & IN_SAVE)
			{
				sgSaveGame();
				S_SaveGame(ret);
				GetSaveLoadFiles();
				break;
			}

			fade = ret + 1;
			S_LoadGame(ret);

			if (!DeathMenuActive)
				SetFade(0, 255);

			ret = -1;
		}

		if (fade && DoFade == 2)
		{
			ret = fade - 1;
			break;
		}

		if (input & IN_OPTION)
		{
			ret = -1;
			break;
		}

		if (MainThread.ended)
			break;
	}

	TIME_Init();

	if (!mono)
		FreeMonoScreen();

	if (!inv_active)
		InventoryActive = 0;

	return ret;
}

void S_DrawTile(long x, long y, long w, long h, LPDIRECT3DTEXTUREX t, long tU, long tV, long tW, long tH, long c0, long c1, long c2, long c3)
{
	D3DTLBUMPVERTEX v[4];
	D3DTLBUMPVERTEX tri[3];
	float u1, v1, u2, v2;

	u1 = float(tU * (1.0F / 256.0F));
	v1 = float(tV * (1.0F / 256.0F));
	u2 = float((tW + tU) * (1.0F / 256.0F));
	v2 = float((tH + tV) * (1.0F / 256.0F));

	v[0].sx = (float)x;
	v[0].sy = (float)y;
	v[0].sz = 0.995F;
	v[0].tu = u1;
	v[0].tv = v1;
	v[0].rhw = 1;
	v[0].color = c0;
	v[0].specular = 0xFF000000;

	v[1].sx = float(w + x);
	v[1].sy = (float)y;
	v[1].sz = 0.995F;
	v[1].tu = u2;
	v[1].tv = v1;
	v[1].rhw = 1;
	v[1].color = c1;
	v[1].specular = 0xFF000000;

	v[2].sx = float(w + x);
	v[2].sy = float(h + y);
	v[2].sz = 0.995F;
	v[2].tu = u2;
	v[2].tv = v2;
	v[2].rhw = 1;
	v[2].color = c3;
	v[2].specular = 0xFF000000;

	v[3].sx = (float)x;
	v[3].sy = float(h + y);
	v[3].sz = 0.995F;
	v[3].tu = u1;
	v[3].tv = v2;
	v[3].rhw = 1;
	v[3].color = c2;
	v[3].specular = 0xFF000000;

	App.dx.lpD3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);
	App.dx.lpD3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFG_POINT);
	App.dx.lpD3DDevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 0);
	DXAttempt(App.dx.lpD3DDevice->SetTexture(0, t));
	tri[0] = v[0];
	tri[1] = v[2];
	tri[2] = v[3];
	App.dx.lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, FVF, v, 3, D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS);
	App.dx.lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, FVF, tri, 3, D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS);
	App.dx.lpD3DDevice->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 1);

	if (App.Filtering)
	{
		App.dx.lpD3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
		App.dx.lpD3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFG_LINEAR);
	}
}

void S_DisplayMonoScreen()
{
	long x[4];
	long y[4];
	unsigned long col;

	x[0] = phd_winxmin;
	y[0] = phd_winymin;
	x[1] = phd_winxmin + phd_winwidth;
	y[1] = phd_winymin + phd_winheight;

	if (tomb4.inv_bg_mode == 1 || tomb4.inv_bg_mode == 3)
		col = 0xFFFFFFFF;
	else
		col = 0xFFFFFF80;

	S_DrawTile(x[0], y[0], x[1] - x[0], y[1] - y[0], MonoScreen[0].tex, 0, 0, 256, 256, col, col, col, col);
}

void CreateMonoScreen()
{
	MonoScreenOn = 1;
	ConvertSurfaceToTextures(App.dx.lpBackBuffer);
}

void FreeMonoScreen()
{
	if (MonoScreen[0].surface)
	{
		Log(4, "Released %s @ %x - RefCnt = %d", "Mono Screen Surface", MonoScreen[0].surface, MonoScreen[0].surface->Release());
		MonoScreen[0].surface = 0;
	}
	else
		Log(1, "%s Attempt To Release NULL Ptr", "Mono Screen Surface");

	if (MonoScreen[0].tex)
	{
		Log(4, "Released %s @ %x - RefCnt = %d", "Mono Screen Texture", MonoScreen[0].tex, MonoScreen[0].tex->Release());
		MonoScreen[0].tex = 0;
	}
	else
		Log(1, "%s Attempt To Release NULL Ptr", "Mono Screen Texture");

	MonoScreenOn = 0;
}

void RGBM_Mono(unsigned char * r, unsigned char * g, unsigned char * b)
{
	unsigned char c;

	if (tomb4.inv_bg_mode != 3)
	{
		c = (*r + *b) >> 1;
		*r = c;
		*g = c;
		*b = c;
	}
}

static void BitMaskGetNumberOfBits(unsigned long bitMask, unsigned long& bitDepth, unsigned long& bitOffset)
{
	long i;

	if (!bitMask) 
	{
		bitOffset = 0;
		bitDepth = 0;
		return;
	}

	for (i = 0; !(bitMask & 1); i++)
		bitMask >>= 1;

	bitOffset = i;

	for (i = 0; bitMask != 0; i++)
		bitMask >>= 1;

	bitDepth = i;
}

static void WinVidGetColorBitMasks(COLOR_BIT_MASKS* bm, LPDDPIXELFORMAT pixelFormat)
{
	bm->dwRBitMask = pixelFormat->dwRBitMask;
	bm->dwGBitMask = pixelFormat->dwGBitMask;
	bm->dwBBitMask = pixelFormat->dwBBitMask;
	bm->dwRGBAlphaBitMask = pixelFormat->dwRGBAlphaBitMask;

	BitMaskGetNumberOfBits(bm->dwRBitMask, bm->dwRBitDepth, bm->dwRBitOffset);
	BitMaskGetNumberOfBits(bm->dwGBitMask, bm->dwGBitDepth, bm->dwGBitOffset);
	BitMaskGetNumberOfBits(bm->dwBBitMask, bm->dwBBitDepth, bm->dwBBitOffset);
	BitMaskGetNumberOfBits(bm->dwRGBAlphaBitMask, bm->dwRGBAlphaBitDepth, bm->dwRGBAlphaBitOffset);
}

static void CustomBlt(LPDDSURFACEDESCX dst, unsigned long dstX, unsigned long dstY, LPDDSURFACEDESCX src, LPRECT srcRect)
{
	COLOR_BIT_MASKS srcMask, dstMask;
	unsigned char* srcLine;
	unsigned char* dstLine;
	unsigned char* srcPtr;
	unsigned char* dstPtr;
	unsigned long srcX, srcY, width, height, srcBpp, dstBpp, color, high, low, r, g, b;

	srcX = srcRect->left;
	srcY = srcRect->top;
	width = srcRect->right - srcRect->left;
	height = srcRect->bottom - srcRect->top;
	srcBpp = src->ddpfPixelFormat.dwRGBBitCount / 8;
	dstBpp = dst->ddpfPixelFormat.dwRGBBitCount / 8;
	WinVidGetColorBitMasks(&srcMask, &src->ddpfPixelFormat);
	WinVidGetColorBitMasks(&dstMask, &dst->ddpfPixelFormat);
	srcLine = (unsigned char*)src->lpSurface + srcY * src->lPitch + srcX * srcBpp;
	dstLine = (unsigned char*)dst->lpSurface + dstY * dst->lPitch + dstX * dstBpp;

	for (unsigned long j = 0; j < height; j++) 
	{
		srcPtr = srcLine;
		dstPtr = dstLine;

		for (unsigned long i = 0; i < width; i++)
		{
			color = 0;
			memcpy(&color, srcPtr, srcBpp);
			r = (color & srcMask.dwRBitMask) >> srcMask.dwRBitOffset;
			g = (color & srcMask.dwGBitMask) >> srcMask.dwGBitOffset;
			b = (color & srcMask.dwBBitMask) >> srcMask.dwBBitOffset;

			if (srcMask.dwRBitDepth < dstMask.dwRBitDepth) 
			{
				high = dstMask.dwRBitDepth - srcMask.dwRBitDepth;
				low = (srcMask.dwRBitDepth > high) ? srcMask.dwRBitDepth - high : 0;
				r = (r << high) | (r >> low);
			}
			else if (srcMask.dwRBitDepth > dstMask.dwRBitDepth)
				r >>= srcMask.dwRBitDepth - dstMask.dwRBitDepth;

			if (srcMask.dwGBitDepth < dstMask.dwGBitDepth)
			{
				high = dstMask.dwGBitDepth - srcMask.dwGBitDepth;
				low = (srcMask.dwGBitDepth > high) ? srcMask.dwGBitDepth - high : 0;
				g = (g << high) | (g >> low);
			}
			else if (srcMask.dwGBitDepth > dstMask.dwGBitDepth)
				g >>= srcMask.dwGBitDepth - dstMask.dwGBitDepth;

			if (srcMask.dwBBitDepth < dstMask.dwBBitDepth) 
			{
				high = dstMask.dwBBitDepth - srcMask.dwBBitDepth;
				low = (srcMask.dwBBitDepth > high) ? srcMask.dwBBitDepth - high : 0;
				b = (b << high) | (b >> low);
			}
			else if (srcMask.dwBBitDepth > dstMask.dwBBitDepth)
				b >>= srcMask.dwBBitDepth - dstMask.dwBBitDepth;

			RGBM_Mono((unsigned char*)&r, (unsigned char*)&g, (unsigned char*)&b);
			color = dst->ddpfPixelFormat.dwRGBAlphaBitMask; // destination is opaque
			color |= r << dstMask.dwRBitOffset;
			color |= g << dstMask.dwGBitOffset;
			color |= b << dstMask.dwBBitOffset;
			memcpy(dstPtr, &color, dstBpp);
			srcPtr += srcBpp;
			dstPtr += dstBpp;
		}

		srcLine += src->lPitch;
		dstLine += dst->lPitch;
	}
}

void ConvertSurfaceToTextures(LPDIRECTDRAWSURFACEX surface)
{
	DDSURFACEDESCX tSurf;
	DDSURFACEDESCX uSurf;
	RECT r;
	unsigned short* pTexture;
	unsigned short* pSrc;

	memset(&tSurf, 0, sizeof(tSurf));
	tSurf.dwSize = sizeof(DDSURFACEDESCX);
	surface->Lock(0, &tSurf, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, 0);
	pSrc = (unsigned short*)tSurf.lpSurface;
	MonoScreen[0].surface = CreateTexturePage(tSurf.dwWidth, tSurf.dwHeight, 0, NULL, RGBM_Mono, -1);

	memset(&uSurf, 0, sizeof(uSurf));
	uSurf.dwSize = sizeof(DDSURFACEDESCX);
	MonoScreen[0].surface->Lock(0, &uSurf, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, 0);
	pTexture = (unsigned short*)uSurf.lpSurface;

	r.left = 0;
	r.top = 0;
	r.right = tSurf.dwWidth;
	r.bottom = tSurf.dwHeight;
	CustomBlt(&uSurf, 0, 0, &tSurf, &r);

	MonoScreen[0].surface->Unlock(0);
	DXAttempt(MonoScreen[0].surface->QueryInterface(TEXGUID, (void**)&MonoScreen[0].tex));
	surface->Unlock(0);
}

void CheckKeyConflicts()
{
	short key;

	for (int i = 0; i < 18; i++)
	{
		key = layout[0][i];
		conflict[i] = 0;

		for (int j = 0; j < 18; j++)
		{
			if (key == layout[1][j])
			{
				conflict[i] = 1;
				break;
			}
		}
	}
}

long S_PauseMenu()
{
	long fade = 0, ret;

	CreateMonoScreen();
	S_DisplayPauseMenu(1);
	InventoryActive = 1;
	Sound.SetReverbType(RT_Outside);

	do
	{
		S_InitialisePolyList();

		if (fade)
			dbinput = 0;
		else
			S_UpdateInput();

		SetDebounce = 1;
		S_DisplayMonoScreen();
		ret = S_DisplayPauseMenu(0);
		UpdatePulseColour();
		S_OutputPolyList();
		S_DumpScreen(G_dxptr->rViewport);

		if (ret == 1)
			break;

		if (ret == 8)
		{
			fade = 8;
			ret = 0;
			SetFade(0, 255);
		}

		if (fade && DoFade == 2)
		{
			ret = fade;
			break;
		}

	} while (!MainThread.ended);

	TIME_Init();
	FreeMonoScreen();
	InventoryActive = 0;
	return ret;
}

long GetSaveLoadFiles()
{
	FILE* file;
	SAVEFILE_INFO* pSave;
	SAVEGAME_INFO save_info;
	static long nSaves;
	char name[75];

	SaveCounter = 0;

	for (int i = 0; i < 15; i++)
	{
		pSave = &SaveGames[i];
		wsprintf(name, "savegame.%d", i);
		file = fopen(name, "rb");

		if (!file)
		{
			pSave->valid = 0;
			strcpy(pSave->name, SCRIPT_TEXT(TXT_Empty_Slot));
			continue;
		}

		fread(&pSave->name, sizeof(char), 75, file);
		fread(&pSave->num, sizeof(long), 1, file);
		fread(&pSave->days, sizeof(short), 1, file);
		fread(&pSave->hours, sizeof(short), 1, file);
		fread(&pSave->minutes, sizeof(short), 1, file);
		fread(&pSave->seconds, sizeof(short), 1, file);
		fread(&save_info, 1, sizeof(SAVEGAME_INFO), file);

		if (!CheckSumValid((char*)&save_info))
		{
			pSave->valid = 0;
			strcpy(pSave->name, SCRIPT_TEXT(TXT_Empty_Slot));
			continue;
		}

		if (pSave->num > SaveCounter)
			SaveCounter = pSave->num;

		pSave->valid = 1;
		fclose(file);
		nSaves++;
	}

	SaveCounter++;
	return nSaves;
}
