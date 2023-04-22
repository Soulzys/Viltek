#include "Tools/Environment/Procedural/BaseDynamicWall.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"

DEFINE_LOG_CATEGORY(LogClass_BaseDynamicWall);

ABaseDynamicWall::ABaseDynamicWall()
{
	// *** Parent class variables initialization *** //
	//
	PrimaryActorTick.bCanEverTick = false;

	// *** Variables initialization *** //
	//
	m_wallLength = 200.0f;
	m_wallHeight = 150.0f;
	m_wallThickness = 25.0f;
	m_meshUV_X = 1.0f;
	m_meshUV_Y = 1.0f;

	// *** Components initialization *** //
	//
	m_DMC_wall = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_wall"));
	m_DMC_wall->SetMobility(EComponentMobility::Movable);
	m_DMC_wall->SetGenerateOverlapEvents(false);
	m_DMC_wall->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_wall->CollisionType = ECollisionTraceFlag::CTF_UseDefault;
	m_DMC_wall->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));

	SetRootComponent(m_DMC_wall);
}

void ABaseDynamicWall::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseDynamicWall::GENERATE_BasicWall
(
	float p_wallLength, 
	float p_wallHeight, 
	float p_wallThickness, 
	float p_uv_x, 
	float p_uv_y, 
	UMaterialInstance* p_material
)
{
	check(m_DMC_wall);

	SET_WallLength(p_wallLength);
	SET_WallHeight(p_wallHeight);
	SET_WallThickness(p_wallThickness);
	SET_MeshUVX(p_uv_x);
	SET_MeshUVY(p_uv_y);
	SET_MaterialInstance(p_material);

	/* CREATE WALL */

	FGeometryScriptPrimitiveOptions _primitiveOpts;
	FTransform _transform;

	m_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		m_DMC_wall->GetDynamicMesh(),
		_primitiveOpts,
		_transform,
		m_wallLength, m_wallThickness, m_wallHeight
	));

	/* RECALCULATE UV */

	FGeometryScriptRecomputeUVsOptions _recomputeUVsOpts;

	m_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshUVFunctions::RecomputeMeshUVs
	(
		m_DMC_wall->GetDynamicMesh(), 
		0, 
		_recomputeUVsOpts
	));

	/* SCALE UV */

	FVector2D _uvScale;
	_uvScale.X = m_meshUV_X;
	_uvScale.Y = m_meshUV_Y;

	FVector2D _uvOrigin;

	m_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshUVFunctions::ScaleMeshUVs
	(
		m_DMC_wall->GetDynamicMesh(), 
		0, 
		_uvScale, 
		_uvOrigin
	));

	/* MATERIAL */

	m_DMC_wall->SetMaterial(0, m_MI_baseMaterial);
}

// ****** SETTERS ****** //
//
void ABaseDynamicWall::SET_WallLength(float p_length)
{
	if (p_length > 0.0f)
	{
		m_wallLength = p_length;
	}
	else
	{
		UE_LOG(LogClass_BaseDynamicWall, Warning, TEXT("FUNCTION ''SET_WallLength'' ===>>> PARAMETER ISSUE -->> p_lenght < 0"));
	}
}

void ABaseDynamicWall::SET_WallHeight(float p_height)
{
	if (p_height > 0.0f)
	{
		m_wallHeight = p_height;
	}
	else
	{
		UE_LOG(LogClass_BaseDynamicWall, Warning, TEXT("FUNCTION ''SET_WallHeight'' ===>>> PARAMETER ISSUE -->> p_height < 0"));
	}
}

void ABaseDynamicWall::SET_WallThickness(float p_thickness)
{
	if (p_thickness > 0.0f)
	{
		m_wallThickness = p_thickness;
	}
	else
	{
		UE_LOG(LogClass_BaseDynamicWall, Warning, TEXT("FUNCTION ''SET_WallThickness'' ===>>> PARAMETER ISSUE -->> p_thickness < 0"));
	}
}

void ABaseDynamicWall::SET_MeshUVX(float p_uv_x)
{
	if (p_uv_x > 0.0f)
	{
		m_meshUV_X = p_uv_x;
	}
	else
	{
		UE_LOG(LogClass_BaseDynamicWall, Warning, TEXT("FUNCTION ''SET_MeshUVX'' ===>>> PARAMETER ISSUE -->> p_uv_x < 0"));
	}
}

void ABaseDynamicWall::SET_MeshUVY(float p_uv_y)
{
	if (p_uv_y > 0.0f)
	{
		m_meshUV_Y = p_uv_y;
	}
	else
	{
		UE_LOG(LogClass_BaseDynamicWall, Warning, TEXT("FUNCTION ''SET_MeshUVY'' ===>>> PARAMETER ISSUE -->> p_uv_y < 0"));
	}
}

void ABaseDynamicWall::SET_MaterialInstance(UMaterialInstance* p_material)
{
	if (p_material != nullptr)
	{
		m_MI_baseMaterial = p_material;
	}
	else
	{
		UE_LOG(LogClass_BaseDynamicWall, Warning, TEXT("FUNCTION ''SET_MaterialInstance'' ===>>> PARAMETER ISSUE -->> p_material == NULL"));
	}
}