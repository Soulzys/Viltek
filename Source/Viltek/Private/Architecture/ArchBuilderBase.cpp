#include "Architecture/ArchBuilderBase.h"
#include "Components/DynamicMeshComponent.h"
#include "../../Public/Managers/BuilderManager.h"
#include "../../Public/Managers/LevelSpaceManager.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "../../Public/Tools/FunctionsLibrary/Geometry/MeshConvertor.h"
//#include "../../Public/Managers/LevelSpaceManager.h"

DEFINE_LOG_CATEGORY(LogStruct_RoomCorner       ) ;
DEFINE_LOG_CATEGORY(LogStruct_SplitCorner      ) ;
DEFINE_LOG_CATEGORY(LogStruct_RoomDynamicWall  ) ;
DEFINE_LOG_CATEGORY(LogStruct_RoomStaticWall   ) ;
DEFINE_LOG_CATEGORY(LogStruct_RoomFillingEntity) ;
DEFINE_LOG_CATEGORY(LogClass_ArchBuilderBase   ) ;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FArchCorner struct methods
//
FArchCorner::FArchCorner()
{

}

FArchCorner::FArchCorner(const FVector& p_cornerLocation)
{
	s_cornerLocation = p_cornerLocation;
}

void FArchCorner::SET_PrecedingCorner(FArchCorner* p_precedingCorner)
{
	if (p_precedingCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomCorner, Error, TEXT("FUNCTION ''SET_PrecedingCorner'' ==>> p_precedingCorner is NULL !"));
		return;
	}

	s_precedingCorner                 = p_precedingCorner;
	s_directionToPrecedingCornerEnum  = COMPUTE_CornerDirectionEnum (s_precedingCorner);
	s_directionToPrecedingCornerPoint = COMPUTE_CornerDirectionPoint(s_directionToPrecedingCornerEnum);
	s_distanceToPrecedingCorner       = COMPUTE_DistanceToCorner    (s_precedingCorner, s_directionToPrecedingCornerEnum);
}

void FArchCorner::SET_FollowingCorner(FArchCorner* p_followingCorner)
{
	if (p_followingCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomCorner, Error, TEXT("FUNCTION ''SET_FollowingCorner'' ==>> p_followingCorner is NULL !"));
		return;
	}

	s_followingCorner                 = p_followingCorner;
	s_directionToFollowingCornerEnum  = COMPUTE_CornerDirectionEnum (s_followingCorner);
	s_directionToFollowingCornerPoint = COMPUTE_CornerDirectionPoint(s_directionToFollowingCornerEnum);
	s_distanceToFollowingCorner       = COMPUTE_DistanceToCorner    (s_followingCorner, s_directionToFollowingCornerEnum);
}

ECardinalDirection FArchCorner::COMPUTE_CornerDirectionEnum(FArchCorner* p_corner)
{
	if (p_corner == nullptr)
	{
		UE_LOG(LogStruct_RoomCorner, Error, TEXT("FUNCTION ''COMPUTE_CornerDirection'' ==>> p_corner is NULL !"));
		return ECardinalDirection::Unknown;
	}

	FVector _cornerLocation = p_corner->s_cornerLocation;

	if (s_cornerLocation.X == _cornerLocation.X) // If on X axis (North-South axis)
	{
		if (s_cornerLocation.Y > _cornerLocation.Y) // If on its "left"
		{
			return ECardinalDirection::West;
		}
		else // If on its "right"
		{
			return ECardinalDirection::East;
		}
	}
	else // If on Y axis (East-West axis)
	{
		if (s_cornerLocation.X > _cornerLocation.X) // If "below"
		{
			return ECardinalDirection::South;
		}
		else // If "above"
		{
			return ECardinalDirection::North;
		}
	}
}

FIntPoint FArchCorner::COMPUTE_CornerDirectionPoint(ECardinalDirection p_cornerDirectionEnum)
{
	if (p_cornerDirectionEnum == ECardinalDirection::North)
	{
		return FIntPoint(1, 1);
	}
	else if (p_cornerDirectionEnum == ECardinalDirection::South)
	{
		return FIntPoint(1, -1);
	}
	else if (p_cornerDirectionEnum == ECardinalDirection::East)
	{
		return FIntPoint(0, 1);
	}
	else if (p_cornerDirectionEnum == ECardinalDirection::West)
	{
		return FIntPoint(0, -1);
	}
	else
	{
		UE_LOG(LogStruct_RoomCorner, Error, TEXT("FUNCTION ''COMPUTE_CornerDirectionPoint'' ==>> p_cornerDirection is unknown !"));

		return FIntPoint(-2, -2); // Should never happen
	}
}

