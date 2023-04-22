#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuilderManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_BuilderManager, Log, All);

class AActor;
class AStartingRoomBuilder;
class ACorridorBuilder;
class ATestingMerging;
class AArchSas;
class ACorridorTest;
class AEndingRoomBuilder;
class ATestingLineTrace;
class ACorridorBuilderTest;
class ULevelSpaceManager;
class ALevelSpaceBuilder;

UCLASS()
class VILTEK_API UBuilderManager : public UObject
{
	GENERATED_BODY()

public:

	UBuilderManager();

public:
	
	void BUILD_World();

	void SET_ArchSas(AArchSas* p_ArchSas);
	void SET_LevelSpaceManager(ULevelSpaceManager* p_ObjectRef);

public:

protected:

	void BUILD_LevelSpace();
	void BUILD_StartingRoom();
	void BUILD_EndingRoom();
	void BUILD_Corridor();

protected:

	AStartingRoomBuilder* m_StartingRoomBuilder = nullptr;
	ACorridorBuilder* m_CorridorBuilder = nullptr;
	AArchSas* m_ArchSas = nullptr;
	ACorridorTest* m_CorridorTest = nullptr;
	AEndingRoomBuilder* m_EndingRoomBuilder = nullptr;
	ACorridorBuilderTest* m_CorridorBuilderTest = nullptr;


	ATestingMerging* m_testingMerging = nullptr;
	ATestingLineTrace* m_TestingLineTrace = nullptr;


	ULevelSpaceManager* m_LevelSpaceManager = nullptr;
	ALevelSpaceBuilder* m_LevelSpaceBuilder = nullptr;
};
