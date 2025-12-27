// Copyright (C) Varian Daemon 2025. All Rights Reserved.

#include "TagFactsEditor.h"

#include "FFactCustomization.h"
#include "GameplayTagsManager.h"
#include "Data/CoreTagFactData.h"

IMPLEMENT_GAME_MODULE(FTagFactsEditorModule, TagFactsEditor)

#define LOCTEXT_NAMESPACE "FTagFactsEditorModule"

void FTagFactsEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Register the FS_Fact struct customization
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FFact::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FFactCustomization::MakeInstance));

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FTagFactsEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE