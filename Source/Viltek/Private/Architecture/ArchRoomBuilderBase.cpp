#include "Architecture/ArchRoomBuilderBase.h"
#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "../../Public/Tools/Environment/Procedural/FillingCube.h"
#include "../../Public/Architecture/ArchSas.h"
#include "../../Public/Managers/BuilderManager.h"
#include "../../Public/Tools/FunctionsLibrary/Geometry/ViltekGeoLib_DoorMaking.h"
#include "../../Public/Tools/VolumeShell.h"

DEFINE_LOG_CATEGORY(LogClass_ArchRoomBuilderBase);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AArchRoomBuilderBase CLASS methods
//
AArchRoomBuilderBase::AArchRoomBuilderBase()
{

}

void AArchRoomBuilderBase::CREATE_RoomDynamicWallStructs()
{
	// First loop : creating new FRoomDynamicWall and adding them to the array
	for (int32 i = 0; i < m_ArchCorners.Num(); i++)
	{
		FRoomDynamicWall* _newRoomDynamicWall = new FRoomDynamicWall(m_ArchCorners[i]);

		if (_newRoomDynamicWall == nullptr)
		{
			UE_LOG(LogClass_ArchRoomBuilderBase, Error, TEXT("FUNCTION ''CREATE_RoomDynamicWalls'' ==>> _newRoomDynamicWall is NULL !"));
			return;
		}

		m_ArchDynamicWalls.Add(_newRoomDynamicWall);
	}

	int32 _roomDynamicWallsSize = m_ArchDynamicWalls.Num();

	// Second loop : setting up the leftover FRoomDynamicWall variables such as s_precedingWall, s_followingWall, s_wallNature...
	// Also : corrects the original walls location
	// 
	// ALSO : need to chance the UDynamicMeshComponent's attachment. No UDynamicMeshComponent should ever become the RootComponentfor (uint32 i = 0; i < _roomDynamicWallsSize; i++)
	for (int32 i = 0; i < _roomDynamicWallsSize; i++)
	{
		UDynamicMeshComponent* _DMC_newWall = NewObject<UDynamicMeshComponent>(this, UDynamicMeshComponent::StaticClass(),
			*FString("DMC_roomDynamicWall_" + FString::FromInt(i)));
		_DMC_newWall->RegisterComponent();
		_DMC_newWall->SetMobility(EComponentMobility::Movable);
		_DMC_newWall->SetGenerateOverlapEvents(false);
		_DMC_newWall->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		_DMC_newWall->CollisionType = ECollisionTraceFlag::CTF_UseSimpleAsComplex;
		_DMC_newWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		_DMC_newWall->EnableComplexAsSimpleCollision();
		_DMC_newWall->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		_DMC_newWall->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

		if (i == 0)
		{
			//SetRootComponent(_DMC_newWall);

			m_ArchDynamicWalls[i]->SET_PrecedingWall(m_ArchDynamicWalls[_roomDynamicWallsSize - 1]);
			m_ArchDynamicWalls[i]->SET_FollowingWall(m_ArchDynamicWalls[i + 1]);
		}
		else if (i == _roomDynamicWallsSize - 1)
		{
			m_ArchDynamicWalls[i]->SET_PrecedingWall(m_ArchDynamicWalls[i - 1]);
			m_ArchDynamicWalls[i]->SET_FollowingWall(m_ArchDynamicWalls[0]);
		}
		else
		{
			//_DMC_newWall->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			m_ArchDynamicWalls[i]->SET_PrecedingWall(m_ArchDynamicWalls[i - 1]);
			m_ArchDynamicWalls[i]->SET_FollowingWall(m_ArchDynamicWalls[i + 1]);
		}

		m_ArchDynamicWalls[i]->s_wallThickness = m_roomDynamicWallsThickness;
		m_ArchDynamicWalls[i]->s_wallHeight    = m_roomDynamicWallsHeight;
		m_ArchDynamicWalls[i]->s_DMC_wall = _DMC_newWall;
		m_ArchDynamicWalls[i]->DETERMINE_WallNature();
		m_ArchDynamicWalls[i]->DETERMINE_WallFlowingDirection();
		m_ArchDynamicWalls[i]->CORRECT_WallLocation();
	}
}

void AArchRoomBuilderBase::CREATE_GroundAndCeiling()
{
	// Safety check
	if (m_DMC_ground == nullptr)
	{
		UE_LOG(LogClass_ArchRoomBuilderBase, Error, TEXT("FUNCTION ''CREATE_Ground'' ==>> m_DMC_ground is NULL !"));
		return;
	}

	// Actual function's body
	//
	// Ground Dynamic Component creation
	//
	FGeometryScriptPrimitiveOptions _primitiveOptions;
	FTransform _defaultTransform;
	//_defaultTransform.SetLocation(FVector(0.0f, 0.0f, -1000.0f));

	m_DMC_ground->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		m_DMC_ground->GetDynamicMesh(),
		_primitiveOptions,
		_defaultTransform,
		(float)(m_roomLength * 100.0f),
		(float)(m_roomWidth * 100.0f),
		m_roomDynamicWallsThickness
	));

	UE_LOG(LogTemp, Warning, TEXT("m_splitCorners.Num() : %d"), m_splitCorners.Num());

	// "Modeling" the ground component
	//
	for (int32 i = 0; i < m_splitCorners.Num(); i++)
	{
		FTransform _modelingMeshTransform;

		// /!\ This is done so when the Builder is spawned somewhere else than FVector(0), everything still follows smoothly
		float _CounterPosX = GetActorLocation().X;
		float _CounterPosY = GetActorLocation().Y;
		float _CounterPosZ = GetActorLocation().Z;

		if (_CounterPosX != 0.0f)
		{
			_CounterPosX *= -1.0f;
		}

		if (_CounterPosY != 0.0f)
		{
			_CounterPosY *= -1.0f;
		}

		if (_CounterPosZ != 0.0f)
		{
			_CounterPosZ *= -1.0f;
		}

		_modelingMeshTransform.SetLocation(FVector(_CounterPosX, _CounterPosY, _CounterPosZ));

		if (m_splitCorners[i].s_splitType == ESplitType::InsideSplit)
		{
			// Creating the subtract mesh
			//
			UDynamicMesh* _subtractMesh = NewObject<UDynamicMesh>(this, UDynamicMesh::StaticClass(), TEXT("SubtractMesh"));

			_subtractMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
			(
				_subtractMesh,
				_primitiveOptions,
				_modelingMeshTransform,
				(float)(m_splitCorners[i].s_splitSurface.X * 100.0f),
				(float)(m_splitCorners[i].s_splitSurface.Y * 100.0f),
				m_roomDynamicWallsThickness
			);

			// Applying boolean subtraction to the ground component
			//
			_modelingMeshTransform.SetLocation(m_splitCorners[i].s_splitSurfaceCenterLocation);
			FGeometryScriptMeshBooleanOptions _booleanOptions;

			m_DMC_ground->SetDynamicMesh(UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
			(
				m_DMC_ground->GetDynamicMesh(),
				_defaultTransform,
				_subtractMesh,
				_modelingMeshTransform,
				EGeometryScriptBooleanOperation::Subtract,
				_booleanOptions
			));

			_subtractMesh->ConditionalBeginDestroy();
			_subtractMesh = nullptr;
		}

		if (m_splitCorners[i].s_splitType == ESplitType::OutsideSplit)
		{
			// Creating the union mesh
			//
			UDynamicMesh* _unionMesh = NewObject<UDynamicMesh>(this, UDynamicMesh::StaticClass(), TEXT("UnionMesh"));

			_unionMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
			(
				_unionMesh,
				_primitiveOptions,
				_modelingMeshTransform, 
				(float)(m_splitCorners[i].s_splitSurface.X * 100.0f),
				(float)(m_splitCorners[i].s_splitSurface.Y * 100.0f),
				m_roomDynamicWallsThickness
			);

			// Applying boolean union to the ground component
			//
			_modelingMeshTransform.SetLocation(m_splitCorners[i].s_splitSurfaceCenterLocation);
			FGeometryScriptMeshBooleanOptions _booleanOptions;

			m_DMC_ground->SetDynamicMesh(UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
			(
				m_DMC_ground->GetDynamicMesh(),
				_defaultTransform,
				_unionMesh,
				_modelingMeshTransform,
				EGeometryScriptBooleanOperation::Union,
				_booleanOptions
			));

			_unionMesh->ConditionalBeginDestroy();
			_unionMesh = nullptr;
		}
	}

	m_DMC_ground->SetRelativeLocation(m_DMC_ground->GetRelativeLocation() + FVector(0.0f, 0.0f, -m_roomDynamicWallsThickness));






	// Safety check 1
	if (m_DMC_ground == nullptr)
	{
		UE_LOG(LogClass_ArchRoomBuilderBase, Error, TEXT("FUNCTION ''CREATE_Ceiling'' ==>> m_DMC_ground is NULL !"));
		return;
	}

	// Safety check 2
	if (m_DMC_ceiling == nullptr)
	{
		UE_LOG(LogClass_ArchRoomBuilderBase, Error, TEXT("FUNCTION ''CREATE_Ceiling'' ==>> m_DMC_ceiling is NULL !"));
		return;
	}

	m_DMC_ceiling->SetDynamicMesh(m_DMC_ground->GetDynamicMesh());
	m_DMC_ceiling->SetRelativeLocation(m_DMC_ceiling->GetRelativeLocation() + FVector(0.0f, 0.0f, m_roomDynamicWallsHeight));
}

FVector AArchRoomBuilderBase::COMPUTE_FirstCubeElementLocation()
{
	FVector _FirstElementLoc;

	if (m_ArchDynamicWalls[0]->s_wallFacingDirection == ECardinalDirection::North)
	{
		if (m_ArchDynamicWalls[0]->s_wallFlowingDirection == ECardinalDirection::East)
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, -50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, -50.0f, 50.0f);
		}
		else //West
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, 50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, 50.0f, 50.0f);
		}
	}

	if (m_ArchDynamicWalls[0]->s_wallFacingDirection == ECardinalDirection::South)
	{
		if (m_ArchDynamicWalls[0]->s_wallFlowingDirection == ECardinalDirection::East)
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, 50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, -50.0f, 50.0f);
		}
		else
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, -50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, -50.0f, 50.0f);
		}
	}

	if (m_ArchDynamicWalls[0]->s_wallFacingDirection == ECardinalDirection::East)
	{
		if (m_ArchDynamicWalls[0]->s_wallFlowingDirection == ECardinalDirection::North)
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, -50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, -50.0f, 50.0f);
		}
		else
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, -50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, -50.0f, 50.0f);
		}
	}

	if (m_ArchDynamicWalls[0]->s_wallFacingDirection == ECardinalDirection::West)
	{
		if (m_ArchDynamicWalls[0]->s_wallFlowingDirection == ECardinalDirection::North)
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, -50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(50.0f, -50.0f, 50.0f);
		}
		else
		{
			DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, -50.0f, 50.0f), 25.0f, 10, FColor::Blue, true, 2, 5.0f);
			_FirstElementLoc = m_ArchCorners[0]->s_cornerLocation + FVector(-50.0f, -50.0f, 50.0f);
		}
	}

	return _FirstElementLoc;
}

void AArchRoomBuilderBase::SPAWN_CubeElements(const FVector& p_FirstElementLoc)
{
	AFillingCube* _TestFillingCube = GetWorld()->SpawnActor<AFillingCube>(p_FirstElementLoc, FRotator(0));
	check(_TestFillingCube);
	_TestFillingCube->SHOOT_Rays();
	m_RoomFillingCubes.Add(_TestFillingCube);

	for (int32 i = 0; i < _TestFillingCube->GET_FriendCubesArray().Num(); i++)
	{
		m_RoomFillingCubes.Add(_TestFillingCube->GET_FriendCubesArray()[i]);
	}

	bool _bIsRoomFilled = false;
	int32 _LoopCount = 0;

	while (_bIsRoomFilled == false)
	{
		int32 _CurrentArrayCount = m_RoomFillingCubes.Num();
		UE_LOG(LogClass_ArchRoomBuilderBase, Warning, TEXT("Current array count : %d"), _CurrentArrayCount);

		for (int32 i = 0; i < _CurrentArrayCount; i++)
		{
			AFillingCube* _TestingCube = m_RoomFillingCubes[i];

			if (_TestingCube->CHECK_IsEveryFriendFound() == false)
			{
				_TestingCube->SHOOT_Rays();
				//m_RoomFillingCubes.Add(_TestingCube);

				for (int8 j = 0; j < _TestingCube->GET_FriendCubesArray().Num(); j++)
				{
					m_RoomFillingCubes.Add(_TestingCube->GET_FriendCubesArray()[j]);
				}
			}
		}

		for (int32 i = 0; i < m_RoomFillingCubes.Num(); i++)
		{
			//m_RoomFillingCubes[i]->SHOOT_Rays();
			m_RoomFillingCubes[i]->UPDATE_CubeColor();
		}

		UE_LOG(LogTemp, Warning, TEXT("UPDATED Array Count : %d"), m_RoomFillingCubes.Num());

		int32 _CheckingCounter = 0;

		for (int32 i = 0; i < m_RoomFillingCubes.Num(); i++)
		{
			if (m_RoomFillingCubes[i]->CHECK_IsEveryFriendFound() == false)
			{
				_CheckingCounter++;
			}
		}

		if (_CheckingCounter == 0)
		{
			_bIsRoomFilled = true;
		}

		_LoopCount++;

		if (m_RoomFillingCubes.Num() > 1500)
		{
			break;
		}
	}

	UE_LOG(LogClass_ArchRoomBuilderBase, Warning, TEXT("Loop count : %d"), _LoopCount);

	UE_LOG(LogClass_ArchRoomBuilderBase, Warning, TEXT("There are now %d AFillingCube instances that were spawned !"), m_RoomFillingCubes.Num());
}

void AArchRoomBuilderBase::PIERCE_Door()
{
	TArray<FRoomDynamicWall*> _PotentialDoorWalls;

	for (int32 i = 0; i < m_ArchDynamicWalls.Num(); i++)
	{
		if ((int)m_ArchDynamicWalls[i]->s_wallLength > 400 && m_ArchDynamicWalls[i]->s_wallNature == EWallNature::OutsideWall) // Should probably switch that to meters for consistency
		{
			_PotentialDoorWalls.Add(m_ArchDynamicWalls[i]);
		}
	}

	int32 _ArrayNum = _PotentialDoorWalls.Num();

	if (_ArrayNum == 0)
	{
		UE_LOG(LogClass_ArchRoomBuilderBase, Error, TEXT("FUNCTION ''PIERCE_Door'' ==>> No suitable wall for Door Piercing !"));
		return;
	}

	int32 _WallToPierceIndex = FMath::RandRange(0, _ArrayNum - 1);

	UE_LOG(LogTemp, Warning, TEXT("We are about to pierce %s"), *_PotentialDoorWalls[_WallToPierceIndex]->s_DMC_wall->GetName());

	FRoomDynamicWall* _WallToPierce = _PotentialDoorWalls[_WallToPierceIndex];
	check(_WallToPierce);

	// Setting the piercing options
	FVGeoLibDoorPiercingOptions _DoorPiercingOpts(_WallToPierce->s_wallLength, _WallToPierce->s_wallHeight, _WallToPierce->s_wallThickness);
	if (_WallToPierce->s_wallAxis == ECardinalAxis::NorthSouth)
	{
		_DoorPiercingOpts.s_bIsWallAxisNorthSouth = true;
	}
	else
	{
		_DoorPiercingOpts.s_bIsWallAxisNorthSouth = false;
	}
	_DoorPiercingOpts.s_DoorWidth = EDoorWidthOptions::Narrow;
	//_DoorPiercingOpts.s_DoorHeight = EDoorHeightOptions::Low;

	DrawDebugSphere(GetWorld(), _WallToPierce->s_wallLocation, 50.0f, 12, FColor::Orange, true, 0, 5.0f);

	UViltekGeoLib_DoorMaking::PIERCE_Door(_WallToPierce->s_DMC_wall->GetDynamicMesh(), _WallToPierce->s_wallLocation, _DoorPiercingOpts);

	_WallToPierce->s_bHasBeenPierced = true;

	m_DoorLocation  = _WallToPierce->s_wallLocation;
	m_DoorDirection = _WallToPierce->s_wallFacingDirection;

	// Need to improve this part
	/*FRotator _SasRot;
	FVector _LocationFixer;

	if (_WallToPierce->s_wallFacingDirection == ECardinalDirection::North)
	{
		_SasRot = FRotator(0.0f, 90.0f, 0.0f);
		_LocationFixer = FVector(m_roomDynamicWallsThickness / 2.0f, 0.0f, 0.0f);
	}
	else if (_WallToPierce->s_wallFacingDirection == ECardinalDirection::South)
	{
		_SasRot = FRotator(0.0f, -90.0f, 0.0f);
		_LocationFixer = FVector(- m_roomDynamicWallsThickness / 2.0f, 0.0f, 0.0f);
	}
	else if (_WallToPierce->s_wallFacingDirection == ECardinalDirection::East)
	{
		_SasRot = FRotator(0.0f, 180.0f, 0.0f);
		_LocationFixer = FVector(0.0f, m_roomDynamicWallsThickness / 2.0f, 0.0f);
	}
	else
	{
		_SasRot = FRotator(0.0f, 0.0f, 0.0f);
		_LocationFixer = FVector(0.0f, - m_roomDynamicWallsThickness / 2.0f, 0.0f);
	}


	m_ArchSas = GetWorld()->SpawnActor<AArchSas>(_WallToPierce->s_wallLocation + _LocationFixer, _SasRot);
	m_builderManager->SET_ArchSas(m_ArchSas);*/


	// SAS


	//m_ArchSas = GetWorld()->SpawnActor<AArchSas>(FVector(0.0f, 0.0f, 1000.0f), FRotator(0));
	//m_builderManager->SET_ArchSas(m_ArchSas);
	//m_ArchSas->SET_Rotation(_WallToPierce->s_wallFacingDirection, m_roomDynamicWallsThickness, _WallToPierce->s_wallLocation);

	UE_LOG(LogTemp, Warning, TEXT("WALL TO PIERCE Facing Direction : %d"), (int)_WallToPierce->s_wallFacingDirection);
}

