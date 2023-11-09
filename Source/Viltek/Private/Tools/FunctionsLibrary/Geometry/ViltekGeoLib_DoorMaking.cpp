#include "Tools/FunctionsLibrary/Geometry/ViltekGeoLib_DoorMaking.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "UDynamicMesh.h"
#include "Engine/StaticMesh.h"
#include "DynamicMeshToMeshDescription.h"
//#include "UDynamicMeshActor.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "../Public/Tools/VTEK_StaticMesh.h"

DEFINE_LOG_CATEGORY(LogClass_ViktelGeoLib_DoorMaking);

FVGeoLibDoorPiercingOptions::FVGeoLibDoorPiercingOptions()
{

}

FVGeoLibDoorPiercingOptions::FVGeoLibDoorPiercingOptions(float p_WallLength, float p_WallHeight, float p_WallThickness) 
{
	if (p_WallLength > 0.0f)
	{
		s_WallLength = p_WallLength;
	}

	if (p_WallHeight > 0.0f)
	{
		s_WallHeight = p_WallHeight;
	}

	if (p_WallThickness > 0.0f)
	{
		s_WallThickness = p_WallThickness;
	}
}

float FVGeoLibDoorPiercingOptions::GET_DoorWidth() const
{
	return s_WidthOptions[(int)s_DoorWidth];
}

float FVGeoLibDoorPiercingOptions::GET_DoorHeight() const
{
	return s_HeightOptions[(int)s_DoorHeight];
}

bool FVGeoLibDoorPiercingOptions::IsWallSafeToPierce() const
{
	if (IsWidthSafe() == true && IsHeightSafe() == true)
	{
		return true;
	}

	return false;
}

bool FVGeoLibDoorPiercingOptions::IsWidthSafe() const
{
	if (GET_DoorWidth() + s_MinimumDoorToCornerDistance <= s_WallLength)
	{
		return true;
	}

	return false;
}

bool FVGeoLibDoorPiercingOptions::IsHeightSafe() const
{
	if (GET_DoorHeight() + s_MinimumDoorToCeilingDistance <= s_WallHeight)
	{
		return true;
	}
	
	return false;
}

UDynamicMesh* UViltekGeoLib_DoorMaking::PIERCE_BasicDoor
(
	UDynamicMesh* p_targetWall, 
	const FTransform& p_targetWallLocation, 
	UDynamicMesh* p_cutoutMesh, 
	const FTransform& p_cutoutLocation, 
	int8 p_targetWallAxis, 
	float p_targetWallLenght, 
	float p_targetWallThickness, 
	float p_targetWallHeight, 
	float p_doorWidth, 
	float p_doorHeight
)
{
	check(p_targetWall);
	check(p_cutoutMesh);

	FGeometryScriptPrimitiveOptions _primitiveOptions;
	FTransform _transform;

	if (p_targetWallAxis == 0) // Y axis
	{
		p_cutoutMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
		(
			p_cutoutMesh, 
			_primitiveOptions, 
			p_cutoutLocation, 
			p_targetWallThickness + 10.0f, 
			p_doorWidth, 
			p_doorHeight
		);
	}
	else
	{
		p_cutoutMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
		(
			p_cutoutMesh,
			_primitiveOptions,
			p_cutoutLocation,
			p_doorWidth,
			p_targetWallThickness + 10.0f, 
			p_doorHeight
		);
	}


	FGeometryScriptMeshBooleanOptions _booleanOptions;

	p_targetWall = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		p_targetWall, 
		_transform, 
		p_cutoutMesh, 
		_transform, 
		EGeometryScriptBooleanOperation::Subtract, 
		_booleanOptions
	);

	//p_cutoutMesh = nullptr;
	//p_cutoutMesh->ConditionalBeginDestroy();
	//p_cutoutMesh = nullptr;

	//p_targetWall->GetDynamic
	//UDynamicMeshComponent* _cutout;
	//_cutout->SetDynamicMesh
	return nullptr;
}

