#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "MeshDescription.h"
#include "GeometryBase.h"
#include "TestingMerging.generated.h"

class UDynamicMesh;
class UDynamicMeshComponent;
class ADynamicMeshActor;
class USceneComponent;
class UStaticMesh;
class UStaticMeshComponent;
class UStaticMeshDescription;
class UMaterialInstance;
//class FDynamicMesh3;
struct FMeshDescription;
struct FGetMeshParameters;

//using namespace UE::Geometry;
using UE::Geometry::FDynamicMesh3;

UCLASS()
class VILTEK_API ATestingMerging : public AActor
{
	GENERATED_BODY()
	
public:	

	ATestingMerging();

protected:

	virtual void BeginPlay() override;

protected:

	void TESTING_Fourth();
	void TESTING_Seventh();
	void TESTING_Tenth();
	void TESTING_Eleventh();

private:

	USceneComponent* m_SC_root = nullptr;
	UDynamicMeshComponent* m_DMC_testing = nullptr;

	UStaticMesh* m_SM_test = nullptr;
	UStaticMesh* m_SM_test2 = nullptr;
	UStaticMeshComponent* m_SMC_test = nullptr;

	FMeshDescription* m_MeshDescription = nullptr;
	FMeshDescription* m_MeshDescription_TEST = nullptr;

	UMaterialInstance* m_MI_SKTC_red;
	UMaterialInstance* m_MI_SKTC_blue;
	UMaterialInstance* m_MI_SKTC_green;

	//class FDynamicMesh3* m_dynamicMesh3 = nullptr;
};

