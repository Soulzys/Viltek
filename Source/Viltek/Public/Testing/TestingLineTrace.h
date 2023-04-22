#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestingLineTrace.generated.h"

class UDynamicMeshComponent;

UCLASS()
class VILTEK_API ATestingLineTrace : public AActor
{
	GENERATED_BODY()
	
public:	

	ATestingLineTrace();

protected:

	virtual void BeginPlay() override;

protected:

	void BUILD_Mesh();
	void TRACE_Line();

protected:

	UDynamicMeshComponent* m_DMC_TestingCube = nullptr;
};
