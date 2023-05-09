#include "pch.h"
#include "tr2_rat.h"
#include "control.h"
#include "scripting.h"
#include "function_stubs.h"
#include "objects.h"

static asIScriptFunction* initialiseFunc = nullptr;
static asIScriptFunction* controlFunc = nullptr;
static asIScriptContext* initialiseContext = nullptr;
static asIScriptContext* controlContext = nullptr;

void LoadTR2RatScript()
{
	scriptBuilder.AddSectionFromFile("scripts/creatures/tr2_rat.as");
	scriptBuilder.BuildModule();

	auto* mod = scriptBuilder.GetModule();
	initialiseFunc = mod->GetFunctionByName("InitialiseRat_TR2");
	controlFunc = mod->GetFunctionByName("ControlRat_TR2");

#if _DEBUG
	Log(1, "TR2 rat script: (init: %s, control: %s)",
		initialiseFunc != nullptr ? "exist" : "missing",
		controlFunc != nullptr ? "exist" : "missing"
	);
#endif

	initialiseContext = scriptEngine->CreateContext();
	controlContext = scriptEngine->CreateContext();
}

void ReleaseTR2RatScript()
{
	if (initialiseContext != NULL)
	{
		assert(initialiseContext->Abort() >= 0);
		assert(initialiseContext->Release() >= 0);
		initialiseContext = NULL;
	}

	if (controlContext != NULL)
	{
		assert(controlContext->Abort() >= 0);
		assert(controlContext->Release() >= 0);
		controlContext = NULL;
	}
#if _DEBUG
	Log(1, "Released tr2 rat script.");
#endif
}

void InitialiseRat_TR2(short item_number)
{
	auto* item = &items[item_number];
	if (initialiseContext == NULL)
	{
#if _DEBUG
		Log(2, "Error in initialise rat (tr2), the init context is nullptr !");
#endif
		return;
	}
	assert(initialiseContext->Prepare(initialiseFunc) >= 0);
	assert(initialiseContext->SetArgWord(0, item_number) >= 0);
	if (initialiseContext->Execute() == asEXECUTION_EXCEPTION)
	{
		Log(2, GetExceptionInfo(controlContext).c_str());
	}
}

void ControlRat_TR2(short item_number)
{
	auto* item = &items[item_number];
	if (controlContext == NULL)
	{
#if _DEBUG
		Log(2, "Error in control rat (tr2), the control context is nullptr !");
#endif
		return;
	}
	assert(controlContext->Prepare(controlFunc) >= 0);
	assert(controlContext->SetArgWord(0, item_number) >= 0);
	if (controlContext->Execute() == asEXECUTION_EXCEPTION)
	{
		Log(2, GetExceptionInfo(controlContext).c_str());
	}
}