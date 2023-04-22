#include "Architecture/EndingRoomBuilder.h"
#include "../../Public/Managers/LevelSpaceManager.h"
#include "../../Public/Tools/VolumeShell.h"

DEFINE_LOG_CATEGORY(LogClass_EndingRoomBuilder);

AEndingRoomBuilder::AEndingRoomBuilder()
{

}

void AEndingRoomBuilder::BUILD_EndingRoom(const FEndingRoomBuildOptions& p_BuildOptions)
{
	SETTING_RoomDimensions(p_BuildOptions.s_AreaSize);
	SETTING_InitialCorners();
	SPLIT_RoomCorners(p_BuildOptions.s_Pattern);
	CREATE_RoomDynamicWallStructs();
	CREATE_DynamicWalls();
	CREATE_GroundAndCeiling();
	PIERCE_Door();

	COMPUTE_RoomNewDimensions(2);


	VDEBUG_RoomCornersLocations();
	VDEBUG_RoomCornersLinks();
}

void AEndingRoomBuilder::SETTING_RoomDimensions(EEndingRoomAreaSize p_AreaSize)
{
	switch (p_AreaSize)
	{
		case EEndingRoomAreaSize::Small:
			m_roomLength = 6;
			m_roomWidth = 6;
			break;

		case EEndingRoomAreaSize::Medium:
			m_roomLength = 12;
			m_roomWidth = 12;
			break;

		case EEndingRoomAreaSize::Large:
			m_roomLength = 16;
			m_roomWidth = 16;
			break;
	}
}

void AEndingRoomBuilder::SETTING_InitialCorners()
{
	float _roomLength = (float)(m_roomLength * 100.0f);
	float _roomWidth = (float)(m_roomWidth * 100.0f);

	//FArchCorner* _toto = new FArchCorner();

	FArchCorner* _topRightCorner    = new FArchCorner(GetActorLocation() + FVector(  _roomLength / 2.0f,   _roomWidth / 2.0f, 0.0f)) ;
	FArchCorner* _bottomRightCorner = new FArchCorner(GetActorLocation() + FVector(- _roomLength / 2.0f,   _roomWidth / 2.0f, 0.0f)) ;
	FArchCorner* _bottomLeftCorner  = new FArchCorner(GetActorLocation() + FVector(- _roomLength / 2.0f, - _roomWidth / 2.0f, 0.0f)) ;
	FArchCorner* _topLeftCorner     = new FArchCorner(GetActorLocation() + FVector(  _roomLength / 2.0f, - _roomWidth / 2.0f, 0.0f)) ;

	check(_topRightCorner   ) ;
	check(_bottomRightCorner) ;
	check(_bottomLeftCorner ) ;
	check(_topLeftCorner    ) ;

	_topRightCorner->s_bIsFirstArrayElement = true ;
	_topLeftCorner ->s_bIsLastArrayElement  = true ;

	_topRightCorner   ->SET_PrecedingCorner(_topLeftCorner    ) ;
	_topRightCorner   ->SET_FollowingCorner(_bottomRightCorner) ;
	_bottomRightCorner->SET_PrecedingCorner(_topRightCorner   ) ;
	_bottomRightCorner->SET_FollowingCorner(_bottomLeftCorner ) ;
	_bottomLeftCorner ->SET_PrecedingCorner(_bottomRightCorner) ;
	_bottomLeftCorner ->SET_FollowingCorner(_topLeftCorner    ) ;
	_topLeftCorner    ->SET_PrecedingCorner(_bottomLeftCorner ) ;
	_topLeftCorner    ->SET_FollowingCorner(_topRightCorner   ) ;

	m_ArchCorners.Add(_topRightCorner   ) ;
	m_ArchCorners.Add(_bottomRightCorner) ;
	m_ArchCorners.Add(_bottomLeftCorner ) ;
	m_ArchCorners.Add(_topLeftCorner    ) ;
}

void AEndingRoomBuilder::SPLIT_RoomCorners(EEndingRoomPattern p_RoomPattern, int32 p_TargetCornerIndex)
{
	switch (p_RoomPattern)
	{
		case EEndingRoomPattern::Basic:
			SPLITTING_BasicPattern(p_TargetCornerIndex);
			break;

		case EEndingRoomPattern::Complex:
			SPLITTING_ComplexPattern(p_TargetCornerIndex);
			break;

		default:
			UE_LOG(LogClass_EndingRoomBuilder, Error, TEXT("FUNCTION ''SPLIT_RoomCorners'' ==>> Switch() has reached the default case !"));
			break;
	}
}

void AEndingRoomBuilder::SPLITTING_BasicPattern(int32 p_TargetCornerIndex)
{

}

void AEndingRoomBuilder::SPLITTING_ComplexPattern(int32 p_TargetCornerIndex)
{
	// Split second corner
	FSplitCorner _FirstSplit = SPLIT_CornerInside(m_ArchCorners, 1);
	FSplitCorner _SecondSplit = SPLIT_CornerOutside(m_ArchCorners, 2, FIntPoint(m_roomLength, m_roomWidth), _FirstSplit);

	// Split third corner
	FSplitCorner _ThirdSplit = SPLIT_CornerInside(m_ArchCorners, m_ArchCorners.Num() - 2);
	FSplitCorner _FourthSplit = SPLIT_CornerOutside(m_ArchCorners, m_ArchCorners.Num() - 3, FIntPoint(m_roomLength - 2, m_roomWidth - 2), _ThirdSplit);

	// Split fourth corner
	FSplitCorner _FifthSplit = SPLIT_CornerInside(m_ArchCorners, m_ArchCorners.Num() - 1);

	m_splitCorners.Add(_FirstSplit);
	m_splitCorners.Add(_SecondSplit);
	m_splitCorners.Add(_ThirdSplit);
	m_splitCorners.Add(_FourthSplit);
	m_splitCorners.Add(_FifthSplit);

	DrawDebugSphere(GetWorld(), m_ArchCorners[0]->s_cornerLocation, 25.0f, 12, FColor::Orange, true, 0, 10.0f);
}