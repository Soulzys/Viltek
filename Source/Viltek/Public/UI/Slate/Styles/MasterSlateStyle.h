#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "MasterSlateStyle.generated.h"

USTRUCT()
struct FMasterSlateStyleStruct : public FSlateWidgetStyle
{
	GENERATED_BODY()

public:

	virtual const FName GetTypeName() const override { return TypeName; };
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FMasterSlateStyleStruct& GetDefault();

public:

	UPROPERTY(EditAnywhere, Category = "TextBlockStyles") FTextBlockStyle TXTS_default;

	UPROPERTY(EditAnywhere, Category = "EditableTextBox") FEditableTextBoxStyle EDTB_default;

	UPROPERTY(EditAnywhere, Category = "SlateBrush") FSlateBrush SBRS_background_black;

	//UPROPERTY(EditAnywhere, Category = "Fonts") FFontData;
	//UPROPERTY(EditAnywhere, Category = "Fonts") FFontRenderInfo;
	UPROPERTY(EditAnywhere, Category = "Fonts") FSlateFontInfo SFIN_default;

public:

	static const FName TypeName;
};

UCLASS()
class VILTEK_API UMasterSlateStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Appearance", meta = (ShowOnlyInnerProperties))
	FMasterSlateStyleStruct m_masterStyleStruct;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&m_masterStyleStruct);
	}	
};