uint32 FArchCorner::COMPUTE_DistanceToCorner(FArchCorner* p_corner, ECardinalDirection p_directionToCorner)
{
	if (p_corner == nullptr)
	{
		UE_LOG(LogStruct_RoomCorner, Error, TEXT("FUNCTION ''COMPUTE_DistanceToCorner'' ==>> p_corner is NULL !"));
		return -1;
	}

	if (p_directionToCorner == ECardinalDirection::Unknown)
	{
		UE_LOG(LogStruct_RoomCorner, Error, TEXT("FUNCTION ''COMPUTE_DistanceToCorner'' ==>> p_directionToCorner is unknown !"));
		return -1;
	}

	FVector _otherCornerLocationVector = p_corner->s_cornerLocation;
	uint32  _ownCornerLocation = 0;
	uint32  _otherCornerLocation = 0;

	if (p_directionToCorner == ECardinalDirection::North)
	{
		_ownCornerLocation = (uint32)(s_cornerLocation.X / 100);
		_otherCornerLocation = (uint32)(_otherCornerLocationVector.X / 100);

		return _otherCornerLocation - _ownCornerLocation;
	}
	else if (p_directionToCorner == ECardinalDirection::South)
	{
		_ownCornerLocation = (uint32)(s_cornerLocation.X / 100);
		_otherCornerLocation = (uint32)(_otherCornerLocationVector.X / 100);

		return _ownCornerLocation - _otherCornerLocation;
	}
	else if (p_directionToCorner == ECardinalDirection::East)
	{
		_ownCornerLocation = (uint32)(s_cornerLocation.Y / 100);
		_otherCornerLocation = (uint32)(_otherCornerLocationVector.Y / 100);

		return _otherCornerLocation - _ownCornerLocation;
	}
	else if (p_directionToCorner == ECardinalDirection::West)
	{
		_ownCornerLocation = (uint32)(s_cornerLocation.Y / 100);
		_otherCornerLocation = (uint32)(_otherCornerLocationVector.Y / 100);

		return _ownCornerLocation - _otherCornerLocation;
	}
	else // Should never happen
	{
		UE_LOG(LogStruct_RoomCorner, Error, TEXT("FUNCTION ''COMPUTE_DistanceToCorner'' ==>> p_directionToCorner is unknown !"));
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FSplitCorner struct methods
//
FSplitCorner::FSplitCorner()
{

}

FSplitCorner::FSplitCorner(bool p_bIsSplitting)
{
	s_bIsSplitting = p_bIsSplitting;
}

FSplitCorner::FSplitCorner(const FIntPoint& p_splitSurface, const FVector& p_splitSurfaceCenterLocation, bool p_bIsSplitting)
{
	if (p_splitSurface.X < 0 || p_splitSurface.Y < 0)
	{
		UE_LOG(LogStruct_SplitCorner, Error, TEXT("FUNCTION ''FSplitCorner'' ==>> The split surface cannot be negative !"));
		return;
	}

	s_splitSurface               = p_splitSurface               ;
	s_splitSurfaceCenterLocation = p_splitSurfaceCenterLocation ;
	s_bIsSplitting               = p_bIsSplitting               ;
}

void FSplitCorner::TDEBUG_FullStructInfo()
{
	FString _splitType = "";

	switch (static_cast<int>(s_splitType))
	{
	case 0:
		_splitType = "InsideSplit";
		break;

	case 1:
		_splitType = "OutsideSplit";
		break;

	case 2:
		_splitType = "None";
		break;
	}

	UE_LOG(LogStruct_SplitCorner, Log, TEXT("s_splitSurface               -->> %s"), *s_splitSurface.ToString()                   ) ;
	UE_LOG(LogStruct_SplitCorner, Log, TEXT("s_splitSurfaceCenterLocation -->> %s"), *s_splitSurfaceCenterLocation.ToString()     ) ;
	UE_LOG(LogStruct_SplitCorner, Log, TEXT("s_splitType                  -->> %s"), *_splitType                                  ) ;
	UE_LOG(LogStruct_SplitCorner, Log, TEXT("s_bIsSplitting               -->> %s"), s_bIsSplitting ? TEXT("TRUE") : TEXT("FALSE")) ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FRoomDynamicWall struct methods
//
FRoomDynamicWall::FRoomDynamicWall()
{

}

FRoomDynamicWall::FRoomDynamicWall(FArchCorner* p_startCorner)
{
	if (p_startCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''FRoomDynamicWall'' ==>> p_startCorner is NULL !"));
		return;
	}

	s_startCorner = p_startCorner;
	s_endCorner   = s_startCorner->s_followingCorner;

	s_wallLocation        = COMPUTE_WallLocation       (s_startCorner, s_endCorner            ) ;
	s_wallFacingDirection = COMPUTE_WallFacingDirection(s_startCorner                         ) ;
	s_wallAxis            = COMPUTE_WallAxis           (s_startCorner                         ) ;
	s_wallLength          = COMPUTE_WallLength         (s_startCorner, s_endCorner, s_wallAxis) ;
}

FVector FRoomDynamicWall::COMPUTE_WallLocation(FArchCorner* p_startCorner, FArchCorner* p_endCorner)
{
	// Safety check 1
	if (p_startCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallLocation'' ==>> p_startCorner is NULL !"));
		return FVector(0);
	}

	// Safety check 2
	if (p_endCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallLocation'' ==>> p_endCorner is NULL !"));
		return FVector(0);
	}

	// Actual function's body
	FVector _wallLocation = p_startCorner->s_cornerLocation +
		FVector
		(
			(FMath::Abs(p_startCorner->s_cornerLocation.X - p_endCorner->s_cornerLocation.X) / 2.0f) * p_startCorner->s_directionToFollowingCornerPoint.Y,
			(FMath::Abs(p_startCorner->s_cornerLocation.Y - p_endCorner->s_cornerLocation.Y) / 2.0f) * p_startCorner->s_directionToFollowingCornerPoint.Y,
			0.0f
		);

	return _wallLocation;
}

ECardinalDirection FRoomDynamicWall::COMPUTE_WallFacingDirection(FArchCorner* p_startCorner)
{
	// Safety check 1
	if (p_startCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallFacingDirection'' ==>> p_startCorner is NULL !"));
		return ECardinalDirection::Unknown;
	}

	// Safety check 2
	if (p_startCorner->s_directionToPrecedingCornerEnum == ECardinalDirection::Unknown ||
		p_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::Unknown)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallFacingDirection'' ==>> p_startCorner direction to preceding or following corner is Unknown !"));
		return ECardinalDirection::Unknown;
	}

	// Actual function's body
	if (p_startCorner->s_directionToPrecedingCornerEnum == ECardinalDirection::North ||
		p_startCorner->s_directionToPrecedingCornerEnum == ECardinalDirection::South)
	{
		if (p_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::East)
		{
			return ECardinalDirection::North;
		}
		else // if s_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::West
		{
			return ECardinalDirection::South;
		}
	}
	else 
	{
		if (p_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::North)
		{
			return ECardinalDirection::West;
		}
		else // if s_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::South
		{
			return ECardinalDirection::East;
		}
	}
}

ECardinalAxis FRoomDynamicWall::COMPUTE_WallAxis(FArchCorner* p_startCorner)
{
	// Safety check 1
	if (p_startCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallAxis'' ==>> p_startCorner is NULL !"));
		return ECardinalAxis::Unknown;
	}

	// Safety check 2
	if (p_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::Unknown)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallAxis'' ==>> p_startCorner->s_directionToFollowingCornerEnum is Unknown !"));
		return ECardinalAxis::Unknown;
	}

	// Actual function's body
	if (p_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::North ||
		p_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::South)
	{
		return ECardinalAxis::NorthSouth;
	}
	else
	{
		return ECardinalAxis::EastWest;
	}
}

float FRoomDynamicWall::COMPUTE_WallLength(FArchCorner* p_startCorner, FArchCorner* p_endCorner, ECardinalAxis p_wallAxis)
{
	// Safety check 1
	if (p_startCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallLength'' ==>> p_startCorner is NULL !"));
		return -1.0f;
	}

	// Safety check 2
	if (p_endCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallLength'' ==>> p_endCorner is NULL !"));
		return -1.0f;
	}

	// Safety check 3
	if (p_wallAxis == ECardinalAxis::Unknown)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''COMPUTE_WallLength'' ==>> p_wallAxis is Unknown !"));
		return -1.0f;
	}

	// Actual function's body
	if (p_wallAxis == ECardinalAxis::NorthSouth)
	{
		return FMath::Abs(p_startCorner->s_cornerLocation.X - p_endCorner->s_cornerLocation.X);
	}
	else
	{
		return FMath::Abs(p_startCorner->s_cornerLocation.Y - p_endCorner->s_cornerLocation.Y);
	}
}

void FRoomDynamicWall::SET_PrecedingWall(FRoomDynamicWall* p_precedingWall)
{
	if (p_precedingWall == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''SET_PrecedingWall'' ==>> p_precedingWall is NULL !"));
		return;
	}

	s_precedingWall = p_precedingWall;
}

void FRoomDynamicWall::SET_FollowingWall(FRoomDynamicWall* p_followingWall)
{
	if (p_followingWall == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''SET_FollowingWall'' ==>> p_followingWall is NULL !"));
		return;
	}

	s_followingWall = p_followingWall;
}

