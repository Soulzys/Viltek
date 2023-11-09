#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MasterHUD.generated.h"

class SStartMenu;

UCLASS()
class VILTEK_API AMasterHUD : public AHUD
{
	GENERATED_BODY()

public:

	void START_Game();

private:

	virtual void BeginPlay() override;

private:

	TSharedPtr<SStartMenu> m_startMenu = nullptr;
	
};
