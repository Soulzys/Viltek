#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchBase.generated.h"

class USceneComponent;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ENUMS
//
UENUM()
enum class ECardinalDirection : uint8
{
	North   = 0 ,
	South   = 1 ,
	East    = 2 ,
	West    = 3 ,
	Top     = 4 ,
	Bottom  = 5 , 
	Unknown = 6
};

//
//

UENUM()
enum class ECardinalAxis : uint8
{
	NorthSouth = 0 , // Unreal X axis
	EastWest   = 1 , // Unreal Y axis
	Unknown    = 2
};

UCLASS()
class VILTEK_API AArchBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AArchBase();

protected:

	virtual void BeginPlay() override;

protected:

	USceneComponent* m_SC_RootComponent = nullptr;
};