void FRoomDynamicWall::DETERMINE_WallNature()
{
	// Safety check 1
	if (s_precedingWall == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''DETERMINE_WallNature'' ==>> s_precedingWall is NULL !"));
		return;
	}

	// Safety check 2
	if (s_followingWall == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''DETERMINE_WallNature'' ==>> s_followingWall is NULL !"));
		return;
	}

	// Safety check 3
	if (s_wallFacingDirection == ECardinalDirection::Unknown)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''DETERMINE_WallNature'' ==>> s_wallDirection is Unknown !"));
		return;
	}


	// Actual function's body
	//
	bool _bIsPreviousCornerIntersecting;
	bool _bIsFollowingCornerIntersecting;

	int8 _intersectionCounter = 0;

	// NORTH
	if (s_wallFacingDirection == ECardinalDirection::North)
	{
		// Checking previous wall
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::East)
		{
			_bIsPreviousCornerIntersecting = true;
		}
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::West)
		{
			_bIsPreviousCornerIntersecting = false;
		}

		// Checking following wall
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::East)
		{
			_bIsFollowingCornerIntersecting = false;
		}
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::West)
		{
			_bIsFollowingCornerIntersecting = true;
		}
	}

	// SOUTH
	if (s_wallFacingDirection == ECardinalDirection::South)
	{
		// Checking previous wall
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::East)
		{
			_bIsPreviousCornerIntersecting = false;
		}
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::West)
		{
			_bIsPreviousCornerIntersecting = true;
		}

		// Checking following wall
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::East)
		{
			_bIsFollowingCornerIntersecting = true;
		}
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::West)
		{
			_bIsFollowingCornerIntersecting = false;
		}
	}

	// EAST
	if (s_wallFacingDirection == ECardinalDirection::East)
	{
		// Checking previous wall
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::North)
		{
			_bIsPreviousCornerIntersecting = false;
		}
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::South)
		{
			_bIsPreviousCornerIntersecting = true;
		}

		// Checking following wall
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::North)
		{
			_bIsFollowingCornerIntersecting = true;
		}
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::South)
		{
			_bIsFollowingCornerIntersecting = false;
		}
	}

	// WEST
	if (s_wallFacingDirection == ECardinalDirection::West)
	{
		// Checking previous wall
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::North)
		{
			_bIsPreviousCornerIntersecting = true;
		}
		if (s_precedingWall->s_wallFacingDirection == ECardinalDirection::South)
		{
			_bIsPreviousCornerIntersecting = false;
		}

		// Checking following wall
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::North)
		{
			_bIsFollowingCornerIntersecting = false;
		}
		if (s_followingWall->s_wallFacingDirection == ECardinalDirection::South)
		{
			_bIsFollowingCornerIntersecting = true;
		}
	}

	//

	if (_bIsPreviousCornerIntersecting)
	{
		_intersectionCounter++;
	}
	if (_bIsFollowingCornerIntersecting)
	{
		_intersectionCounter++;
	}

	//

	if (_intersectionCounter == 2)
	{
		s_wallNature = EWallNature::InsideWall;
	}
	if (_intersectionCounter == 1)
	{
		s_wallNature = EWallNature::InbetweenWall;
	}
	if (_intersectionCounter == 0)
	{
		s_wallNature = EWallNature::OutsideWall;
	}
}

void FRoomDynamicWall::DETERMINE_WallFlowingDirection()
{
	// Safety check 1
	if (s_startCorner == nullptr)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''DETERMINE_WallFlowingDirection'' ==>> s_startCorner is NULL !"));
		return;
	}

	// Safety check 2
	if (s_startCorner->s_directionToFollowingCornerEnum == ECardinalDirection::Unknown)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''DETERMINE_WallFlowingDirection'' ==>> s_startCorner->s_directionToFollowingCornerEnum is Unknown !"));
		return;
	}

	s_wallFlowingDirection = s_startCorner->s_directionToFollowingCornerEnum;
}

void FRoomDynamicWall::CORRECT_WallLocation()
{
	// Safety check
	if (s_wallThickness == 0.0f)
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''CORRECT_WallLocation'' ==>> s_wallThickness was not initialized !"));
		return;
	}

	if (s_wallFacingDirection == ECardinalDirection::North)
	{
		s_wallLocation += FVector(s_wallThickness / 2.0f, 0.0f, 0.0f);
	}
	else if (s_wallFacingDirection == ECardinalDirection::South)
	{
		s_wallLocation += FVector(- s_wallThickness / 2.0f, 0.0f, 0.0f);
	}
	else if (s_wallFacingDirection == ECardinalDirection::East)
	{
		s_wallLocation += FVector(0.0f, s_wallThickness / 2.0f, 0.0f);
	}
	else if (s_wallFacingDirection == ECardinalDirection::West)
	{
		s_wallLocation += FVector(0.0f, - s_wallThickness / 2.0f, 0.0f);
	}
	else
	{
		UE_LOG(LogStruct_RoomDynamicWall, Error, TEXT("FUNCTION ''CORRECT_WallLocation'' ==>> s_wallLocation is Unknown !"));
		return;
	}
}

