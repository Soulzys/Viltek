#include "Tools/VTEK_StaticMesh.h"
#include "MeshDescription.h"
#include "StaticMeshDescription.h"
#include "StaticMeshAttributes.h"

UVTEK_StaticMesh::UVTEK_StaticMesh()
{

}

void UVTEK_StaticMesh::BuildFromStaticMeshDescriptions
(
	const TArray<UStaticMeshDescription*>& StaticMeshDescriptions,
	bool bBuildSimpleCollision,
	bool bFastBuild
)
{
	TArray<const FMeshDescription*> MeshDescriptions;
	MeshDescriptions.Reserve(StaticMeshDescriptions.Num());

	for (UStaticMeshDescription* StaticMeshDescription : StaticMeshDescriptions)
	{
		MeshDescriptions.Emplace(&StaticMeshDescription->GetMeshDescription());
	}

	FBuildMeshDescriptionsParams Params;
	Params.bBuildSimpleCollision = bBuildSimpleCollision;
	Params.bFastBuild = bFastBuild;
	BuildFromMeshDescriptions(MeshDescriptions, Params);

	UE_LOG(LogTemp, Warning, TEXT("HEEEEEEEEEELLOOOOOOOOOOOOOOO"));
}

bool UVTEK_StaticMesh::BuildFromMeshDescriptions
(
	const TArray<const FMeshDescription*>& MeshDescriptions,
	const FBuildMeshDescriptionsParams& Params
)
{
	UE_LOG(LogTemp, Warning, TEXT("HEEEEEEEEEELLOOOOOOOOOOOOOOO"));
	return false;
}

void UVTEK_StaticMesh::BuildFromMeshDescription(const FMeshDescription& MeshDescription, FStaticMeshLODResources& LODResources)
{
	UE_LOG(LogTemp, Warning, TEXT("HEEEEEEEEEELLOOOOOOOOOOOOOOO"));
}