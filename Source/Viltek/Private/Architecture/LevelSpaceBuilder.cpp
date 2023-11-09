#include "Architecture/LevelSpaceBuilder.h"
#include "../../Public/Tools/VolumeShell.h"

void ALevelSpaceBuilder::BUILD_LevelSpace()
{
	m_ActualVolumeShell->COMPUTESET_SpaceOffset(10);
	m_ActualVolumeShell->GENERATE_WorldShell();
}

void ALevelSpaceBuilder::GENERATE_ActualVolumeShell(AVolumeShell* p_StartingRoomVolumeShell, AVolumeShell* p_EndingRoomVolumeShell)
{
	if (p_StartingRoomVolumeShell == nullptr || p_EndingRoomVolumeShell == nullptr)
	{
		return;
	}

	float _LargestX = p_StartingRoomVolumeShell->GET_LargestX() ;
	float _LargestY = p_StartingRoomVolumeShell->GET_LargestY() ;
	float _LargestZ = p_StartingRoomVolumeShell->GET_LargestZ() ;
	float _LowestX  = p_StartingRoomVolumeShell->GET_LowestX()  ;
	float _LowestY  = p_StartingRoomVolumeShell->GET_LowestY()  ;
	float _LowestZ  = p_StartingRoomVolumeShell->GET_LowestZ()  ;

	if (_LargestX < p_EndingRoomVolumeShell->GET_LargestX())
	{
		_LargestX = p_EndingRoomVolumeShell->GET_LargestX();
	}

	if (_LowestX > p_EndingRoomVolumeShell->GET_LowestX())
	{
		_LowestX = p_EndingRoomVolumeShell->GET_LowestX();
	}

	if (_LargestY < p_EndingRoomVolumeShell->GET_LargestY())
	{
		_LargestY = p_EndingRoomVolumeShell->GET_LargestY();
	}

	if (_LowestY > p_EndingRoomVolumeShell->GET_LowestY())
	{
		_LowestY = p_EndingRoomVolumeShell->GET_LowestY();
	}

	if (_LargestZ < p_EndingRoomVolumeShell->GET_LargestZ())
	{
		_LargestZ = p_EndingRoomVolumeShell->GET_LargestZ();
	}

	if (_LowestZ > p_EndingRoomVolumeShell->GET_LowestZ())
	{
		_LowestZ = p_EndingRoomVolumeShell->GET_LowestZ();
	}

	FVector _TopRight_Bottom = FVector(_LargestX, _LargestY, _LowestZ);
	FVector _BottomLeft_Top = FVector(_LowestX, _LowestY, _LargestZ);

	FVector _MiddlePoint = (_TopRight_Bottom - _BottomLeft_Top) / 2.0f + _BottomLeft_Top;

	m_ActualVolumeShell = GetWorld()->SpawnActor<AVolumeShell>(_MiddlePoint, FRotator(0));
	m_ActualVolumeShell->COMPUTESET_VolumeDimensions(_LargestX, _LargestY, _LargestZ, _LowestX, _LowestY, _LowestZ);	
}

void ALevelSpaceBuilder::DEBUG_DisplayLevelVolume()
{
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::TopRight_Bottom   ) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::BottomRight_Bottom) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::BottomLeft_Bottom ) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::TopLeft_Bottom    ) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::TopRight_Top      ) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::BottomRight_Top   ) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::BottomLeft_Top    ) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), m_ActualVolumeShell->GET_CornerPosition(ECornerType::TopLeft_Top       ) , 45.0f, 12, FColor::Blue, true, 0, 10.0f);
}