bool FRoomDynamicWall::CLEAN_StructData()
{
	if (s_DMC_wall)
	{
		if (s_DMC_wall->IsRegistered())
		{
			s_DMC_wall->UnregisterComponent();
		}

		s_DMC_wall->ConditionalBeginDestroy();
		s_DMC_wall = nullptr;
	}

	if (s_startCorner)
	{
		delete s_startCorner;
		s_startCorner = nullptr;
	}

	if (s_endCorner)
	{
		delete s_endCorner;
		s_endCorner = nullptr;
	}

	/*if (s_precedingWall)
	{
		if (s_precedingWall->CLEAN_StructData())
		{
			s_precedingWall = nullptr;
		}
		else
		{
			return false;
		}
	}

	if (s_followingWall)
	{
		if (s_followingWall->CLEAN_StructData())
		{
			s_followingWall = nullptr;
		}
		else
		{
			return false;
		}
	}*/

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FRoomStaticWall struct methods
//
FRoomStaticWall::FRoomStaticWall()
{

}

FRoomStaticWall::FRoomStaticWall(const FRoomDynamicWall& p_DynamicWall)
{
	s_WallLength    = p_DynamicWall.s_wallLength    ;
	s_WallThickness = p_DynamicWall.s_wallThickness ;
	s_WallHeight    = p_DynamicWall.s_wallHeight    ;
}

void FRoomStaticWall::SET_WallMeshComponent(UStaticMeshComponent* p_Component)
{
	if (p_Component == nullptr)
	{
		UE_LOG(LogStruct_RoomStaticWall, Error, TEXT("FUNCTION ''SET_WallMeshComponent'' ==>> p_Component is NULL !"));
		return;
	}

	s_SMC_Wall = p_Component;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// FRoomFillingEntity struct methods
//
FRoomFillingEntity::FRoomFillingEntity()
{

}

FRoomFillingEntity::FRoomFillingEntity(UWorld* p_World, const FVector& p_EntityLocation)
{
	if (p_World != nullptr)
	{
		s_EntityLocation = p_EntityLocation;
		s_World = p_World;

		DrawDebugBox(s_World, p_EntityLocation, FVector(15.0f), FColor::Green, true, 0, 5.0f);
	}	
}

EFillingEntityFriendType FRoomFillingEntity::SHOOT_Ray(UWorld* p_World, const FVector& p_RayTargetLoc)
{
	if (p_World == nullptr)
	{
		UE_LOG(LogStruct_RoomFillingEntity, Error, TEXT("FUNCTION ''SHOOT_Ray'' ==>> World parameter is NULL !"));
		return EFillingEntityFriendType::Error;
	}

	FHitResult _HitResult;

	FVector _RayStartLoc = s_EntityLocation;
	FVector _RayEndLoc = p_RayTargetLoc + _RayStartLoc;

	FCollisionQueryParams _QueryParams;
	_QueryParams.bTraceComplex = true; // Is this necessary ? So far, we are only using simple collisions...

	bool _bIsHit = p_World->LineTraceSingleByChannel(_HitResult, _RayStartLoc, _RayEndLoc, ECC_Visibility, _QueryParams);
	DrawDebugLine(p_World, _RayStartLoc, _RayEndLoc, FColor::Orange, true, 0, 2.5f);

	if (_bIsHit == true)
	{
		if (_HitResult.GetComponent()->IsA<UStaticMeshComponent>()) // But wait... the future meshes will also be SMC...
		{
			return EFillingEntityFriendType::Wall;
		}
		else
		{
			return EFillingEntityFriendType::OtherFillingEntity;
		}
	}
	else
	{
		return EFillingEntityFriendType::Void;
	}
}

EFillingEntityFriendType FRoomFillingEntity::SHOOT_Ray(UWorld* p_World, EFillingEntityFace p_Face)
{
	if (p_Face == EFillingEntityFace::Face_1)
	{
		return SHOOT_Ray(p_World, FVector(-105.0f, 0.0f, 0.0f));
	}
	else if (p_Face == EFillingEntityFace::Face_2)
	{
		return SHOOT_Ray(p_World, FVector(0.0f, 105.0f, 0.0f));
	}
	else if (p_Face == EFillingEntityFace::Face_3)
	{
		return SHOOT_Ray(p_World, FVector(105.0f, 0.0f, 0.0f));
	}
	else if (p_Face == EFillingEntityFace::Face_4)
	{
		return SHOOT_Ray(p_World, FVector(0.0f, -105.0f, 0.0f));
	}
	else if (p_Face == EFillingEntityFace::Face_5)
	{
		return SHOOT_Ray(p_World, FVector(0.0f, 0.0f, 105.0f));
	}
	else
	{
		return SHOOT_Ray(p_World, FVector(0.0f, 0.0f, -105.0f));
	}
}

void FRoomFillingEntity::SHOOT()
{
	s_Face1FriendType = SHOOT_Ray(s_World, EFillingEntityFace::Face_1);
	s_Face2FriendType = SHOOT_Ray(s_World, EFillingEntityFace::Face_2);
	s_Face3FriendType = SHOOT_Ray(s_World, EFillingEntityFace::Face_3);
	s_Face4FriendType = SHOOT_Ray(s_World, EFillingEntityFace::Face_4);
	s_Face5FriendType = SHOOT_Ray(s_World, EFillingEntityFace::Face_5);
	s_Face6FriendType = SHOOT_Ray(s_World, EFillingEntityFace::Face_6);

	if (s_Face1FriendType == EFillingEntityFriendType::Void)
	{
		FRoomFillingEntity* _NewFriendEntity = new FRoomFillingEntity(s_World, s_EntityLocation + FVector(-100.0f, 0.0f, 0.0f));
		_NewFriendEntity->SET_FatherEntity(this);
		s_FriendEntities.Add(_NewFriendEntity);

		s_Face1FriendType = EFillingEntityFriendType::OtherFillingEntity;
	}

	if (s_Face2FriendType == EFillingEntityFriendType::Void)
	{
		FRoomFillingEntity* _NewFriendEntity = new FRoomFillingEntity(s_World, s_EntityLocation + FVector(0.0f, 100.0f, 0.0f));
		_NewFriendEntity->SET_FatherEntity(this);
		s_FriendEntities.Add(_NewFriendEntity);

		s_Face2FriendType = EFillingEntityFriendType::OtherFillingEntity;
	}

	if (s_Face3FriendType == EFillingEntityFriendType::Void)
	{
		FRoomFillingEntity* _NewFriendEntity = new FRoomFillingEntity(s_World, s_EntityLocation + FVector(100.0f, 0.0f, 0.0f));
		_NewFriendEntity->SET_FatherEntity(this);
		s_FriendEntities.Add(_NewFriendEntity);

		s_Face3FriendType = EFillingEntityFriendType::OtherFillingEntity;
	}

	if (s_Face4FriendType == EFillingEntityFriendType::Void)
	{
		FRoomFillingEntity* _NewFriendEntity = new FRoomFillingEntity(s_World, s_EntityLocation + FVector(0.0f, -100.0f, 0.0f));
		_NewFriendEntity->SET_FatherEntity(this);
		s_FriendEntities.Add(_NewFriendEntity);

		s_Face4FriendType = EFillingEntityFriendType::OtherFillingEntity;
	}

	if (s_Face5FriendType == EFillingEntityFriendType::Void)
	{
		FRoomFillingEntity* _NewFriendEntity = new FRoomFillingEntity(s_World, s_EntityLocation + FVector(0.0f, 0.0f, 100.0f));
		_NewFriendEntity->SET_FatherEntity(this);
		s_FriendEntities.Add(_NewFriendEntity);

		s_Face5FriendType = EFillingEntityFriendType::OtherFillingEntity;
	}

	if (s_Face6FriendType == EFillingEntityFriendType::Void)
	{
		FRoomFillingEntity* _NewFriendEntity = new FRoomFillingEntity(s_World, s_EntityLocation + FVector(0.0f, 0.0f, -100.0f));
		_NewFriendEntity->SET_FatherEntity(this);
		s_FriendEntities.Add(_NewFriendEntity);

		s_Face6FriendType = EFillingEntityFriendType::OtherFillingEntity;
	}
}

void FRoomFillingEntity::SET_FatherEntity(FRoomFillingEntity* p_Entity)
{
	if (p_Entity == nullptr)
	{
		UE_LOG(LogStruct_RoomFillingEntity, Error, TEXT("FUNCTION ''SET_FatherEntity'' ==>> The FRoomFillingEntity passed as argument is NULL !"));
		return;
	}

	s_FatherEntity = p_Entity;
}

bool FRoomFillingEntity::CHECK_WereEntityFriendsAllFind()
{
	if (
			s_Face1FriendType == EFillingEntityFriendType::Void || 
			s_Face2FriendType == EFillingEntityFriendType::Void || 
			s_Face3FriendType == EFillingEntityFriendType::Void || 
			s_Face4FriendType == EFillingEntityFriendType::Void || 
			s_Face5FriendType == EFillingEntityFriendType::Void || 
			s_Face6FriendType == EFillingEntityFriendType::Void			
	   )
	{
		return false;
	}
	else
	{
		return true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AArchBuilderBase class methods
//

AArchBuilderBase::AArchBuilderBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Variables initialization
	m_roomDynamicWallsThickness = 30.0f;
	m_roomDynamicWallsHeight = 400.0f;

	// Loading Assets
	const ConstructorHelpers::FObjectFinder<UMaterialInstance>OF_MI_SKTC_Red  (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Red.MI_SKTC_Red'"    ));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance>OF_MI_SKTC_Green(TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Green.MI_SKTC_Green'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance>OF_MI_SKTC_Blue (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Blue.MI_SKTC_Blue'"  ));

	check(OF_MI_SKTC_Red  .Succeeded()) ;
	check(OF_MI_SKTC_Green.Succeeded()) ;
	check(OF_MI_SKTC_Blue .Succeeded()) ;

	m_MI_SKTC_Red   = OF_MI_SKTC_Red  .Object ;
	m_MI_SKTC_Green = OF_MI_SKTC_Green.Object ;
	m_MI_SKTC_Blue  = OF_MI_SKTC_Blue .Object ;

	// *** DynamicMeshComponents *** //
	//
	// Ground
	//
	m_DMC_ground = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ground"));
	m_DMC_ground->SetMobility(EComponentMobility::Movable);
	m_DMC_ground->SetGenerateOverlapEvents(false);
	m_DMC_ground->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ground->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ground->EnableComplexAsSimpleCollision();
	m_DMC_ground->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ground->SetupAttachment(RootComponent);
	//m_DMC_ground->SetMaterial(0, m_MI_SKTC_Red);

	//
	
	// Ceiling
	//
	m_DMC_ceiling = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ceiling"));
	m_DMC_ceiling->SetMobility(EComponentMobility::Movable);
	m_DMC_ceiling->SetGenerateOverlapEvents(false);
	m_DMC_ceiling->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ceiling->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ceiling->EnableComplexAsSimpleCollision();
	m_DMC_ceiling->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ceiling->SetupAttachment(RootComponent);
	//m_DMC_ceiling->SetMaterial(0, m_MI_SKTC_Blue);
}



void AArchBuilderBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArchBuilderBase::CREATE_DynamicWalls()
{
	for (int32 i = 0; i < m_ArchDynamicWalls.Num(); i++)
	{
		// Safety check 1
		if (m_ArchDynamicWalls[i]->s_wallNature == EWallNature::Unknown)
		{
			UE_LOG(LogClass_ArchBuilderBase, Error, TEXT("FUNCTION ''CREATE_WallsGeometry'' ==>> Wall nature is Unknown !"));
			return;
		}

		// Safety check 2
		if (m_ArchDynamicWalls[i]->s_wallAxis == ECardinalAxis::Unknown)
		{
			UE_LOG(LogClass_ArchBuilderBase, Error, TEXT("FUNCTION ''CREATE_WallsGeometry'' ==>> Wall axis is Unknown !"));
			return;
		}

		// Safety check 3
		if (m_ArchDynamicWalls[i]->s_wallFlowingDirection == ECardinalDirection::Unknown)
		{
			UE_LOG(LogClass_ArchBuilderBase, Error, TEXT("FUNCTION ''CREATE_WallsGeometry'' ==>> Wall flowing direction is Unknown !"));
			return;
		}

		// For debug purposes
		if (i == 0)
		{
			DrawDebugBox(GetWorld(), m_ArchDynamicWalls[i]->s_wallLocation, FVector(20.0f), FColor::Yellow, true, 2, 3.0f);
		}

		// Actual function's body
		//
		FGeometryScriptPrimitiveOptions _primitiveOptions;
		FTransform _defaultTransform;
		_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation);

		if (m_ArchDynamicWalls[i]->s_wallAxis == ECardinalAxis::NorthSouth) // X axis
		{
			if (m_ArchDynamicWalls[i]->s_wallNature == EWallNature::InsideWall)
			{
				m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
				(
					m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
					_primitiveOptions,
					_defaultTransform,
					m_ArchDynamicWalls[i]->s_wallLength - m_ArchDynamicWalls[i]->s_wallThickness * 2.0f,
					m_ArchDynamicWalls[i]->s_wallThickness,
					m_ArchDynamicWalls[i]->s_wallHeight
				));

				m_ArchDynamicWalls[i]->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_precedingWall->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_followingWall->s_bIsLocationFixed = true;
			}
			else if (m_ArchDynamicWalls[i]->s_wallNature == EWallNature::InbetweenWall)
			{
				// To fix the possible intersection between the last and the first wall
				if (i == m_ArchDynamicWalls.Num() - 1)
				{
					if (m_ArchDynamicWalls[i]->s_followingWall->s_wallNature == EWallNature::InbetweenWall)
					{
						if (m_ArchDynamicWalls[i]->s_wallFlowingDirection == ECardinalDirection::North)
						{
							_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(-m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f, 0.0f));
						}
						else
						{
							_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f, 0.0f));
						}

						m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
						(
							m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
							_primitiveOptions,
							_defaultTransform,
							m_ArchDynamicWalls[i]->s_wallLength - m_ArchDynamicWalls[i]->s_wallThickness,
							m_ArchDynamicWalls[i]->s_wallThickness,
							m_ArchDynamicWalls[i]->s_wallHeight
						));

						continue;
					}
				}

				if (m_ArchDynamicWalls[i]->s_bIsLocationFixed == false)
				{
					if (i == 0) // If not done, the first wall will be corrected when it is supposed to always be the following wall that is
					{
						m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
						(
							m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
							_primitiveOptions,
							_defaultTransform,
							m_ArchDynamicWalls[i]->s_wallLength,
							m_ArchDynamicWalls[i]->s_wallThickness,
							m_ArchDynamicWalls[i]->s_wallHeight
						));

						continue;
					}

					if (m_ArchDynamicWalls[i]->s_wallFlowingDirection == ECardinalDirection::North)
					{
						_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f, 0.0f));
					}
					else
					{
						_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(-m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f, 0.0f));
					}

					m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
					(
						m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
						_primitiveOptions,
						_defaultTransform,
						m_ArchDynamicWalls[i]->s_wallLength - m_ArchDynamicWalls[i]->s_wallThickness,
						m_ArchDynamicWalls[i]->s_wallThickness,
						m_ArchDynamicWalls[i]->s_wallHeight
					));

					m_ArchDynamicWalls[i]->s_bIsLocationFixed = true;
					m_ArchDynamicWalls[i]->s_followingWall->s_bIsLocationFixed = true;
				}
				else // if s_bIsLocationFixed == true
				{
					m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
					(
						m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
						_primitiveOptions,
						_defaultTransform,
						m_ArchDynamicWalls[i]->s_wallLength,
						m_ArchDynamicWalls[i]->s_wallThickness,
						m_ArchDynamicWalls[i]->s_wallHeight
					));
				}
			}
			else // If s_wallNature == EWallNature::OutsideWall
			{
				m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
				(
					m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
					_primitiveOptions,
					_defaultTransform,
					m_ArchDynamicWalls[i]->s_wallLength,
					m_ArchDynamicWalls[i]->s_wallThickness,
					m_ArchDynamicWalls[i]->s_wallHeight
				));

				m_ArchDynamicWalls[i]->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_precedingWall->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_followingWall->s_bIsLocationFixed = true;
			}
		}
		else // Y axis
		{
			if (m_ArchDynamicWalls[i]->s_wallNature == EWallNature::InsideWall)
			{
				m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
				(
					m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
					_primitiveOptions,
					_defaultTransform,
					m_ArchDynamicWalls[i]->s_wallThickness,
					m_ArchDynamicWalls[i]->s_wallLength - m_ArchDynamicWalls[i]->s_wallThickness * 2.0f,
					m_ArchDynamicWalls[i]->s_wallHeight
				));

				m_ArchDynamicWalls[i]->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_precedingWall->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_followingWall->s_bIsLocationFixed = true;
			}
			else if (m_ArchDynamicWalls[i]->s_wallNature == EWallNature::InbetweenWall)
			{
				// To fix the possible intersection between the last and the first wall
				if (i == m_ArchDynamicWalls.Num() - 1)
				{
					if (m_ArchDynamicWalls[i]->s_followingWall->s_wallNature == EWallNature::InbetweenWall)
					{
						if (m_ArchDynamicWalls[i]->s_wallFlowingDirection == ECardinalDirection::East)
						{
							_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(0.0f, m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f));
						}
						else
						{
							_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(0.0f, -m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f));
						}

						m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
						(
							m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
							_primitiveOptions,
							_defaultTransform,
							m_ArchDynamicWalls[i]->s_wallThickness,
							m_ArchDynamicWalls[i]->s_wallLength - m_ArchDynamicWalls[i]->s_wallThickness,
							m_ArchDynamicWalls[i]->s_wallHeight
						));

						continue;
					}
				}

				if (m_ArchDynamicWalls[i]->s_bIsLocationFixed == false)
				{
					if (i == 0) // If not done, the first wall will be corrected when it is supposed to always be the following wall that is
					{
						m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
						(
							m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
							_primitiveOptions,
							_defaultTransform,
							m_ArchDynamicWalls[i]->s_wallThickness,
							m_ArchDynamicWalls[i]->s_wallLength,
							m_ArchDynamicWalls[i]->s_wallHeight
						));

						continue;
					}

					// This part is weird. Above, the minus sign (-) was switched from North to South, but here it isn't ? Doesn't feel right
					if (m_ArchDynamicWalls[i]->s_wallFlowingDirection == ECardinalDirection::East)
					{
						_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(0.0f, m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f));
					}
					else
					{
						_defaultTransform.SetLocation(m_ArchDynamicWalls[i]->s_wallLocation + FVector(0.0f, -m_ArchDynamicWalls[i]->s_wallThickness / 2.0f, 0.0f));
					}

					m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
					(
						m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
						_primitiveOptions,
						_defaultTransform,
						m_ArchDynamicWalls[i]->s_wallThickness,
						m_ArchDynamicWalls[i]->s_wallLength - m_ArchDynamicWalls[i]->s_wallThickness,
						m_ArchDynamicWalls[i]->s_wallHeight
					));

					m_ArchDynamicWalls[i]->s_bIsLocationFixed = true;
					m_ArchDynamicWalls[i]->s_followingWall->s_bIsLocationFixed = true;
				}
				else
				{
					m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
					(
						m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
						_primitiveOptions,
						_defaultTransform,
						m_ArchDynamicWalls[i]->s_wallThickness,
						m_ArchDynamicWalls[i]->s_wallLength,
						m_ArchDynamicWalls[i]->s_wallHeight
					));
				}
			}
			else
			{
				m_ArchDynamicWalls[i]->s_DMC_wall->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
				(
					m_ArchDynamicWalls[i]->s_DMC_wall->GetDynamicMesh(),
					_primitiveOptions,
					_defaultTransform,
					m_ArchDynamicWalls[i]->s_wallThickness,
					m_ArchDynamicWalls[i]->s_wallLength,
					m_ArchDynamicWalls[i]->s_wallHeight
				));

				m_ArchDynamicWalls[i]->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_precedingWall->s_bIsLocationFixed = true;
				m_ArchDynamicWalls[i]->s_followingWall->s_bIsLocationFixed = true;
			}
		}
	}
}

