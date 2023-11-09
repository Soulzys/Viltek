#pragma once

#include "SlateBasics.h"
#include "../../../../Public/UI/Slate/Styles/MasterSlateStyle.h"
#include "MasterStyleController.h"

class VILTEK_API FMasterStyleController
{

public:

	static void Initialize();
	static void Shutdown();

public:

	static FName GET_StyleSetName();
	static const ISlateStyle& GET_StyleSet();

	static const struct FMasterSlateStyleStruct& GET_MasterSlateStyleStruct()
	{
		return FMasterStyleController::GET_StyleSet().GetWidgetStyle<FMasterSlateStyleStruct>("SWS_MasterStyle");
	}

private:

	static TSharedRef<class FSlateStyleSet> Create();

private:

	static TSharedPtr<class FSlateStyleSet> m_controllerInstance;
};
