#pragma once

#include "CoreMinimal.h"
#include "Architecture/ArchRoomBuilderBase.h"
#include "EndingRoomBuilder.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_EndingRoomBuilder, Log, All);

UENUM()
enum class EEndingRoomAreaSize : uint8
{
	Small  = 0 , 
	Medium = 1 , 
	Large  = 2
};

UENUM()
enum class EEndingRoomPattern : uint8
{
	Basic   = 0, 
	Complex = 1
};

USTRUCT()
struct VILTEK_API FEndingRoomBuildOptions
{
	GENERATED_BODY()

public:

	EEndingRoomAreaSize s_AreaSize = EEndingRoomAreaSize::Medium ;
	EEndingRoomPattern  s_Pattern  = EEndingRoomPattern ::Basic  ;
};

UCLASS()
class VILTEK_API AEndingRoomBuilder : public AArchRoomBuilderBase
{
	GENERATED_BODY()

public:

	AEndingRoomBuilder();

	void BUILD_EndingRoom(const FEndingRoomBuildOptions& p_BuildOptions);

protected:

	void SETTING_RoomDimensions(EEndingRoomAreaSize p_AreaSize);
	void SETTING_InitialCorners();
	void SPLIT_RoomCorners(EEndingRoomPattern p_RoomPattern, int32 p_TargetCornerIndex = 1);

protected: // Splitting functions

	void SPLITTING_BasicPattern(int32 p_TargetCornerIndex);
	void SPLITTING_ComplexPattern(int32 p_TargetCornerIndex);
	
};
