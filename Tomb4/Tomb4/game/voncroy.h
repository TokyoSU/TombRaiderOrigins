#pragma once

void SetCutSceneCamera(ItemInfo* item);
void ClearCutSceneCamera();
void GetAIEnemy(CreatureInfo* info, long tfl);
void DoVonCroyCutscene(ItemInfo* item, CreatureInfo* info);
void InitialiseVoncroy(short item_number);
void VoncroyRaceControl(short item_number);
void VoncroyControl(short item_number);

extern uchar VonCroyCutFlags[64];
extern char bVoncroyCutScene;
