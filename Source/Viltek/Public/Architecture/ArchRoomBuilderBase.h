#pragma once

#include "CoreMinimal.h"
#include "Architecture/ArchBuilderBase.h"
#include "ArchRoomBuilderBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_ArchRoomBuilderBase, Log, All);

class AFillingCube;
class AArchSas;

UCLASS()
class VILTEK_API AArchRoomBuilderBase : public AArchBuilderBase
{
	GENERATED_BODY()

public:

	AArchRoomBuilderBase();

public:

	const FVector& GET_DoorLocation() const;
	ECardinalDirection GET_DoorDirection() const;

protected:

	void CREATE_RoomDynamicWallStructs();
	void CREATE_GroundAndCeiling();
	FVector COMPUTE_FirstCubeElementLocation();
	void SPAWN_CubeElements(const FVector& p_FirstElementLoc);
	void PIERCE_Door();

	// Testing
	void COMPUTE_RoomNewDimensions(uint32 p_RoomExteriorDistance); // We could also compute it for every SPLIT, idk which way is the best

protected:

	TArray<FRoomFillingEntity*> m_RoomFillingEntities;
	TArray<AFillingCube*> m_RoomFillingCubes; 

	// This should eventually be put into the ASasBuilder, or at least into a dedicated USTRUCT
	FVector m_DoorLocation;
	ECardinalDirection m_DoorDirection;

	AArchSas* m_ArchSas = nullptr;
};