void AArchRoomBuilderBase::COMPUTE_RoomNewDimensions(uint32 p_RoomExteriorDistance)
{
	// Note : so far, we only manually affect the height of the room, which is therefore kept constant throughout the whole building process.
	// Consequently, the _LargestZ and _LowestZ are unused until this is changed

	FVector _ArchCornerLoc = m_ArchCorners[0]->s_cornerLocation;

	float _LargestX = _ArchCornerLoc.X ;
	float _LargestY = _ArchCornerLoc.Y ;
	float _LargestZ = _ArchCornerLoc.Z ;
	float _LowestX  = _ArchCornerLoc.X ;
	float _LowestY  = _ArchCornerLoc.Y ;
	float _LowestZ  = _ArchCornerLoc.Z ;

	for (int32 i = 0; i < m_ArchCorners.Num(); i++)
	{
		_ArchCornerLoc = m_ArchCorners[i]->s_cornerLocation;

		if (_ArchCornerLoc.X > _LargestX)
		{
			_LargestX = _ArchCornerLoc.X;
		}

		if (_ArchCornerLoc.X < _LowestX)
		{
			_LowestX = _ArchCornerLoc.X;
		}

		if (_ArchCornerLoc.Y > _LargestY)
		{
			_LargestY = _ArchCornerLoc.Y;
		}

		if (_ArchCornerLoc.Y < _LowestY)
		{
			_LowestY = _ArchCornerLoc.Y;
		}

		if (_ArchCornerLoc.Z > _LargestZ)
		{
			_LargestZ = _ArchCornerLoc.Z;
		}

		if (_ArchCornerLoc.Z < _LowestZ)
		{
			_LowestZ = _ArchCornerLoc.Z;
		}
	}

	_LargestX += (float)(p_RoomExteriorDistance) * 100.0f ;
	_LargestY += (float)(p_RoomExteriorDistance) * 100.0f ;
	_LowestX  -= (float)(p_RoomExteriorDistance) * 100.0f ;
	_LowestY  -= (float)(p_RoomExteriorDistance) * 100.0f ;

	// As of now, Z is a bit special, so this is how we currently deal with it
	_LargestZ += m_roomDynamicWallsHeight + (float)(p_RoomExteriorDistance) * 100.0f ;
	_LowestZ  -= (float)(p_RoomExteriorDistance) * 100.0f ;

	// Bottom 
	DrawDebugSphere(GetWorld(), FVector(_LargestX, _LargestY, _LowestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Top Right
	DrawDebugSphere(GetWorld(), FVector(_LowestX , _LargestY, _LowestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Bottom Right
	DrawDebugSphere(GetWorld(), FVector(_LowestX , _LowestY , _LowestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Bottom Left
	DrawDebugSphere(GetWorld(), FVector(_LargestX, _LowestY , _LowestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Top Left

	// Top
	DrawDebugSphere(GetWorld(), FVector(_LargestX, _LargestY, _LargestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Top Right
	DrawDebugSphere(GetWorld(), FVector(_LowestX , _LargestY, _LargestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Bottom Right
	DrawDebugSphere(GetWorld(), FVector(_LowestX , _LowestY , _LargestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Bottom Left
	DrawDebugSphere(GetWorld(), FVector(_LargestX, _LowestY , _LargestZ), 35.0f, 20, FColor::Black, true, 0, 10.0f); // Top Left

	//
	//

	FVector _TopRight_Bottom = FVector(_LargestX , _LargestY , _LowestZ  );
	FVector _BottomLeft_Top  = FVector(_LowestX  , _LowestY  , _LargestZ );

	FVector _MiddlePoint = (_TopRight_Bottom - _BottomLeft_Top) / 2.0f + _BottomLeft_Top;

	m_VolumeShell = GetWorld()->SpawnActor<AVolumeShell>(_MiddlePoint, FRotator(0));
	m_VolumeShell->COMPUTESET_VolumeDimensions(_LargestX, _LargestY, _LargestZ, _LowestX, _LowestY, _LowestZ);
	m_VolumeShell->GENERATE_RoomShell();
}

const FVector& AArchRoomBuilderBase::GET_DoorLocation() const
{
	return m_DoorLocation;
}

ECardinalDirection AArchRoomBuilderBase::GET_DoorDirection() const
{
	return m_DoorDirection;
}