#pragma once

#include "CoreMinimal.h"
#include "Architecture/ArchBuilderBase.h"
#include "CorridorBuilderTest.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_CorridorBuilderTest, Log, All);

class AArchRoomBuilderBase;
struct FArchRoomVolume;

USTRUCT()
struct VILTEK_API FCorridorBuilderTestOptions
{
	GENERATED_BODY()

public:

	AArchRoomBuilderBase* s_RoomStart = nullptr;
	AArchRoomBuilderBase* s_RoomEnd = nullptr;

	FVector s_StartLoc = FVector(0);
	FVector s_EndLoc   = FVector(0);
};

UCLASS()
class VILTEK_API ACorridorBuilderTest : public AArchBuilderBase
{
	GENERATED_BODY()

public:

	ACorridorBuilderTest();

public:

	void BUILD_CorridorTest(const FCorridorBuilderTestOptions& p_BuildOptions);

protected:

	void COMPUTE_CorridorPoint(ECardinalDirection p_Direction, uint32 p_RoomExteriorDistance, const FVector& p_StartLoc, FVector& p_OutVector);
	//void COMPUTE_Path(const FVector& p_StartLoc, const FVector& p_EndLoc, const FArchVolume& p_StartRoomVolume, const FArchVolume& p_EndRoomVolume);

protected:

	TArray<FVector> m_CorridorPath;

	int32 m_CorridorWidth;
};
