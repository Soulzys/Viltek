#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Architecture/ArchStructureBase.h"
#include "ArchSas.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_ArchSas, Log, All);

class UStaticMeshComponent  ;
class UDynamicMeshComponent ;
class UStaticMeshSocket     ;

UCLASS()
class VILTEK_API AArchSas : public AArchStructureBase
{
	GENERATED_BODY()
	
public:	

	AArchSas();

	const FVector  GET_SocketBeginLocation() const ;
	const FVector  GET_SocketEndLocation  () const ;
	const FRotator GET_SocketBeginRotation() const ;
	const FRotator GET_SocketEndRotation  () const ;

	const ECardinalDirection GET_ArchFacingDirection();

	void SET_Rotation(ECardinalDirection p_WallDirection, float p_WallThickness, const FVector& p_WallLocation);

protected:

	virtual void BeginPlay() override;

	void BUILD_Sas();

protected:

	UStaticMeshComponent*  m_SMC_Sas         = nullptr ;
	UStaticMeshComponent*  m_SMC_SasObstacle = nullptr ;
	UDynamicMeshComponent* m_DMC_Testing     = nullptr ;

	const UStaticMeshSocket* m_SMSK_AttachBegin = nullptr;
	const UStaticMeshSocket* m_SMSK_AttachEnd   = nullptr;

	ECardinalDirection m_FacingDirection; // The direction toward the outside, usually in the prolongement of the corridor
};
