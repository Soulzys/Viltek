#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Public/Architecture/ArchRoomBuilderBase.h"
#include "StartingRoomBuilder.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_StartingRoomBuilder, Log, All);

class AFillingCube;
class AArchSas;

// *** Building Options data type *** //

UENUM()
enum class EStartingRoomAreaSize : uint8
{
	Small  = 0 , 
	Medium = 1 , 
	Large  = 2
};

UENUM()
enum class EStartingRoomPattern : uint8
{
	Basic       = 0 , 
	VPattern    = 1 , 
	DoubleRooms = 2
};

USTRUCT()
struct VILTEK_API FStartingRoomBuildOptions
{
	GENERATED_BODY()

public:

	EStartingRoomAreaSize s_areaSize = EStartingRoomAreaSize::Small ;
	EStartingRoomPattern  s_pattern  = EStartingRoomPattern ::Basic ;
};

//

UCLASS()
class VILTEK_API AStartingRoomBuilder : public AArchRoomBuilderBase
{
	GENERATED_BODY()
	
public:	

	AStartingRoomBuilder();

	void BUILD_StartingRoom(const FStartingRoomBuildOptions& p_buildOptions);

protected:

	virtual void BeginPlay() override;

	void SETTING_RoomDimensions(EStartingRoomAreaSize p_roomInitialAreaSize);
	void SETTING_InitialCorners();
	void SPLIT_RoomCorners(EStartingRoomPattern p_roomPattern, int32 p_targetCornerIndex = 1);

	// For later uses
	void PLACE_PlayerStart();

protected: // Splitting functions

	void SPLITTING_VPattern(int32 p_targetCornerIndex);
	void SPLITTING_DoubleRooms(int32 p_targetCornerIndex);

protected:

	bool m_bIsRoomFilled = false;
};
