#include "../../../Public/UI/HUD/MasterHUD.h"
#include "../../../Public/UI/Slate/Widgets/Menu/SStartMenu.h"
#include "../../../Public/Controllers/GameModes/SpectatorGameMode.h"

void AMasterHUD::BeginPlay()
{
	Super::BeginPlay();

	SAssignNew(m_startMenu, SStartMenu).m_owningHUD(this);

	check(m_startMenu);

	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_startMenu.ToSharedRef()));

	m_startMenu->SetVisibility(EVisibility::Visible);

	PlayerOwner->SetInputMode(FInputModeUIOnly());
	//PlayerOwner->SetInputMode(FInputModeGameAndUI());
	//SetInputMode()
}

void AMasterHUD::START_Game()
{
	GEngine->GameViewport->RemoveAllViewportWidgets();
	PlayerOwner->SetInputMode(FInputModeGameOnly());

	ASpectatorGameMode* _GM = Cast<ASpectatorGameMode>(GetWorld()->GetAuthGameMode());

	check(_GM);

	_GM->START_Game();
}