void AArchBuilderBase::CONVERT_DynamicToStatic()
{
	for (int32 i = 0; i < m_ArchDynamicWalls.Num(); i++)
	{
		UStaticMeshComponent* _NewSMC = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(),
			*FString("SMC_RoomStaticWall_" + FString::FromInt(i)));
		check(_NewSMC);
		_NewSMC->RegisterComponent();
		_NewSMC->SetMobility(EComponentMobility::Movable); // Wouldn't this cause issues later ? 
		_NewSMC->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		_NewSMC->SetStaticMesh(UMeshConvertor::CREATE_StaticMesh(m_ArchDynamicWalls[i]->s_DMC_wall));
		_NewSMC->SetMaterial(0, m_MI_SKTC_Red);

		if (m_ArchDynamicWalls[i]->s_bHasBeenPierced == true)
		{
			_NewSMC->GetStaticMesh()->GetBodySetup()->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
			// Idk how this work but it fixed the collision issue. Need to check more about it later so it doesn't come to bite us later
			// by fucking up some other stuff
			_NewSMC->GetStaticMesh()->Build();
		}

		m_StaticWalls.Add(_NewSMC);

		m_ArchDynamicWalls[i]->s_DMC_wall->UnregisterComponent();
		m_ArchDynamicWalls[i]->s_DMC_wall->ConditionalBeginDestroy();
		m_ArchDynamicWalls[i]->s_DMC_wall = nullptr;
	}

	// Ground
	UStaticMeshComponent* _NewStaticGround = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(),
		*FString("SMC_Ground"));
	check(_NewStaticGround);
	_NewStaticGround->RegisterComponent();
	_NewStaticGround->SetMobility(EComponentMobility::Movable);
	_NewStaticGround->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	_NewStaticGround->SetStaticMesh(UMeshConvertor::CREATE_StaticMesh(m_DMC_ground));
	_NewStaticGround->SetRelativeLocation(_NewStaticGround->GetRelativeLocation() + FVector(0.0f, 0.0f, -m_roomDynamicWallsThickness));
	_NewStaticGround->GetStaticMesh()->GetBodySetup()->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseSimpleAndComplex;
	m_StaticWalls.Add(_NewStaticGround);

	m_DMC_ground->UnregisterComponent();
	m_DMC_ground->ConditionalBeginDestroy();
	m_DMC_ground = nullptr;

	// Ceiling
	UStaticMeshComponent* _NewStaticCeiling = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(),
		*FString("SMC_Ceiling"));
	check(_NewStaticCeiling);
	_NewStaticCeiling->RegisterComponent();
	_NewStaticCeiling->SetMobility(EComponentMobility::Movable);
	_NewStaticCeiling->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	_NewStaticCeiling->SetStaticMesh(UMeshConvertor::CREATE_StaticMesh(m_DMC_ceiling));
	_NewStaticCeiling->SetRelativeLocation(_NewStaticCeiling->GetRelativeLocation() + FVector(0.0f, 0.0f, m_roomDynamicWallsHeight));
	//_NewStaticCeiling->SetRelativeLocation(_NewStaticCeiling->GetRelativeLocation());

	UE_LOG(LogTemp, Warning, TEXT("CEILING RELATIVE LOC : %s"), *_NewStaticCeiling->GetRelativeLocation().ToString());

	m_StaticWalls.Add(_NewStaticCeiling);

	m_DMC_ceiling->UnregisterComponent();
	m_DMC_ceiling->ConditionalBeginDestroy();
	m_DMC_ceiling = nullptr;
}

