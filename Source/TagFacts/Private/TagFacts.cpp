// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagFacts.h"

#define LOCTEXT_NAMESPACE "FTagFactsModule"

DEFINE_LOG_CATEGORY(LogTagFacts);

void FTagFactsModule::StartupModule()
{
	UE_LOG(LogTagFacts, Log, TEXT("TagFacts module initiated"))
}

void FTagFactsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTagFactsModule, TagFacts)