#include "Architecture/CorridorBuilderTest.h"
#include "Architecture/ArchBuilderBase.h"
#include "Architecture/ArchRoomBuilderBase.h"

DEFINE_LOG_CATEGORY(LogClass_CorridorBuilderTest);

ACorridorBuilderTest::ACorridorBuilderTest()
{
	m_CorridorWidth = 4;
}

void ACorridorBuilderTest::BUILD_CorridorTest(const FCorridorBuilderTestOptions& p_BuildOptions)
{
	FVector _StartLoc = p_BuildOptions.s_RoomStart->GET_DoorLocation();
	FVector _EndLoc   = p_BuildOptions.s_RoomEnd  ->GET_DoorLocation() ;

	ECardinalDirection _StartDoorDir = p_BuildOptions.s_RoomStart->GET_DoorDirection() ;
	ECardinalDirection _EndDoorDir   = p_BuildOptions.s_RoomEnd  ->GET_DoorDirection() ;

	//FArchVolume* _StartRoomVolume = p_BuildOptions.s_RoomStart->GET_RoomVolume();
	//FArchVolume* _EndRoomVolume   = p_BuildOptions.s_RoomEnd->GET_RoomVolume();
	//check(_StartRoomVolume);
	//check(_EndRoomVolume);

	FVector _FirstSecondPoint;
	COMPUTE_CorridorPoint(_StartDoorDir, 2, _StartLoc, _FirstSecondPoint);

	/*if (_StartDoorDir == ECardinalDirection::North)
	{
		_FirstSecondPoint = _StartLoc + FVector(200.0f, 0.0f, 0.0f);
	}
	else if (_StartDoorDir == ECardinalDirection::South)
	{
		_FirstSecondPoint = _StartLoc + FVector(-200.0f, 0.0f, 0.0f);
	}
	else if (_StartDoorDir == ECardinalDirection::East)
	{
		_FirstSecondPoint = _StartLoc + FVector(0.0f, 200.0f, 0.0f);
	}
	else if (_StartDoorDir == ECardinalDirection::West)
	{
		_FirstSecondPoint = _StartLoc + FVector(0.0f, -200.0f, 0.0f);
	}
	else
	{

	}*/

	FVector _LastSecondPoint;
	COMPUTE_CorridorPoint(_EndDoorDir, 2, _EndLoc, _LastSecondPoint);

	/*if (_EndDoorDir == ECardinalDirection::North)
	{
		_LastSecondPoint = _EndLoc + FVector(200.0f, 0.0f, 0.0f);
	}
	else if (_EndDoorDir == ECardinalDirection::South)
	{
		_LastSecondPoint = _EndLoc + FVector(-200.0f, 0.0f, 0.0f);
	}
	else if (_EndDoorDir == ECardinalDirection::East)
	{
		_LastSecondPoint = _EndLoc + FVector(0.0f, 200.0f, 0.0f);
	}
	else if (_EndDoorDir == ECardinalDirection::West)
	{
		_LastSecondPoint = _EndLoc + FVector(0.0f, -200.0f, 0.0f);
	}
	else
	{

	}*/

	DrawDebugLine(GetWorld(), _StartLoc, _FirstSecondPoint, FColor::Red, true, 0, 50.0f);
	DrawDebugLine(GetWorld(), _FirstSecondPoint, _LastSecondPoint, FColor::Red, true, 0, 50.0f);
	DrawDebugLine(GetWorld(), _LastSecondPoint, _EndLoc, FColor::Red, true, 0, 50.0f);
}

void ACorridorBuilderTest::COMPUTE_CorridorPoint(ECardinalDirection p_Direction, uint32 p_RoomExteriorDistance, const FVector& p_StartLoc, FVector& p_OutVector)
{
	// Safety check
	if (p_Direction == ECardinalDirection::Unknown)
	{
		UE_LOG(LogClass_CorridorBuilderTest, Error, TEXT("FUNCTION ''COMPUTE_CorridorPoint'' ==>> Direction is Unknown !"));
		return;
	}

	float _DepthDistance = (float)(p_RoomExteriorDistance + m_CorridorWidth) * 100.0f;

	if (p_Direction == ECardinalDirection::North)
	{
		p_OutVector = p_StartLoc + FVector(_DepthDistance, 0.0f, 0.0f);
	}
	else if (p_Direction == ECardinalDirection::South)
	{
		p_OutVector = p_StartLoc + FVector(- _DepthDistance, 0.0f, 0.0f);
	}
	else if (p_Direction == ECardinalDirection::East)
	{
		p_OutVector = p_StartLoc + FVector(0.0f, _DepthDistance, 0.0f);
	}
	else
	{
		p_OutVector = p_StartLoc + FVector(0.0f, - _DepthDistance, 0.0f);
	}
}

/*void ACorridorBuilderTest::COMPUTE_Path(const FVector& p_StartLoc, const FVector& p_EndLoc, const FArchVolume& p_StartRoomVolume, const FArchVolume& p_EndRoomVolume)
{
	if (p_StartLoc.X > p_EndLoc.X) // If StartLoc is "above" EndLoc
	{

	}
}*/