#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ViltekGeoLib_DoorMaking.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_ViktelGeoLib_DoorMaking, Log, All);

UENUM()
enum class EDoorPositionOptions : uint8
{
	Center = 0, 
	Right  = 1, 
	Left   = 2
};

UENUM()
enum class EDoorWidthOptions : uint8
{
	SuperNarrow = 0, // 1m
	VeryNarrow  = 1, // 1.5m
	Narrow      = 2, // 2m
	Normal      = 3, // 2.5m
	NormalLarge = 4, // 3m
	Large       = 5, // 3.5m 
	VeryLarge   = 6, // 4m
	ExtraLarge  = 7  // 5m
};

UENUM()
enum class EDoorHeightOptions : uint8
{
	VeryLow    = 0, // 1m
	Low        = 1, // 1.5m
	NormalLow  = 2, // 2m
	Normal     = 3, // 2.5m
	NormalHigh = 4, // 3m
	High       = 5, // 3.5m
	VeryHigh   = 6, // 4m
	ExtraHigh  = 7  // 5m
};

USTRUCT()
struct VILTEK_API FVGeoLibDoorPiercingOptions
{
	GENERATED_BODY()

public:

	FVGeoLibDoorPiercingOptions();
	FVGeoLibDoorPiercingOptions(float p_WallLength, float p_WallHeight, float p_WallThickness);

public:
	
	bool IsWallSafeToPierce() const;

	float GET_DoorWidth() const;
	float GET_DoorHeight() const;

private:

	bool IsWidthSafe() const;
	bool IsHeightSafe() const;

public:
	
	EDoorWidthOptions    s_DoorWidth    = EDoorWidthOptions   ::Normal ;
	EDoorHeightOptions   s_DoorHeight   = EDoorHeightOptions  ::Normal ;
	EDoorPositionOptions s_DoorPosition = EDoorPositionOptions::Center ;

	float s_MinimumDoorToCornerDistance = 20.0f;
	float s_MinimumDoorToCeilingDistance = 20.0f;

	float s_WallLength    = 0.0f ;
	float s_WallThickness = 0.0f ;
	float s_WallHeight    = 0.0f ;

	bool s_bIsWallAxisNorthSouth = true;

private:

	float s_WidthOptions[8] =
	{
		100.0f,
		150.0f,
		200.0f,
		250.0f,
		300.0f,
		350.0f,
		400.0f,
		500.0f
	};

	float s_HeightOptions[8] =
	{
		100.0f,
		150.0f,
		200.0f,
		250.0f,
		300.0f,
		350.0f,
		400.0f,
		500.0f
	};
};

class UDynamicMesh;
class UVTEK_StaticMesh;

UCLASS()
class VILTEK_API UViltekGeoLib_DoorMaking : public UObject
{
	GENERATED_BODY()

public:

	static UDynamicMesh* PIERCE_BasicDoor
	(
		UDynamicMesh* p_targetWall, 
		const FTransform& p_targetWallLocation, 
		UDynamicMesh* p_cutoutMesh, 
		const FTransform& p_cutoutLocation, 
		int8 p_targetWallAxis, 
		float p_targetWallLength, 
		float p_targetWallThickness, 
		float p_targetWallHeight, 
		float p_doorWidth, 
		float p_doorHeight
	);

	static UDynamicMesh* PIERCE_Door
	(
		UDynamicMesh* p_TargetWall,
		const FVector& p_TargetWallLocation,
		const FVGeoLibDoorPiercingOptions& p_Options
	);		

	static void COPY_ToStaticMesh
	(
		UDynamicMesh* p_fromDynamicMesh, 
		UStaticMesh* p_toStaticMeshAsset
	);
};
