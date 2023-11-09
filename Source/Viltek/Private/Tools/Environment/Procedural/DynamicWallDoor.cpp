#include "Tools/Environment/Procedural/DynamicWallDoor.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshModelingFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogClass_DynamicWallDoor);

ADynamicWallDoor::ADynamicWallDoor()
{
	// *** Variables initialization *** //
	//
	m_cutoutWidth  = 30.0f;
	m_cutoutHeight = 80.0f;
	m_cutoutDepth  = 50.0f;
	m_cutoutLocation = FVector(0);

	// *** Components initialization *** //
	//
	m_DMC_cutout = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_cutout"));
	m_DMC_cutout->SetMobility(EComponentMobility::Movable);
	m_DMC_cutout->SetGenerateOverlapEvents(false);
	m_DMC_cutout->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_cutout->CollisionType = ECollisionTraceFlag::CTF_UseDefault;
	m_DMC_cutout->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));
	m_DMC_cutout->SetupAttachment(m_DMC_wall);	
	m_DMC_cutout->SetRelativeLocation(FVector(0.0f, 250.0f, 30.0f));
}

void ADynamicWallDoor::GENERATE_DoorWall
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
	const FVector& p_cutoutLocation
)
{
	GENERATE_BasicWall(p_wallLength, p_wallHeight, p_wallThickness, p_uv_x, p_uv_y, p_material); // From the parent class

	SET_CutoutWidth(p_cutoutWidth);
	SET_CutoutHeight(p_cutoutHeight);
	SET_CutoutDepth(p_cutoutDepth);
	SET_CutoutLocation(p_cutoutLocation);

	GENERATE_Cutout();
	EXTRUDE_Cutout();
	APPLY_CutoutBoolean();
	DESTROY_Cutout();

	//DrawDebugSphere(GetWorld(), m_DMC_cutout->GetRelativeLocation(), 25.0f, 16, FColor::Red, true, 1, 3.0f);
}

void ADynamicWallDoor::GENERATE_Cutout()
{
	check(m_DMC_wall);
	check(m_DMC_cutout);

	FGeometryScriptPrimitiveOptions _primitiveOpts;
	FTransform _transform;
	FRotator _rota = FRotator(0.0f, 0.0f, 90.0f);
	_transform.SetRotation(_rota.Quaternion());

	m_DMC_cutout->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_cutout->GetDynamicMesh(),
		_primitiveOpts,
		_transform,
		m_cutoutWidth, m_cutoutHeight
	));
}

void ADynamicWallDoor::EXTRUDE_Cutout()
{
	check(m_DMC_cutout);

	FGeometryScriptMeshExtrudeOptions _extrudeOpts;
	_extrudeOpts.ExtrudeDistance = m_cutoutDepth;
	_extrudeOpts.ExtrudeDirection = FVector(0, 1, 0);

	m_DMC_cutout->SetDynamicMesh(UGeometryScriptLibrary_MeshModelingFunctions::ApplyMeshExtrude
	(
		m_DMC_cutout->GetDynamicMesh(),
		_extrudeOpts
	));
}

void ADynamicWallDoor::APPLY_CutoutBoolean()
{
	check(m_DMC_wall);
	check(m_DMC_cutout);

	FTransform _wallTransform;
	FTransform _cutoutTransform;

	_cutoutTransform.SetLocation(m_cutoutLocation);

	FGeometryScriptMeshBooleanOptions _meshBooleanOpts;

	m_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		m_DMC_wall->GetDynamicMesh(), 
		_wallTransform, 
		m_DMC_cutout->GetDynamicMesh(), 
		_cutoutTransform, 
		EGeometryScriptBooleanOperation::Subtract, 
		_meshBooleanOpts
	));
}

void ADynamicWallDoor::DESTROY_Cutout()
{
	//m_DMC_cutout->DestroyComponent();
	//m_DMC_cutout = nullptr;
}

// ****** SETTERS ****** //
//
void ADynamicWallDoor::SET_CutoutWidth(float p_width)
{
	if (p_width > 0.0f)
	{
		m_cutoutWidth = p_width;
	}
	else
	{
		UE_LOG(LogClass_DynamicWallDoor, Warning, TEXT("FUNCTION ''SET_CutoutWidth'' ===>>> PARAMETER ISSUE -->> p_width < 0"));
	}
}

void ADynamicWallDoor::SET_CutoutHeight(float p_height)
{
	if (p_height > 0.0f)
	{
		m_cutoutHeight = p_height;
	}
	else
	{
		UE_LOG(LogClass_DynamicWallDoor, Warning, TEXT("FUNCTION ''SET_CutoutHeight'' ===>>> PARAMETER ISSUE -->> p_height < 0"));
	}
}

void ADynamicWallDoor::SET_CutoutDepth(float p_depth)
{
	if (p_depth > 0.0f)
	{
		m_cutoutDepth = p_depth;
	}
	else
	{
		UE_LOG(LogClass_DynamicWallDoor, Warning, TEXT("FUNCTION ''SET_CutoutDepth'' ===>>> PARAMETER ISSUE -->> p_depth"));
	}
}

void ADynamicWallDoor::SET_CutoutLocation(const FVector& p_location)
{
	m_cutoutLocation = p_location;
}