void AArchBuilderBase::CLEAN_DynamicData()
{
	for (int32 i = 0; i < m_ArchDynamicWalls.Num(); i++)
	{
		m_ArchDynamicWalls[i]->s_DMC_wall->UnregisterComponent();
		m_ArchDynamicWalls[i]->s_DMC_wall->ConditionalBeginDestroy();
		m_ArchDynamicWalls[i]->s_DMC_wall = nullptr;
	}

	m_ArchCorners.Empty();
	m_ArchDynamicWalls.Empty();
	m_splitCorners.Empty();

	m_DMC_ceiling->UnregisterComponent();
	m_DMC_ceiling->ConditionalBeginDestroy();
	m_DMC_ceiling = nullptr;

	m_DMC_ground->UnregisterComponent();
	m_DMC_ground->ConditionalBeginDestroy();
	m_DMC_ground = nullptr;
}

//
// ************************************************************* //
// ******************** Splitting functions ******************** //
// ************************************************************* //
//

FSplitCorner AArchBuilderBase::SPLIT_CornerBasic
(
	TArray<FArchCorner*>& p_workingArray       ,
	uint32                p_targetCornerIndex  , 
	const FIntPoint&      p_maximumSplitAmount , 
	const FIntPoint&      p_minimumSplitAmount
)
{
	if (p_minimumSplitAmount.X > p_maximumSplitAmount.X || p_minimumSplitAmount.Y > p_maximumSplitAmount.Y)
	{
		UE_LOG(LogClass_ArchBuilderBase, Warning, TEXT("FUNCTION ''SPLIT_CornerBasic'' ==>> Maximum splitting limit reached !"));
		return FSplitCorner(false);
	}

	FArchCorner* _targetCorner = p_workingArray[p_targetCornerIndex]; // For performance purposes

	if (_targetCorner->s_directionToPrecedingCornerPoint == FIntPoint(-2, -2) || _targetCorner->s_directionToFollowingCornerPoint == FIntPoint(-2, -2))
	{
		UE_LOG(LogClass_ArchBuilderBase, Error, TEXT("FUNCTION ''SPLIT_CornerBasic'' ==>> The target FArchCorner has not been well initialized !"));
		return FSplitCorner(false);
	}

	FVector _targetCornerOriginalLocation = _targetCorner->s_cornerLocation;

	int32 _splitAmountX = FMath::RandRange(p_minimumSplitAmount.X, p_maximumSplitAmount.X);
	int32 _splitAmountY = FMath::RandRange(p_minimumSplitAmount.Y, p_maximumSplitAmount.Y);

	FVector _newPrecedingCorner_increaseDistance = FVector
	(
		(float)(_splitAmountX * _targetCorner->s_directionToPrecedingCornerPoint.X * _targetCorner->s_directionToPrecedingCornerPoint.Y * 100.0f), 
		(float)(_splitAmountY * _targetCorner->s_directionToFollowingCornerPoint.X * _targetCorner->s_directionToPrecedingCornerPoint.Y * 100.0f), 
		0.0f
	);

	FVector _newFollowingCorner_increaseDistance = FVector
	(
		(float)(_splitAmountX * _targetCorner->s_directionToFollowingCornerPoint.X * _targetCorner->s_directionToFollowingCornerPoint.Y * 100.0f), 
		(float)(_splitAmountY * _targetCorner->s_directionToPrecedingCornerPoint.X * _targetCorner->s_directionToFollowingCornerPoint.Y * 100.0f), 
		0.0f
	);

	FArchCorner* _newPrecedingCorner = new FArchCorner(_targetCorner->s_cornerLocation + _newPrecedingCorner_increaseDistance);
	FArchCorner* _newFollowingCorner = new FArchCorner(_targetCorner->s_cornerLocation + _newFollowingCorner_increaseDistance);

	// Setting new location, preceding and following FArchCorner
	//
	p_workingArray[p_targetCornerIndex]->s_cornerLocation += _newPrecedingCorner_increaseDistance + _newFollowingCorner_increaseDistance ;
	p_workingArray[p_targetCornerIndex]->SET_PrecedingCorner(_newPrecedingCorner)                                                        ;
	p_workingArray[p_targetCornerIndex]->SET_FollowingCorner(_newFollowingCorner)                                                        ;

	// Updating the old preceding and following FArchCorner
	//
	if (p_workingArray[p_targetCornerIndex]->s_bIsFirstArrayElement)
	{
		int32 _workingArrayCount = p_workingArray.Num();

		p_workingArray[_workingArrayCount - 1] ->SET_FollowingCorner(_newPrecedingCorner) ;
		p_workingArray[p_targetCornerIndex + 1]->SET_PrecedingCorner(_newFollowingCorner) ;

		_newPrecedingCorner->SET_PrecedingCorner(p_workingArray[_workingArrayCount - 1 ]) ;
		_newPrecedingCorner->SET_FollowingCorner(p_workingArray[p_targetCornerIndex    ]) ;
		_newFollowingCorner->SET_PrecedingCorner(p_workingArray[p_targetCornerIndex    ]) ;
		_newFollowingCorner->SET_FollowingCorner(p_workingArray[p_targetCornerIndex + 1]) ;
	}
	else if (p_workingArray[p_targetCornerIndex]->s_bIsLastArrayElement)
	{
		p_workingArray[p_targetCornerIndex - 1]->SET_FollowingCorner(_newPrecedingCorner) ;
		p_workingArray[0                      ]->SET_PrecedingCorner(_newFollowingCorner) ;

		_newPrecedingCorner->SET_PrecedingCorner(p_workingArray[p_targetCornerIndex - 1]) ;
		_newPrecedingCorner->SET_FollowingCorner(p_workingArray[p_targetCornerIndex    ]) ;
		_newFollowingCorner->SET_PrecedingCorner(p_workingArray[p_targetCornerIndex    ]) ;
		_newFollowingCorner->SET_FollowingCorner(p_workingArray[0                      ]) ;

		// Update s_bIsLastArrayElement
		p_workingArray[p_targetCornerIndex]->s_bIsLastArrayElement = false ;
		_newFollowingCorner                ->s_bIsLastArrayElement = true  ;
	}
	else
	{
		p_workingArray[p_targetCornerIndex - 1]->SET_FollowingCorner(_newPrecedingCorner) ;
		p_workingArray[p_targetCornerIndex + 1]->SET_PrecedingCorner(_newFollowingCorner) ;

		_newPrecedingCorner->SET_PrecedingCorner(p_workingArray[p_targetCornerIndex - 1]) ;
		_newPrecedingCorner->SET_FollowingCorner(p_workingArray[p_targetCornerIndex    ]) ;
		_newFollowingCorner->SET_PrecedingCorner(p_workingArray[p_targetCornerIndex    ]) ;
		_newFollowingCorner->SET_FollowingCorner(p_workingArray[p_targetCornerIndex + 1]) ;
	}

	if (p_workingArray[p_targetCornerIndex]->s_bIsFirstArrayElement)
	{
		m_ArchCorners.Insert(_newPrecedingCorner, p_workingArray.Num());
		m_ArchCorners.Insert(_newFollowingCorner, p_targetCornerIndex + 1);
	}
	else if (p_workingArray[p_targetCornerIndex]->s_bIsLastArrayElement)
	{
		m_ArchCorners.Insert(_newPrecedingCorner, p_targetCornerIndex);
		m_ArchCorners.Insert(_newFollowingCorner, p_workingArray.Num());
	}
	else
	{
		m_ArchCorners.Insert(_newPrecedingCorner, p_targetCornerIndex);
		m_ArchCorners.Insert(_newFollowingCorner, p_targetCornerIndex + 2);
	}

	// Computting the split area center location
	FVector _splitAreaCenterLocation = _targetCornerOriginalLocation + 
		((_newPrecedingCorner_increaseDistance / 2.0f) + (_newFollowingCorner_increaseDistance / 2.0f));

	UE_LOG(LogTemp, Warning, TEXT("_splitAreaCenterLocation : %s"), *_splitAreaCenterLocation.ToString());

	// Possible visual debug stuff here

	return FSplitCorner(FIntPoint(_splitAmountX, _splitAmountY), _splitAreaCenterLocation, true);
}

