#pragma once

#include "CoreMinimal.h"
#include "../../Public/Architecture/ArchBuilderBase.h"
#include "CorridorBuilder.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_CorridorBuilder, Log, All);

USTRUCT()
struct VILTEK_API FCorridorNode
{
	GENERATED_BODY()

public:

	FVector s_Location = FVector(0);

	FCorridorNode* s_Parent = nullptr;

	bool s_bIsStartNode = false;
	bool s_bIsEndNode = false;

	ECardinalDirection s_ParentDirection = ECardinalDirection::Unknown;
};

UCLASS()
class VILTEK_API ACorridorBuilder : public AArchBuilderBase
{
	GENERATED_BODY()

public:

	ACorridorBuilder();

	void BUILD_Corridor(const FVector& p_StartPos, ECardinalDirection p_StartPosFacingDir, const FVector& p_EndPos, ECardinalDirection p_EndPosFacingDir, uint8 p_SafeRoomSpace);

protected:

	virtual void BeginPlay() override;

protected:

	void SHOOT_Ray(const FVector p_RayStartLoc, ECardinalDirection p_RayDir);

protected:

	int32 m_CorridorLength;
	int32 m_CorridorBaseWidth;
};
