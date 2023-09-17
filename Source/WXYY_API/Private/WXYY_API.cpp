// Copyright Epic Games, Inc. All Rights Reserved.

#include "WXYY_API.h"

#define LOCTEXT_NAMESPACE "FWXYY_APIModule"

void FWXYY_APIModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FWXYY_APIModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWXYY_APIModule, WXYY_API)