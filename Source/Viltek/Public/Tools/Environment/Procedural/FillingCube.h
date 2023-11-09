#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FillingCube.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_FillingCube, Log, All);

class UStaticMeshComponent;
class UMaterialInstance;

UENUM()
enum class EFillingCubeFriendTypes : uint8
{
	RoomWall         = 0 , 
	OtherFillingCube = 1 , 
	Void             = 2 , 
	Error            = 3
};

UENUM()
enum class EFillingCubeFaces : uint8
{
	Face_1 = 0,
	Face_2 = 1,
	Face_3 = 2,
	Face_4 = 3,
	Face_5 = 4,  // Top
	Face_6 = 5   // Bottom
};

UCLASS()
class VILTEK_API AFillingCube : public AActor
{
	GENERATED_BODY()
	
public:	

	AFillingCube();

public:

	void UPDATE_CubeColor();
	void SHOOT_Rays();

	bool CHECK_IsEveryFriendFound();

	TArray<AFillingCube*>& GET_FriendCubesArray();
	
protected:

	virtual void BeginPlay() override;

protected:

	EFillingCubeFriendTypes SHOOT_Ray(const FVector& p_RayTargetLoc);
	EFillingCubeFriendTypes SHOOT_Ray(EFillingCubeFaces p_Face);

protected:

	UStaticMeshComponent* m_SMC_FillingCube = nullptr;

	UMaterialInstance* m_MI_SKTC_Green   = nullptr; // 0 Friends
	UMaterialInstance* m_MI_SKTC_Blue    = nullptr; // 1 Friend
	UMaterialInstance* m_MI_SKTC_Yellow  = nullptr; // 2 Friends
	UMaterialInstance* m_MI_SKTC_Magenta = nullptr; // ...
	UMaterialInstance* m_MI_SKTC_Cyan    = nullptr;
	UMaterialInstance* m_MI_SKTC_Red     = nullptr;
	UMaterialInstance* m_MI_SKTC_Black   = nullptr; // 6 Friends

	EFillingCubeFriendTypes m_Face_1_FriendType;
	EFillingCubeFriendTypes m_Face_2_FriendType;
	EFillingCubeFriendTypes m_Face_3_FriendType;
	EFillingCubeFriendTypes m_Face_4_FriendType;
	EFillingCubeFriendTypes m_Face_5_FriendType;
	EFillingCubeFriendTypes m_Face_6_FriendType;

	TArray<AFillingCube*> m_FriendCubes;

	int8 m_ObstaclesCount;
};
