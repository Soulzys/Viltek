#include "Managers/BuilderManager.h"
#include "../../Public/Architecture/StartingRoomBuilder.h"
#include "../../Public/Architecture/CorridorBuilder.h"
#include "../../Public/Testing/TestingMerging.h"
#include "../../Public/Testing/TestingLineTrace.h"
#include "../../Public/Architecture/ArchSas.h"
#include "../../Public/Architecture/CorridorTest.h"
#include "../../Public/Architecture/EndingRoomBuilder.h"
#include "../../Public/Architecture/CorridorBuilderTest.h"
#include "../../Public/Managers/LevelSpaceManager.h"
#include "../../Public/Architecture/LevelSpaceBuilder.h"

DEFINE_LOG_CATEGORY(LogClass_BuilderManager);

UBuilderManager::UBuilderManager()
{

}

void UBuilderManager::BUILD_World()
{
	check(GetWorld());

	BUILD_LevelSpace();
	BUILD_StartingRoom();
	BUILD_EndingRoom();

	// Should this be setup here ? Wouldn't it be better if each FArchVolumeCuboid was added in their distinct ARoomBuilder class ?
	// E.g inside AStartingRoomBuilder or AEndingRoomBuilder. I don't know... 
	m_LevelSpaceBuilder->GENERATE_ActualVolumeShell(m_StartingRoomBuilder->GET_VolumeShell(), m_EndingRoomBuilder->GET_VolumeShell());
	m_LevelSpaceBuilder->BUILD_LevelSpace();
	m_LevelSpaceBuilder->DEBUG_DisplayLevelVolume();

	//
	//

	BUILD_Corridor();
}

void UBuilderManager::BUILD_LevelSpace()
{
	m_LevelSpaceBuilder = GetWorld()->SpawnActor<ALevelSpaceBuilder>();
}

void UBuilderManager::BUILD_StartingRoom()
{
	// ORIGINAL CODE
	//
	m_StartingRoomBuilder = GetWorld()->SpawnActor<AStartingRoomBuilder>(FVector(0.0f, 0.0f, 0.0f), FRotator(0));

	if (m_StartingRoomBuilder == nullptr)
	{
		UE_LOG(LogClass_BuilderManager, Error, TEXT("FUNCTION ''BUILD_StartingRoom'' ==>> Could not spawn AStartingRoomBuilder : returned reference is NULL !"));
		return;
	}

	//Setting the class "owner"
	m_StartingRoomBuilder->SET_BuilderManager(this);
	m_StartingRoomBuilder->SET_LevelSpaceManager(m_LevelSpaceManager);

	FStartingRoomBuildOptions _buildOptions                       ;
	_buildOptions.s_areaSize = EStartingRoomAreaSize::Large       ;
	_buildOptions.s_pattern  = EStartingRoomPattern::DoubleRooms  ;

	m_StartingRoomBuilder->BUILD_StartingRoom(_buildOptions);

	//
	//
	// Sas
	//
	//
	//m_ArchSas = GetWorld()->SpawnActor<AArchSas>();



	/*if (m_startingRoom_staticBuilder == nullptr)
	{
		UE_LOG(LogClass_BuilderManager, Error, TEXT("m_startingRoom_dynamicBuilder is NULL !"));
	}

	check(m_startingRoom_staticBuilder);

	m_startingRoom_staticBuilder->BUILD_StaticStartingRoom();*/

	//
	//

	// TESTING CODE
	//
	/*m_testingMerging = GetWorld()->SpawnActor<ATestingMerging>();
	check(m_testingMerging);*/
}

void UBuilderManager::BUILD_EndingRoom()
{
	FEndingRoomBuildOptions _BuildOptions;
	_BuildOptions.s_Pattern = EEndingRoomPattern::Complex;

	m_EndingRoomBuilder = GetWorld()->SpawnActor<AEndingRoomBuilder>();

	m_EndingRoomBuilder->SET_LevelSpaceManager(m_LevelSpaceManager);

	FVector _NewLoc = FVector(3500.0f, 0.0f, 0.0f);
	m_EndingRoomBuilder->SetActorLocation(_NewLoc);

	m_EndingRoomBuilder->BUILD_EndingRoom(_BuildOptions);
}

void UBuilderManager::BUILD_Corridor()
{
	m_CorridorBuilder = GetWorld()->SpawnActor<ACorridorBuilder>();
	m_CorridorBuilder->BUILD_Corridor(m_StartingRoomBuilder->GET_DoorLocation(),
		m_StartingRoomBuilder->GET_DoorDirection(), m_EndingRoomBuilder->GET_DoorLocation(), m_EndingRoomBuilder->GET_DoorDirection(), 2);
}

void UBuilderManager::SET_ArchSas(AArchSas* p_ArchSas)
{
	if (p_ArchSas == nullptr)
	{
		UE_LOG(LogClass_BuilderManager, Error, TEXT("FUNCTION ''SET_ArchSas'' ==>> Function parameter is NULL !"));
		return;
	}

	m_ArchSas = p_ArchSas;
}

void UBuilderManager::SET_LevelSpaceManager(ULevelSpaceManager* p_ObjectRef)
{
	if (p_ObjectRef == nullptr)
	{
		UE_LOG(LogClass_BuilderManager, Error, TEXT("FUNCTION ''SET_LevelSpaceManager'' ==>> ObjectRef is NULL !"));
		return;
	}

	m_LevelSpaceManager = p_ObjectRef;
}