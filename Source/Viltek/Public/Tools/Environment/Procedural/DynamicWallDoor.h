#pragma once

#include "CoreMinimal.h"
#include "Tools/Environment/Procedural/BaseDynamicWall.h"
#include "DynamicWallDoor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_DynamicWallDoor, Log, All);

class UDynamicMeshComponent;

UCLASS()
class VILTEK_API ADynamicWallDoor : public ABaseDynamicWall
{
	GENERATED_BODY()

public:

	ADynamicWallDoor();

	void GENERATE_DoorWall
	(
		float p_wallLength, 
		float p_wallHeight, 
		float p_wallThickness, 
		float p_uv_x, 
		float p_uv_y, 
		UMaterialInstance* p_material, 
		float p_cutoutWidth, 
		float p_cutoutHeight, 
		float p_cutoutDepth, 
		const FVector& p_cutoutPosition
	);

	// *** SETTERS *** //
	void SET_CutoutWidth(float p_width);
	void SET_CutoutHeight(float p_height);
	void SET_CutoutDepth(float p_depth);
	void SET_CutoutLocation(const FVector& p_location);

protected:


	void GENERATE_Cutout();
	void EXTRUDE_Cutout();
	void APPLY_CutoutBoolean();
	void DESTROY_Cutout();

protected:

	UDynamicMeshComponent* m_DMC_cutout = nullptr;
	
	float m_cutoutWidth; // x
	float m_cutoutHeight; // z
	float m_cutoutDepth; // y
	FVector m_cutoutLocation;
};
