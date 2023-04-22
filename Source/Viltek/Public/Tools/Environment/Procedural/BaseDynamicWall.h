#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "BaseDynamicWall.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_BaseDynamicWall, Log, All);

class UDynamicMeshComponent;
class UMaterialInstance;

UCLASS()
class VILTEK_API ABaseDynamicWall : public ADynamicMeshActor
{
	GENERATED_BODY()

public:

	// *** Constructors *** //
	//
	ABaseDynamicWall();

	// *** Functions *** //
	void GENERATE_BasicWall
	(
		float p_wallLength, 
		float p_wallHeight, 
		float p_wallThickness, 
		float p_uv_x = 1.0f, 
		float p_uv_y = 1.0f, 
		UMaterialInstance* p_material = nullptr
	);

	// *** Setters *** //
	//
	void SET_WallLength   (float p_length    );
	void SET_WallHeight   (float p_height    );
	void SET_WallThickness(float p_thickness );
	void SET_MeshUVX      (float p_uv_x      );
	void SET_MeshUVY      (float p_uv_y      );
	void SET_MaterialInstance(UMaterialInstance* p_material);

protected:

	virtual void BeginPlay() override;

protected:

	UDynamicMeshComponent* m_DMC_wall = nullptr;

	UMaterialInstance* m_MI_baseMaterial = nullptr;

	float m_wallLength;
	float m_wallHeight;
	float m_wallThickness;	
	float m_meshUV_X;
	float m_meshUV_Y;
};
