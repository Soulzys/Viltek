#include "Controllers/GameModes/SpectatorGameMode.h"
#include "../../../Public/Gameplay/Characters/BaseSpectator.h"
#include "GameFramework/PlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "../../../Public/Controllers/PControllers/SpectatorPController.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Public/Managers/BuilderManager.h"
#include "../../../Public/Managers/LevelSpaceManager.h"
#include "../../../Public/UI/HUD/MasterHUD.h"

DEFINE_LOG_CATEGORY(LogClass_SpectatorGameMode);

ASpectatorGameMode::ASpectatorGameMode()
{
	DefaultPawnClass      = ABaseSpectator       ::StaticClass() ;
	PlayerControllerClass = ASpectatorPController::StaticClass() ;
	HUDClass              = AMasterHUD           ::StaticClass() ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// *** Overrided functions *** //
//
void ASpectatorGameMode::BeginPlay()
{
	Super::BeginPlay();	
}

void ASpectatorGameMode::START_Game()
{
	check(m_baseSpectator);

	//m_baseSpectator->SET_StartPosition(p_playerStartLocation);
	//m_baseSpectator->SetActorLocation(p_playerStartLocation);

	UE_LOG(LogTemp, Warning, TEXT("STARTING GAME !"));

	// Initializing the Manager classes
	m_builderManager = NewObject<UBuilderManager>(this, UBuilderManager::StaticClass(), *FString("BuilderManagerClass"));
	check(m_builderManager);
	/*m_LevelSpaceManager = NewObject<ULevelSpaceManager>(this, ULevelSpaceManager::StaticClass(), *FString("LevelSpaceManagerClass"));
	check(m_LevelSpaceManager);*/

	//m_builderManager->SET_LevelSpaceManager(m_LevelSpaceManager);
	m_builderManager->BUILD_World();
}

// Doesn't currently do anything
AActor* ASpectatorGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	m_playerStart = GetWorld()->SpawnActor<APlayerStart>(FVector(0.0f, 0.0f, 850.0f), FRotator(0));

	if (m_playerStart == nullptr)
	{
		UE_LOG(LogClass_SpectatorGameMode, Error, TEXT("FUNCTION ''ChoosePlayerStart_Implementation'' ==>> m_playerStart is NULL !"));
		return nullptr;
	}

	return m_playerStart;
}

// Used to get a reference to the DefaultPawn, which is currently set to be of type ABaseSpectator
APawn* ASpectatorGameMode::SpawnDefaultPawnFor_Implementation(AController* Player, AActor* StartSpot)
{
	m_baseSpectator = GetWorld()->SpawnActor<ABaseSpectator>();
	check(m_baseSpectator);
	return m_baseSpectator;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// *** Own class functions *** //
//
void ASpectatorGameMode::SET_PlayerStart(APlayerStart* p_playerStart)
{
	if (p_playerStart == nullptr)
	{
		UE_LOG(LogClass_SpectatorGameMode, Error, TEXT("FUNCTION ''SET_PlayerStart'' ==>> p_playerStart is NULL !"));
		return;
	}

	m_playerStart = p_playerStart;
}

