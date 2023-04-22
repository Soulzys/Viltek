#include "Gameplay/Characters/BaseSpectator.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
//#include "Viltek/Public/Controllers/PControllers/SpectatorPController.h"
//#include "VTekUI/Public/HUD/MasterHUD.h"

DEFINE_LOG_CATEGORY(LogClass_BaseSpectator);

ABaseSpectator::ABaseSpectator()
{
	PrimaryActorTick.bCanEverTick = false;

	// *** Variable initialization *** //
	m_baseTurnRate   = 45.0f ;
	m_baseLookUpRate = 45.0f ;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Loading assets
	const ConstructorHelpers::FObjectFinder<UStaticMesh> OF_BasicCube(TEXT("StaticMesh'/Game/Geometry/Testing/SM_BasicCube.SM_BasicCube'"));

	check(OF_BasicCube.Succeeded());

	// *** m_SPC_collision *** //
	//
	m_SPC_collision = CreateDefaultSubobject<USphereComponent>(TEXT("SPC_collision")) ;
	m_SPC_collision->InitSphereRadius(25.0f)                                          ;
	m_SPC_collision->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName)     ;
	m_SPC_collision->CanCharacterStepUpOn = ECB_No                                    ;
	m_SPC_collision->SetShouldUpdatePhysicsVolume(true)                               ;
	m_SPC_collision->SetCanEverAffectNavigation(false)                                ;
	m_SPC_collision->bDynamicObstacle = true                                          ;
	m_SPC_collision->SetMobility(EComponentMobility::Movable)                         ;
	SetRootComponent(m_SPC_collision)                                                 ;

	// *** m_PMC_movement *** //
	m_PMC_movement = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("PMC_movement"));

	// *** m_SMC_cube *** //
	m_SMC_cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMC_cube"))     ;
	m_SMC_cube->SetStaticMesh(OF_BasicCube.Object)                                  ;
	m_SMC_cube->SetRelativeScale3D(FVector(0.2f))                                   ;
	m_SMC_cube->SetupAttachment(GetRootComponent())                                 ;
	m_SMC_cube->SetMobility(EComponentMobility::Movable)                            ;
	m_SMC_cube->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName) ;
}

void ABaseSpectator::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent == nullptr)
	{
		UE_LOG(LogClass_BaseSpectator, Error, TEXT("FUNCTION ''SetupPlayerInputComponent'' ==>> PlayerInputComponent is NULL !"));
		return;
	}

	// Setting my own personal bindings
	INIT_InputBindings();

	PlayerInputComponent->BindAxis("BaseSpectator_MoveForward" , this, &ABaseSpectator::MoveForward            ) ;
	PlayerInputComponent->BindAxis("BaseSpectator_MoveRight"   , this, &ABaseSpectator::MoveRight              ) ;
	PlayerInputComponent->BindAxis("BaseSpectator_MoveUp"      , this, &ABaseSpectator::MoveUp_World           ) ;
	PlayerInputComponent->BindAxis("BaseSpectator_Turn"        , this, &ABaseSpectator::AddControllerYawInput  ) ;
	//PlayerInputComponent->BindAxis("BaseSpectator_TurnRate"    , this, &ABaseSpectator::TurnAtRate             ) ;
	PlayerInputComponent->BindAxis("BaseSpectator_LookUp"      , this, &ABaseSpectator::AddControllerPitchInput) ;
	//PlayerInputComponent->BindAxis("BaseSpectator_LookUpAtRate", this, &ABaseSpectator::LookUpAtRate           ) ;

	PlayerInputComponent->BindAction("BaseSpectator_Confirm", EInputEvent::IE_Pressed, this, &ABaseSpectator::ACTION_Confirm);
	PlayerInputComponent->BindAction("BaseSpectator_Testing", EInputEvent::IE_Pressed, this, &ABaseSpectator::ACTION_Testing);
}

void ABaseSpectator::INIT_InputBindings()
{
	static bool _bAreBindingsAdded = false;

	if (_bAreBindingsAdded == false)
	{
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_MoveForward", EKeys::W              ,  1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_MoveForward", EKeys::S              , -1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_MoveRight"  , EKeys::A              , -1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_MoveRight"  , EKeys::D              ,  1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_TurnRate"   , EKeys::Left           , -1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_TurnRate"   , EKeys::Right          ,  1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_Turn"       , EKeys::MouseX         ,  1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_LookUpRate" , EKeys::Gamepad_RightY ,  1.0f)) ;
		UPlayerInput::AddEngineDefinedAxisMapping  (FInputAxisKeyMapping  ("BaseSpectator_LookUp"     , EKeys::MouseY         , -1.0f)) ;

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("BaseSpectator_Confirm" , EKeys::Enter));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("BaseSpectator_Testing" , EKeys::T    ));

		UE_LOG(LogClass_BaseSpectator, Log, TEXT("FUNCTION ''INIT_InputBindings'' ==>> New bindings added !"));

		_bAreBindingsAdded = true;
	}
}

void ABaseSpectator::MoveForward(float Value_p)
{
	if (Value_p != 0.0f)
	{
		if (Controller)
		{
			FRotator const _ControlSpaceRot = Controller->GetControlRotation();

			// Transforms to world space and adds it
			AddMovementInput(FRotationMatrix(_ControlSpaceRot).GetScaledAxis(EAxis::X), Value_p);
		}
	}
}

void ABaseSpectator::MoveRight(float Value_p)
{
	if (Value_p != 0.0f)
	{
		if (Controller)
		{
			FRotator const _ControlSpaceRot = Controller->GetControlRotation();

			// Transforms to world space and adds it
			AddMovementInput(FRotationMatrix(_ControlSpaceRot).GetScaledAxis(EAxis::Y), Value_p);
		}
	}
}

void ABaseSpectator::MoveUp_World(float Value_p)
{
	if (Value_p != 0.0f)
	{
		AddMovementInput(FVector::UpVector, Value_p);
	}
}

void ABaseSpectator::TurnAtRate(float Rate_p)
{
	// Calculates delta for this frame from the rate information
	AddControllerYawInput(Rate_p * m_baseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void ABaseSpectator::LookUpAtRate(float Rate_p)
{
	// Same as above
	AddControllerPitchInput(Rate_p * m_baseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void ABaseSpectator::ACTION_Confirm()
{
	UE_LOG(LogTemp, Warning, TEXT("I have confirmed my action (meaning, pressed ENTER)"));
}

void ABaseSpectator::ACTION_Testing()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Some debug message!"));
}

void ABaseSpectator::SET_StartPosition(const FVector& p_startPos)
{
	SetActorLocation(p_startPos);
}
