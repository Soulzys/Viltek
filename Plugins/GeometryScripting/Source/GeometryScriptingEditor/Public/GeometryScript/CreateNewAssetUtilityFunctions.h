// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "Engine/BlockingVolume.h"
#include "BodySetupEnums.h"
#include "CreateNewAssetUtilityFunctions.generated.h"

class UStaticMesh;
class UDynamicMesh;
class AVolume;


USTRUCT(BlueprintType)
struct GEOMETRYSCRIPTINGEDITOR_API FGeometryScriptUniqueAssetNameOptions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = Options)
	int32 UniqueIDDigits = 6;
};


USTRUCT(BlueprintType)
struct GEOMETRYSCRIPTINGEDITOR_API FGeometryScriptCreateNewVolumeFromMeshOptions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = Options)
	TSubclassOf<class AVolume> VolumeType = ABlockingVolume::StaticClass();

	UPROPERTY(BlueprintReadWrite, Category = Options)
	bool bAutoSimplify = true;

	UPROPERTY(BlueprintReadWrite, Category = Options)
	int32 MaxTriangles = 250;
};


USTRUCT(BlueprintType)
struct GEOMETRYSCRIPTINGEDITOR_API FGeometryScriptCreateNewStaticMeshAssetOptions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = Options)
	bool bEnableRecomputeNormals = false;

	UPROPERTY(BlueprintReadWrite, Category = Options)
	bool bEnableRecomputeTangents = false;

	UPROPERTY(BlueprintReadWrite, Category = Options)
	bool bEnableNanite = false;

	UPROPERTY(BlueprintReadWrite, Category = Options)
	float NaniteProxyTrianglePercent = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = Options)
	bool bEnableCollision = true;

	UPROPERTY(BlueprintReadWrite, Category = Options)
	TEnumAsByte<ECollisionTraceFlag> CollisionMode = ECollisionTraceFlag::CTF_UseDefault;
};




UCLASS(meta = (ScriptName = "GeometryScript_NewAssetUtils"))
class GEOMETRYSCRIPTINGEDITOR_API UGeometryScriptLibrary_CreateNewAssetFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "GeometryScript|AssetManagement", meta = (ExpandEnumAsExecs = "Outcome"))
	static void
	CreateUniqueNewAssetPathName(
		FString AssetFolderPath,
		FString BaseAssetName,
		FString& UniqueAssetPathAndName,
		FString& UniqueAssetName,
		FGeometryScriptUniqueAssetNameOptions Options,
		TEnumAsByte<EGeometryScriptOutcomePins>& Outcome,
		UGeometryScriptDebug* Debug = nullptr);


	UFUNCTION(BlueprintCallable, Category = "GeometryScript|AssetManagement", meta = (ExpandEnumAsExecs = "Outcome"))
	static UPARAM(DisplayName = "Volume Actor") AVolume* 
	CreateNewVolumeFromMesh(
		UDynamicMesh* FromDynamicMesh, 
		UPARAM(ref) UWorld* CreateInWorld,
		FTransform ActorTransform,
		FString BaseActorName,
		FGeometryScriptCreateNewVolumeFromMeshOptions Options,
		TEnumAsByte<EGeometryScriptOutcomePins>& Outcome,
		UGeometryScriptDebug* Debug = nullptr);

	UFUNCTION(BlueprintCallable, Category = "GeometryScript|AssetManagement", meta = (ExpandEnumAsExecs = "Outcome"))
	static UPARAM(DisplayName = "Static Mesh Asset") UStaticMesh* 
	CreateNewStaticMeshAssetFromMesh(
		UDynamicMesh* FromDynamicMesh, 
		FString AssetPathAndName,
		FGeometryScriptCreateNewStaticMeshAssetOptions Options,
		TEnumAsByte<EGeometryScriptOutcomePins>& Outcome,
		UGeometryScriptDebug* Debug = nullptr);

};

