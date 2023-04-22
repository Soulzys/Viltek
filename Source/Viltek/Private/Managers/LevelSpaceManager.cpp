#include "Managers/LevelSpaceManager.h"
#include "../../Public/Architecture/ArchRoomBuilderBase.h"

DEFINE_LOG_CATEGORY(LogClass_LevelSpaceManager);

ULevelSpaceManager::ULevelSpaceManager()
{

}

void ULevelSpaceManager::ADD_ArchVolume(FArchVolume* p_ArchVolumeRef)
{
	if (p_ArchVolumeRef == nullptr)
	{
		UE_LOG(LogClass_LevelSpaceManager, Error, TEXT("FUNCTION ''ADD_ArchVolume'' ==>> p_ArchVolumeRef is NULL !"));
		return;
	}

	m_Volumes.Add(p_ArchVolumeRef);
	//GENERATE_LevelSpace();
}