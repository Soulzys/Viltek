// Copyright Epic Games, Inc. All Rights Reserved.

#include "Viltek.h"
#include "Modules/ModuleManager.h"
#include "Public/UI/Slate/Styles/MasterStyleController.h"

class FViltekGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(FMasterStyleController::GET_StyleSetName());
		FMasterStyleController::Initialize();
	}

	virtual void ShutdownModule() override
	{
		FMasterStyleController::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FViltekGameModule, Viltek, "Viltek" );
