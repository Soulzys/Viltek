#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumeShell.generated.h"

class USceneComponent;
class UDynamicMeshComponent;
struct FArchVolume;
struct FArchVolumeCuboid;
struct FArchCorner;

UENUM()
enum class ECornerType : uint8
{
	TopRight_Bottom    = 0 , 
	BottomRight_Bottom = 1 , 
	BottomLeft_Bottom  = 2 , 
	TopLeft_Bottom     = 3 , 
	TopRight_Top       = 4 , 
	BottomRight_Top    = 5 , 
	BottomLeft_Top     = 6 , 
	TopLeft_Top        = 7
};

UCLASS()
class VILTEK_API AVolumeShell : public AActor
{
	GENERATED_BODY()
	
public:	

	AVolumeShell();

public:

	void GENERATE_RoomShell();
	void GENERATE_WorldShell();

	void SET_ArchVolume(FArchVolume* p_ArchVolumeRef);
	void SET_ArchVolumeCuboid(FArchVolumeCuboid* p_ArchVolumeCuboidRef);

	void SET_LargestX(float p_LargestX) ;
	void SET_LargestY(float p_LargestY) ;
	void SET_LargestZ(float p_LargestZ) ;
	void SET_LowestX (float p_LowestX)  ;
	void SET_LowestY (float p_LowestY)  ;
	void SET_LowestZ (float p_LowestZ)  ;

	float GET_LargestX() const;
	float GET_LargestY() const;
	float GET_LargestZ() const;
	float GET_LowestX () const;
	float GET_LowestY () const;
	float GET_LowestZ () const;

	FVector GET_CornerPosition(ECornerType p_CornerType);

	void COMPUTESET_VolumeDimensions
	(
		float p_LargestX , 
		float p_LargestY , 
		float p_LargestZ , 
		float p_LowestX  , 
		float p_LowestY  , 
		float p_LowestZ
	);

	void COMPUTESET_SpaceOffset(uint8 p_SpaceOffset);

protected:

	FVector COMPUTE_VectorsMiddleLocation(const FVector& p_V1, const FVector& p_V2);
	void COMPUTE_VolumeSize();

protected:

	FArchVolume* m_ArchVolume = nullptr;
	FArchVolumeCuboid* m_ArchVolumeCuboid = nullptr;

	USceneComponent* m_SC_RootComponent = nullptr;

	FVector m_VolumeShellCenterLoc; // This is different from AVolumeShell->GetActorLocation() !!
	
	UDynamicMeshComponent* m_DMC_ShellSide_1 = nullptr; // North
	UDynamicMeshComponent* m_DMC_ShellSide_2 = nullptr; // East
	UDynamicMeshComponent* m_DMC_ShellSide_3 = nullptr; // South
	UDynamicMeshComponent* m_DMC_ShellSide_4 = nullptr; // West
	UDynamicMeshComponent* m_DMC_ShellSide_5 = nullptr; // Top
	UDynamicMeshComponent* m_DMC_ShellSide_6 = nullptr; // Down

	float m_Length ; // X axis
	float m_Width  ; // Y axis
	float m_Height ; // Z axis

	float m_LargestX ; // The world X position of the furthest point on the X axis
	float m_LargestY ;
	float m_LargestZ ;
	float m_LowestX  ;
	float m_LowestY  ;
	float m_LowestZ  ;
};