FSplitCorner AArchBuilderBase::SPLIT_CornerInside
(
	TArray<FArchCorner*>& p_workingArray       ,
	uint32                p_targetCornerIndex  , 
	uint32                p_maximumSplitAmount , 
	const FIntPoint&      p_minimumSplitAmount
)
{
	FArchCorner _targetCorner = *p_workingArray[p_targetCornerIndex];

	//int32 _distanceToPrecedingCorner = _targetCorner.s_distanceToPrecedingCorner - p_maximumSplitAmount;
	//int32 _distanceToFollowingCorner = _targetCorner.s_distanceToFollowingCorner - p_maximumSplitAmount;

	int32 _distanceToPrecedingCorner = p_workingArray[p_targetCornerIndex]->s_distanceToPrecedingCorner - p_maximumSplitAmount;
	int32 _distanceToFollowingCorner = p_workingArray[p_targetCornerIndex]->s_distanceToFollowingCorner - p_maximumSplitAmount;

	if (p_workingArray[p_targetCornerIndex]->s_directionToPrecedingCornerEnum == ECardinalDirection::North ||
		p_workingArray[p_targetCornerIndex]->s_directionToPrecedingCornerEnum == ECardinalDirection::South)
	{
		FSplitCorner _splitCornerInside = SPLIT_CornerBasic(p_workingArray, p_targetCornerIndex, FIntPoint(_distanceToPrecedingCorner, _distanceToFollowingCorner), p_minimumSplitAmount);
		_splitCornerInside.s_splitType = ESplitType::InsideSplit;
		return _splitCornerInside;
	}
	else
	{
		FSplitCorner _splitCornerInside = SPLIT_CornerBasic(p_workingArray, p_targetCornerIndex, FIntPoint(_distanceToFollowingCorner, _distanceToPrecedingCorner), p_minimumSplitAmount);
		_splitCornerInside.s_splitType = ESplitType::InsideSplit;
		return _splitCornerInside;
	}
}

