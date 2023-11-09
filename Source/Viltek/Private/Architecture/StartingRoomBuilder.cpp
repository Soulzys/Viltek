#include "Architecture/StartingRoomBuilder.h"
#include "UDynamicMesh.h"
#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshAssetFunctions.h"
#include "../../Public/Controllers/GameModes/SpectatorGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "../../Public/Managers/BuilderManager.h"
#include "../../Public/Tools/FunctionsLibrary/Geometry/ViltekGeoLib_DoorMaking.h"
#include "../../Public/Tools/FunctionsLibrary/Geometry/MeshConvertor.h"
#include "../../Public/Tools/Environment/Procedural/FillingCube.h"
#include "../../Public/Architecture/ArchSas.h"
#include "../../Public/Managers/LevelSpaceManager.h"

DEFINE_LOG_CATEGORY(LogClass_StartingRoomBuilder);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// AStartingRoomDynamicBuilder class methods
//

AStartingRoomBuilder::AStartingRoomBuilder()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AStartingRoomBuilder::BeginPlay()
{
	Super::BeginPlay();	
}

void AStartingRoomBuilder::BUILD_StartingRoom(const FStartingRoomBuildOptions& p_BuildOptions)
{
	SETTING_RoomDimensions(p_BuildOptions.s_areaSize);
	SETTING_InitialCorners();
	SPLIT_RoomCorners(p_BuildOptions.s_pattern);
	CREATE_RoomDynamicWallStructs();
	CREATE_DynamicWalls();
	CREATE_GroundAndCeiling();
	PIERCE_Door();

	COMPUTE_RoomNewDimensions(2);


	// Debbugging
	VDEBUG_RoomCornersLocations();
	VDEBUG_RoomCornersLinks();

	TDEBUG_SplitCornersArray();

	UE_LOG(LogTemp, Warning, TEXT("CEILING INTRIGUE !! WallsHeight : %f"), m_roomDynamicWallsHeight);
	UE_LOG(LogTemp, Warning, TEXT("CEILING INTRIGUE !! RootComponent : %s"), *RootComponent->GetName());
}

void AStartingRoomBuilder::SETTING_RoomDimensions(EStartingRoomAreaSize p_roomAreaSize)
{
	switch (p_roomAreaSize)
	{
		case EStartingRoomAreaSize::Small:
			m_roomLength = 4;
			m_roomWidth  = 4;
			break;

		case EStartingRoomAreaSize::Medium:
			m_roomLength = 10;
			m_roomWidth  = 10;
			break;

		case EStartingRoomAreaSize::Large:
			m_roomLength = 12;
			m_roomWidth  = 12;
			break;
	}
}

// ********** Generate room corners SUB-PROCESS :: START ********** //
//

void AStartingRoomBuilder::SETTING_InitialCorners()
{
	float _roomLength = (float)(m_roomLength * 100.0f) ;
	float _roomWidth  = (float)(m_roomWidth  * 100.0f) ;

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

	// Setting direction ?

	m_ArchCorners.Add(_topRightCorner   ) ;
	m_ArchCorners.Add(_bottomRightCorner) ;
	m_ArchCorners.Add(_bottomLeftCorner ) ;
	m_ArchCorners.Add(_topLeftCorner    ) ;
}

void AStartingRoomBuilder::SPLIT_RoomCorners(EStartingRoomPattern p_roomPattern, int32 p_targetCornerIndex)
{
	switch (static_cast<int>(p_roomPattern))
	{
		case 0: // Basic
			return;
			
		case 1: // VPattern
			SPLITTING_VPattern(p_targetCornerIndex);
			break;

		case 2: // Double rooms
			SPLITTING_DoubleRooms(p_targetCornerIndex);
			break;

		default:
			UE_LOG(LogClass_StartingRoomBuilder, Error, TEXT("FUNCTION ''SPLIT_RoomCorners'' ==>> Switch() has reached the 'default' case !"));
			break;
	}
}
//
// ********** Generate room corners SUB-PROCESS :: END ********** //

//
//

void AStartingRoomBuilder::PLACE_PlayerStart()
{
	//FVector _playerStartLoc = GetActorLocation() + FVector(0.0f, 0.0f, m_roomHeight / 2.0f);
	FVector _playerStartLoc = GetActorLocation() + FVector(0.0f, 0.0f, 200.0f);

	m_playerStart = GetWorld()->SpawnActor<APlayerStart>(_playerStartLoc, FRotator(0));

	// Safety check
	if (m_playerStart == nullptr)
	{
		UE_LOG(LogClass_StartingRoomBuilder, Error, TEXT("FUNCTION ''PLACE_PlayerStart'' ==>> Could not spawn 'm_playerStart' !"));
		return;
	}

	ASpectatorGameMode* _GM = Cast<ASpectatorGameMode>(GetWorld()->GetAuthGameMode());

	// Safety check
	if (_GM == nullptr)
	{
		UE_LOG(LogClass_StartingRoomBuilder, Error, TEXT("FUNCTION ''PLACE_PlayerStart'' ==>> Could not get ASpectatorGameMode reference !"));
		return;
	}

	//_GM->START_Game(_playerStartLoc);
	//_GM->START_Game(
	//_GM->SET_PlayerStart(m_playerStart);
}
//
// ********** Generate room ground and ceiling SUB-PROCESS :: END ********** //

//
//

void AStartingRoomBuilder::SPLITTING_VPattern(int32 p_targetCornerIndex)
{
	FSplitCorner _firstSplit = SPLIT_CornerInside(m_ArchCorners, p_targetCornerIndex);
	m_splitCorners.Add(_firstSplit);
}

void AStartingRoomBuilder::SPLITTING_DoubleRooms(int32 p_targetCornerIndex)
{
	if (p_targetCornerIndex == 0)
	{
		FSplitCorner _firstSplit = SPLIT_CornerInside(m_ArchCorners, 0);
		FSplitCorner _secondSplit = SPLIT_CornerOutside(m_ArchCorners, 0, FIntPoint(m_roomLength, m_roomWidth), _firstSplit);
		m_splitCorners.Add(_firstSplit);
		m_splitCorners.Add(_secondSplit);
	}
	else
	{
		FSplitCorner _firstSplit = SPLIT_CornerInside(m_ArchCorners, p_targetCornerIndex);
		FSplitCorner _secondSplit = SPLIT_CornerOutside(m_ArchCorners, p_targetCornerIndex + 1, FIntPoint(m_roomLength, m_roomWidth), _firstSplit);
		m_splitCorners.Add(_firstSplit);
		m_splitCorners.Add(_secondSplit);
	}
}