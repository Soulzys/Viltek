#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "VTEK_StaticMesh.generated.h"

UCLASS()
class VILTEK_API UVTEK_StaticMesh : public UStaticMesh
{
	GENERATED_BODY()

public:

	UVTEK_StaticMesh();

	void BuildFromStaticMeshDescriptions
	(
		const TArray<UStaticMeshDescription*>& StaticMeshDescriptions, 
		bool bBuildSimpleCollision = false, 
		bool bFastBuild = true
	);

	bool BuildFromMeshDescriptions
	(
		const TArray<const FMeshDescription*>& MeshDescriptions,
		const FBuildMeshDescriptionsParams& Params = FBuildMeshDescriptionsParams()
	);
	
	void BuildFromMeshDescription(const FMeshDescription& MeshDescription, FStaticMeshLODResources& LODResources);
};
