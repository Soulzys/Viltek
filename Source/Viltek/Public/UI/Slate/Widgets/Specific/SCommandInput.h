#pragma once

#include "SlateBasics.h"
#include "InputCoreTypes.h"

class SStartMenu;

class VILTEK_API SCommandInput : public SEditableTextBox
{

public:

	SLATE_BEGIN_ARGS(SCommandInput) {}

	SLATE_STYLE_ARGUMENT(FEditableTextBoxStyle, m_Style)

	SLATE_ARGUMENT(SStartMenu*, m_OwningMenu)

	SLATE_ATTRIBUTE(FText, m_HintText)
	SLATE_ATTRIBUTE(ETextJustify::Type, m_Justification)
	SLATE_ATTRIBUTE(bool, m_ClearKeyboardFocusOnCommit)

	SLATE_END_ARGS()

public:

	~SCommandInput();
	void Construct(const FArguments& p_inArgs);

private:

	virtual bool SupportsKeyboardFocus() const override { return true; };
	virtual FReply OnPreviewKeyDown(const FGeometry& p_geometry, const FKeyEvent& p_inKeyEvent) override;

private:

	SStartMenu* m_OwningMenu;
};
