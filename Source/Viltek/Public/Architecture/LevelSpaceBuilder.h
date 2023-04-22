#pragma once

#include "CoreMinimal.h"
#include "Architecture/ArchBuilderBase.h"
#include "LevelSpaceBuilder.generated.h"

class AVolumeShell;

UCLASS()
//class VILTEK_API ALevelSpaceBuilder : public AArchBuilderBase
class VILTEK_API ALevelSpaceBuilder : public AActor
{
	GENERATED_BODY()

public:

	void BUILD_LevelSpace();

	void GENERATE_ActualVolumeShell(AVolumeShell* p_StartingRoomVolumeShell, AVolumeShell* p_EndingRoomVolumeShell); // The one we are currently using

	void DEBUG_DisplayLevelVolume();

protected:

	TArray<AVolumeShell*> m_VolumeShells; // So far, this is actually "unused" as we are only working with two AVolumeShell : the Starting and the Ending rooms' ones
	AVolumeShell* m_ActualVolumeShell = nullptr;
};
