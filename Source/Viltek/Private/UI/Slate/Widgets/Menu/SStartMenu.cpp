#include "../../../../../Public/UI/Slate/Widgets/Menu/SStartMenu.h"
#include "../../../../../Public/UI/HUD/MasterHUD.h"
#include "../../../../../Public/UI/Slate/Styles/MasterSlateStyle.h"
#include "../../../../../Public/UI/Slate/Styles/MasterStyleController.h"
#include "../../../../../Public/UI/Slate/Widgets/Specific/SCommandInput.h"
//#include "Viltek/Public/Controllers/PControllers/SpectatorPController.h"
//#include "../../../../../Viltek/Public/Controllers/PControllers/SpectatorPController.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "StartMenu"

SStartMenu::~SStartMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("SStartMenu is being destroyed !"));
}

void SStartMenu::Construct(const FArguments& p_inArgs)
{
	m_owningHUD = p_inArgs._m_owningHUD;

	const FMasterSlateStyleStruct& _masterStyle = FMasterStyleController::GET_MasterSlateStyleStruct();

	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[
		SNew(SOverlay)

		+ SOverlay::Slot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SImage)
			.Image(&_masterStyle.SBRS_background_black)
		]
		
		+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(FOptionalSize(550.0f))
			.HeightOverride(FOptionalSize(80.0f))
			[
				SAssignNew(m_commandInput, SCommandInput)
				.m_OwningMenu(this)
				.m_HintText(LOCTEXT("GameCommand", "Insert command !"))
				.m_Justification(ETextJustify::Left)
				.m_Style(&_masterStyle.EDTB_default)
				.m_ClearKeyboardFocusOnCommit(true)
			]			
		]
	];
}

FReply SStartMenu::OnKeyDown(const FGeometry& p_geometry, const FKeyEvent& p_inKeyEvent)
{
	const FKey _key = p_inKeyEvent.GetKey();

	if (_key == EKeys::Enter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ENTER KEY WAS ACTIVATED !!!"));

		if (m_EditableTextBox.IsValid())
		{
			COMMIT_TextCommand(m_EditableTextBox->GetText());
			return FReply::Handled();
		}
	}

	return FReply::Handled();
}

void SStartMenu::COMMIT_TextCommand(const FText& p_command)
{
	if (p_command.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Command is empty !"));
		return;
	}

	p_command.ToLower();

	FString _text = p_command.ToString();

	// PLAY ==>> Starts the game
	if (_text == "play")
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAY !!"));
		m_owningHUD->START_Game();
	}

	// OPTIONS ==>> Opens the option menu
	if (_text == "option" || _text == "options" || _text == "parameter" || _text == "parameters")
	{

	}

	// QUIT ==>> Closes the game
	if (_text == "quit" || _text == "quit game" || _text == "leave" || _text == "leave game")
	{
		if (m_owningHUD.IsValid())
		{
			APlayerController* _PC = m_owningHUD->PlayerOwner;

			if (_PC)
			{
				_PC->ConsoleCommand("quit");
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION