#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MeshConvertor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_MeshConvertor, Log, All);

class UStaticMesh;
class UDynamicMeshComponent;

UCLASS()
class VILTEK_API UMeshConvertor : public UObject
{
	GENERATED_BODY()

public:

	static UStaticMesh* CREATE_StaticMesh(UDynamicMeshComponent* p_DynMeshComp, bool p_bRecomputeNormals = true, bool p_bComputeTangents = true);
	
};
