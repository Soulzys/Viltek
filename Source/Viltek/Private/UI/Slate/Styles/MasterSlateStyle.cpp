#include "../../../../Public/UI/Slate/Styles/MasterSlateStyle.h"
//#include "UI/Slate/Styles/MasterSlateStyle.h"

const FName FMasterSlateStyleStruct::TypeName = TEXT("FMasterSlateStyleStruct");

void FMasterSlateStyleStruct::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&SBRS_background_black);
}

const FMasterSlateStyleStruct& FMasterSlateStyleStruct::GetDefault()
{
	static FMasterSlateStyleStruct _defaultStruct;
	return _defaultStruct;
}