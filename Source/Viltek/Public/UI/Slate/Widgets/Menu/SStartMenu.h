#pragma once

#include "SlateBasics.h"
#include "InputCoreTypes.h"

class AMasterHUD;
class SCommandInput;

class VILTEK_API SStartMenu : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS(SStartMenu) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AMasterHUD>, m_owningHUD)

	SLATE_END_ARGS()

public:

	~SStartMenu();


	void COMMIT_TextCommand(const FText& p_command);

private:

	virtual bool SupportsKeyboardFocus() const override { return true; };
	virtual FReply OnKeyDown(const FGeometry& p_geometry, const FKeyEvent& p_inKeyEvent) override;

private:


private:

	TWeakObjectPtr<AMasterHUD> m_owningHUD = nullptr;
	
	TSharedPtr<SEditableTextBox> m_EditableTextBox;
	TSharedPtr<SCommandInput> m_commandInput;
};
