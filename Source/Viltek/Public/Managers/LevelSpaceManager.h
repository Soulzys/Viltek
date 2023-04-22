#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelSpaceManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_LevelSpaceManager, Log, All);

struct FArchVolume;

UCLASS()
class VILTEK_API ULevelSpaceManager : public UObject
{
	GENERATED_BODY()

public:

	ULevelSpaceManager();

public:

	void ADD_ArchVolume(FArchVolume* p_ArchVolumeRef);

protected:

	TArray<FArchVolume*> m_Volumes;
	
};