FSplitCorner AArchBuilderBase::SPLIT_CornerOutside
(
	TArray<FArchCorner*>& p_workingArray              ,
	uint32                p_targetCornerIndex         , 
	const FIntPoint&      p_maximumSplitAmount        ,
	const FSplitCorner&   p_insideSplit               , 
	uint32                p_minimumExtraSplitDistance
)
{
	if (p_insideSplit.s_bIsSplitting == false)
	{
		UE_LOG(LogClass_ArchBuilderBase, Warning, TEXT("FUNCTION ''SPLIT_CornerOutside'' ==>> The upstream inside split did not split !"));
		return FSplitCorner(false);
	}

	int32 _minimumSplitDistanceX = p_insideSplit.s_splitSurface.X + p_minimumExtraSplitDistance;
	int32 _minimumSplitDistanceY = p_insideSplit.s_splitSurface.Y + p_minimumExtraSplitDistance;

	FSplitCorner _splitCornerOutside = SPLIT_CornerBasic(p_workingArray, p_targetCornerIndex, p_maximumSplitAmount, FIntPoint(_minimumSplitDistanceX, _minimumSplitDistanceY));
	_splitCornerOutside.s_splitType = ESplitType::OutsideSplit;
	return _splitCornerOutside;
}

//
// ************************************************************* //
// ******************** Debugging functions ******************** //
// ************************************************************* //
//

void AArchBuilderBase::VDEBUG_RoomCornersLocations()
{
	for (int32 i = 0; i < m_ArchCorners.Num(); i++)
	{
		DrawDebugSphere(GetWorld(), m_ArchCorners[i]->s_cornerLocation, 20.0f, 8, FColor::Red, true, 2.0f);
	}
}

void AArchBuilderBase::VDEBUG_RoomCornersLinks()
{
	for (int32 i = 0; i < m_ArchCorners.Num(); i++)
	{
		DrawDebugLine(GetWorld(), m_ArchCorners[i]->s_cornerLocation, m_ArchCorners[i]->s_followingCorner->s_cornerLocation, FColor::Green, true, 2, 5.0f);
	}
}

void AArchBuilderBase::TDEBUG_RoomDynamicWall(FRoomDynamicWall* p_wall)
{
	/*FString _wallAxis = "";
	FString _wallNature = "";
	FString _wallDirection = "";
	FString _wallAxisDir = "";

	switch (static_cast<int>(p_wall->s_wallAxis))
	{
	case 0:
		_wallAxis = "NORTH-SOUTH";
		break;
	case 1:
		_wallAxis = "EAST-WEST";
		break;
	}

	switch (static_cast<int>(p_wall->s_wallNature))
	{
	case 0:
		_wallNature = "InsideWall";
		break;
	case 1:
		_wallNature = "OutsideWall";
		break;
	case 2:
		_wallNature = "InbetweenWall";
		break;
	}

	switch (static_cast<int>(p_wall->s_wallFacingDirection))
	{
	case 0:
		_wallDirection = "North";
		break;
	case 1:
		_wallDirection = "South";
		break;
	case 2:
		_wallDirection = "East";
		break;
	case 3:
		_wallDirection = "West";
		break;
	}

	switch (static_cast<int>(p_wall->s_wallAxisDir))
	{
	case 0:
		_wallAxisDir = "North";
		break;
	case 1:
		_wallAxisDir = "South";
		break;
	case 2:
		_wallAxisDir = "East";
		break;
	case 3:
		_wallAxisDir = "West";
		break;
	}


	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("***"));

	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Name : %s"), *p_wall->s_name);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Location : %s"), *p_wall->s_wallLocation.ToString());
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Previous wall : %s"), *p_wall->s_previousWall->s_name);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Following wall : %s"), *p_wall->s_followingWall->s_name);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Start corner : %s"), *p_wall->s_wallStartPoint->s_name);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("End corner : %s"), *p_wall->s_wallEndPoint->s_name);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall axis : %s"), *_wallAxis);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall length : %f"), p_wall->s_wallLength);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall thickness : %f"), p_wall->s_wallThickness);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall height : %f"), p_wall->s_wallHeight);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall nature : %s"), *_wallNature);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall direction : %s"), *_wallDirection);
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall modification : %s"), (p_wall->s_bIsInbetweenWallAlreadyModified ? TEXT("TRUE") : TEXT("FALSE")));
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall location fixed : %s"), (p_wall->s_bIsLocationFixed ? TEXT("TRUE") : TEXT("FALSE")));
	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("Wall axis direction : %s"), *_wallAxisDir);

	UE_LOG(LogClass_BaseDynamicBuilder, Log, TEXT("***"));*/
}

void AArchBuilderBase::TDEBUG_RoomDynamicWallsArray()
{

}

void AArchBuilderBase::TDEBUG_SplitCornersArray()
{
	UE_LOG(LogClass_ArchBuilderBase, Warning, TEXT("***"));

	for (int32 i = 0; i < m_splitCorners.Num(); i++)
	{
		UE_LOG(LogClass_ArchBuilderBase, Log, TEXT("m_splitCorners[%d] ... "), i);
		m_splitCorners[i].TDEBUG_FullStructInfo();
	}

	UE_LOG(LogClass_ArchBuilderBase, Warning, TEXT("***"));
}

void AArchBuilderBase::SET_BuilderManager(UBuilderManager* p_builderRef)
{
	if (p_builderRef == nullptr)
	{
		UE_LOG(LogClass_ArchBuilderBase, Error, TEXT("FUNCTION ''SET_BuilderManager'' ==>> p_builderRef is NULL !"));
		return;
	}

	m_builderManager = p_builderRef;
}

void AArchBuilderBase::SET_LevelSpaceManager(ULevelSpaceManager* p_LevelSpaceManagerRef)
{
	if (p_LevelSpaceManagerRef == nullptr)
	{
		UE_LOG(LogClass_ArchBuilderBase, Error, TEXT("FUNCTION ''SET_BuilderManager'' ==>> p_LevelSpaceManagerRef is NULL !"));
		return;
	}

	m_LevelSpaceManager = p_LevelSpaceManagerRef;
}

AVolumeShell* AArchBuilderBase::GET_VolumeShell() const
{
	if (m_VolumeShell == nullptr)
	{
		UE_LOG(LogClass_ArchBuilderBase, Error, TEXT("FUNCTION ''GET_VolumeShell'' ==>> m_VolumeShell is NULL !"));
		return nullptr;
	}

	return m_VolumeShell;
}