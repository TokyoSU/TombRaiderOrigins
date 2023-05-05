#pragma once
#include "../global/types.h"

bool REG_OpenKey(LPCSTR lpSubKey);
bool OpenRegistry(LPCSTR SubKeyName);
void REG_CloseKey();
void CloseRegistry();
void REG_WriteLong(char* SubKeyName, unsigned long value);
void REG_WriteBool(char* SubKeyName, bool value);
bool REG_ReadLong(char* SubKeyName, unsigned long& value, unsigned long defaultValue);
bool REG_ReadBool(char* SubKeyName, bool& value, bool defaultValue);
bool LoadSettings();
void SaveSettings();
bool SaveSetup(HWND hDlg);
bool REG_KeyWasCreated();
