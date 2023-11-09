#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpectatorGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_SpectatorGameMode, Log, All);

class ATestingDynLoadingAsset;
class ABaseSpectator;
class APlayerStart;
class UBuilderManager;
class ULevelSpaceManager;

UCLASS()
class VILTEK_API ASpectatorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASpectatorGameMode();

	void SET_PlayerStart(APlayerStart* p_playerStart);

	void START_Game();

protected:

	virtual void BeginPlay();
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* Player, AActor* StartSpot) override;
	
private:

	APlayerStart* m_playerStart = nullptr;

	ATestingDynLoadingAsset* m_testingLoadingAsset = nullptr;
	ABaseSpectator* m_baseSpectator = nullptr;

	UBuilderManager* m_builderManager = nullptr;
	ULevelSpaceManager* m_LevelSpaceManager = nullptr;
};
