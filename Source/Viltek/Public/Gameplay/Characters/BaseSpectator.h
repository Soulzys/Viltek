#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseSpectator.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_BaseSpectator, Log, All);

class USphereComponent       ;
class UPawnMovementComponent ;

UCLASS()
class VILTEK_API ABaseSpectator : public APawn
{
	GENERATED_BODY()

public:

	ABaseSpectator();

	void SET_StartPosition(const FVector& p_startPos);

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:	

	void INIT_InputBindings();

protected: // Movement functions

	virtual void MoveForward (float Value_p) ;
	virtual void MoveRight   (float Value_p) ;
	virtual void MoveUp_World(float Value_p) ;
	virtual void TurnAtRate  (float Rate_p)  ;
	virtual void LookUpAtRate(float Rate_p)  ;

	void ACTION_Confirm();
	void ACTION_Testing();

protected:

	UPROPERTY(EditAnywhere, Category = "Spectator Pawn")
	USphereComponent* m_SPC_collision = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Spectator Pawn")
	UPawnMovementComponent* m_PMC_movement = nullptr;

	UStaticMeshComponent* m_SMC_cube = nullptr;

	// Movement variables
	float m_baseTurnRate   ;
	float m_baseLookUpRate ;

};