UDynamicMesh* UViltekGeoLib_DoorMaking::PIERCE_Door
(
	UDynamicMesh* p_TargetWall,
	const FVector& p_TargetWallLocation,
	const FVGeoLibDoorPiercingOptions& p_Options
)
{
	// Safety check 1
	if (p_TargetWall == nullptr)
	{
		UE_LOG(LogClass_ViktelGeoLib_DoorMaking, Error, TEXT("FUNCTON ''PIERCE_Door'' ==>> p_TargetWall is NULL !"));
		return p_TargetWall;
	}

	// Safety check 2 -- Width & Height
	if (p_Options.IsWallSafeToPierce() == false)
	{
		UE_LOG(LogClass_ViktelGeoLib_DoorMaking, Error, TEXT("FUNCTION ''PIERCE_Door'' ==>> p_TargetWall dimensions are too small for piercing !"));
		return p_TargetWall;
	}

	// Main function's body
	//
	FGeometryScriptPrimitiveOptions _PrimitiveOptions;
	FTransform _Transform;
	_Transform.SetLocation(p_TargetWallLocation);

	// Cutout mesh creation
	UDynamicMesh* _CutoutMesh = NewObject<UDynamicMesh>();
	check(_CutoutMesh);

	if (p_Options.s_bIsWallAxisNorthSouth == true)
	{
		_CutoutMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
		(
			_CutoutMesh, 
			_PrimitiveOptions, 
			_Transform, 
			p_Options.GET_DoorWidth(), 
			p_Options.s_WallThickness + 10.0f,
			p_Options.GET_DoorHeight()
		);
	}
	else
	{
		_CutoutMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
		(
			_CutoutMesh,
			_PrimitiveOptions,
			_Transform,
			p_Options.s_WallThickness + 10.0f,
			p_Options.GET_DoorWidth(),
			p_Options.GET_DoorHeight()
		);
	}

	// Boolean operation
	FGeometryScriptMeshBooleanOptions _BooleanOptions;

	p_TargetWall = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		p_TargetWall,
		_Transform,
		_CutoutMesh,
		_Transform,
		EGeometryScriptBooleanOperation::Subtract,
		_BooleanOptions
	);

	_CutoutMesh->ConditionalBeginDestroy();
	_CutoutMesh = nullptr;

	return p_TargetWall;
}

void UViltekGeoLib_DoorMaking::COPY_ToStaticMesh(UDynamicMesh* p_fromDynamicMesh, UStaticMesh* p_toStaticMeshAsset)
{
	//int32 _LODIndex = 0;
	/* Me trying to make things work */
	//FStaticMeshRenderData* _data = p_toStaticMeshAsset->GetRenderData();
	
	
	//
	//

	/*p_toStaticMeshAsset->SetFlags(RF_Transactional);

	if (WITH_EDITOR)
	{
		UE_LOG(LogTemp, Error, TEXT("We are with the editor here !"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("We are NOT with the editor here !"));
	}

	p_toStaticMeshAsset->Modify();

	p_toStaticMeshAsset->SetNumSourceModels(_LODIndex + 1);

	FStaticMeshSourceModel& _LODSourceModel = p_toStaticMeshAsset->GetSourceModel(_LODIndex);
	FMeshBuildSettings& _buildSettings = _LODSourceModel.BuildSettings;
	_buildSettings.bRecomputeNormals = false;
	_buildSettings.bRecomputeTangents = false;
	_buildSettings.bRemoveDegenerates = false;

	FMeshDescription* _meshDescription = p_toStaticMeshAsset->GetMeshDescription(_LODIndex);
	if (_meshDescription == nullptr)
	{
		_meshDescription = p_toStaticMeshAsset->CreateMeshDescription(_LODIndex);
	}

	p_toStaticMeshAsset->ModifyMeshDescription(_LODIndex);
	
	FConversionToMeshDescriptionOptions _conversionOptions;
	FDynamicMeshToMeshDescription _converter(_conversionOptions);
	p_fromDynamicMesh->ProcessMesh([&](const FDynamicMesh3& _readMesh)
	{
		_converter.Convert(&_readMesh, *_meshDescription, !_buildSettings.bRecomputeTangents);
	});

	FStaticMeshSourceModel& _thisSourceModel = p_toStaticMeshAsset->GetSourceModel(_LODIndex);
	_thisSourceModel.ReductionSettings.PercentTriangles = 1.0f;
	_thisSourceModel.ReductionSettings.PercentVertices = 1.0f;

	p_toStaticMeshAsset->CommitMeshDescription(_LODIndex);

	p_toStaticMeshAsset->PostEditChange();*/
}