#include "../../../../../Public/UI/Slate/Widgets/Specific/SCommandInput.h"
//#include "UI/Slate/Widgets/Specific/SCommandInput.h"
#include "../../../../../Public/UI/Slate/Widgets/Menu/SStartMenu.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "CommandInput"

SCommandInput::~SCommandInput()
{
	UE_LOG(LogTemp, Warning, TEXT("SCommandInput is being destroyed !"));
}

void SCommandInput::Construct(const FArguments& p_inArgs)
{
	m_OwningMenu = p_inArgs._m_OwningMenu;

	SEditableTextBox::Construct(SEditableTextBox::FArguments()
		.HintText(p_inArgs._m_HintText)
		.Justification(p_inArgs._m_Justification)
		.Style(p_inArgs._m_Style)
		.ClearKeyboardFocusOnCommit(p_inArgs._m_ClearKeyboardFocusOnCommit)
	);
}

FReply SCommandInput::OnPreviewKeyDown(const FGeometry& p_geometry, const FKeyEvent& p_inKeyEvent)
{
	const FKey _key = p_inKeyEvent.GetKey();

	if (_key == EKeys::Enter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ENTER input from SCommandInput !"));

		if (m_OwningMenu != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Command text : %s"), *GetText().ToString());
			m_OwningMenu->COMMIT_TextCommand(GetText());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("m_OwnerMenu is NULL !"));
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION