#include "../../../../Public/UI/Slate/Styles/MasterStyleController.h"
//#include "UI/Slate/Styles/MasterStyleController.h"
#include "Slate/SlateGameResources.h"

TSharedPtr<FSlateStyleSet> FMasterStyleController::m_controllerInstance = nullptr;

void FMasterStyleController::Initialize()
{
	if (m_controllerInstance.IsValid() == false)
	{
		m_controllerInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*m_controllerInstance);
	}
}

void FMasterStyleController::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*m_controllerInstance);
	ensure(m_controllerInstance.IsUnique());
	m_controllerInstance.Reset();
}

FName FMasterStyleController::GET_StyleSetName()
{
	static FName _styleSetName(TEXT("MasterStyleController"));
	return _styleSetName;
}

const ISlateStyle& FMasterStyleController::GET_StyleSet()
{
	// Need to have a check test
	return *m_controllerInstance;
}

TSharedRef<FSlateStyleSet> FMasterStyleController::Create()
{
	TSharedRef<FSlateStyleSet> _styleRef = FSlateGameResources::New
	(
		FMasterStyleController::GET_StyleSetName(),
		"/Game/UI/Slate/Styles",
		"/Game/UI/Slate/Styles"
	);

	return _styleRef;
}