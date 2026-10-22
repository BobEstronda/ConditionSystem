// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConditionSystemEditor.h"
#include "ConditionSystem/Public/ConditionTypes.h"
#include "ConditionSystemEditor/Public/ConditionCustomizations.h"

#define LOCTEXT_NAMESPACE "FConditionSystemEditorModule"

void FConditionSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPropertyEditorModule& PE = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	/*PE.RegisterCustomPropertyTypeLayout(FConditionAccessor::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FConditionAccessorCustomization::MakeInstance));
	PE.RegisterCustomPropertyTypeLayout(FConditionStep::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FConditionStepCustomization::MakeInstance));
	PE.RegisterCustomPropertyTypeLayout(FConditionParam::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FConditionParamCustomization::MakeInstance));*/
	PE.RegisterCustomPropertyTypeLayout(FConditionOperand::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FConditionOperandCustomization::MakeInstance));

	PE.NotifyCustomizationModuleChanged();
}

void FConditionSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PE = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		/*PE.UnregisterCustomPropertyTypeLayout(FConditionAccessor::StaticStruct()->GetFName());
		PE.UnregisterCustomPropertyTypeLayout(FConditionStep::StaticStruct()->GetFName());
		PE.UnregisterCustomPropertyTypeLayout(FConditionParam::StaticStruct()->GetFName());*/
		PE.UnregisterCustomPropertyTypeLayout(FConditionOperand::StaticStruct()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FConditionSystemEditorModule, ConditionSystemEditor)