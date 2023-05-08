#pragma once

extern asIScriptEngine* scriptEngine;
extern CContextMgr scriptManager;
extern CScriptBuilder scriptBuilder;

//extern asIScriptContext* GetEntityContext(int object_number);
extern void InitialiseScripting();
extern void ReleaseScripting();
