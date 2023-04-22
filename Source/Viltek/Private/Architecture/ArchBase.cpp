#include "Architecture/ArchBase.h"

AArchBase::AArchBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setting RootComponent
	m_SC_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SC_RootComponent"));
	SetRootComponent(m_SC_RootComponent);
}

void AArchBase::BeginPlay()
{
	Super::BeginPlay();	
}
