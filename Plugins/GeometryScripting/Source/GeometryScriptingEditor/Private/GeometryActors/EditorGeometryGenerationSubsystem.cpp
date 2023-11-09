// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeometryActors/EditorGeometryGenerationSubsystem.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "Editor.h"


void UEditorGeometryGenerationManager::Tick(float DeltaTime)
{
	// rebuild all valid generated actors, if necessary
	for (AGeneratedDynamicMeshActor* Actor : ActiveGeneratedActors)
	{
		if (Actor->IsValidLowLevel() && IsValid(Actor) && !Actor->IsUnreachable() && Actor->GetLevel() != nullptr)
		{
			Actor->ExecuteRebuildGeneratedMeshIfPending();
		}
	}
}

bool UEditorGeometryGenerationManager::IsTickable() const
{
	return (ActiveGeneratedActors.Num() > 0);
}

TStatId UEditorGeometryGenerationManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEditorGeometryGenerationManager, STATGROUP_Tickables);
}


void UEditorGeometryGenerationManager::Shutdown()
{
	ActiveGeneratedActors.Reset();
}

void UEditorGeometryGenerationManager::RegisterGeneratedMeshActor(AGeneratedDynamicMeshActor* Actor)
{
	ensure(ActiveGeneratedActors.Contains(Actor) == false);
	ActiveGeneratedActors.Add(Actor);
}

void UEditorGeometryGenerationManager::UnregisterGeneratedMeshActor(AGeneratedDynamicMeshActor* Actor)
{
	ensure(ActiveGeneratedActors.Contains(Actor));
	ActiveGeneratedActors.Remove(Actor);
}




bool UEditorGeometryGenerationSubsystem::bIsShuttingDown = false;


void UEditorGeometryGenerationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GenerationManager = NewObject<UEditorGeometryGenerationManager>(this);

	GEditor->OnEditorClose().AddUObject(this, &UEditorGeometryGenerationSubsystem::OnShutdown);
	FCoreDelegates::OnEnginePreExit.AddUObject(this, &UEditorGeometryGenerationSubsystem::OnShutdown);
}

void UEditorGeometryGenerationSubsystem::Deinitialize()
{
	GEditor->OnEditorClose().RemoveAll(this);
	FCoreDelegates::OnEnginePreExit.RemoveAll(this);
	bIsShuttingDown = true;

	if (GenerationManager)
	{
		GenerationManager->Shutdown();
		GenerationManager = nullptr;
	}
}


void UEditorGeometryGenerationSubsystem::OnShutdown()
{
	bIsShuttingDown = true;
}



void UEditorGeometryGenerationSubsystem::RegisterGeneratedMeshActor(AGeneratedDynamicMeshActor* Actor)
{
	if (bIsShuttingDown || GEditor == nullptr)		// subsystem no longer exists
	{
		return;
	}

	UEditorGeometryGenerationSubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorGeometryGenerationSubsystem>();
	if (ensure(Subsystem))
	{
		Subsystem->GenerationManager->RegisterGeneratedMeshActor(Actor);
	}
}

void UEditorGeometryGenerationSubsystem::UnregisterGeneratedMeshActor(AGeneratedDynamicMeshActor* Actor)
{
	if (bIsShuttingDown || GEditor == nullptr)		// subsystem no longer exists
	{
		return;
	}

	UEditorGeometryGenerationSubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorGeometryGenerationSubsystem>();
	if (ensure(Subsystem))
	{
		Subsystem->GenerationManager->UnregisterGeneratedMeshActor(Actor);
	}
}


