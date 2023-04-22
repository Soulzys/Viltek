#include "Architecture/CorridorTest.h"

DEFINE_LOG_CATEGORY(LogClass_CorridorTest);

ACorridorTest::ACorridorTest()
{

}

void ACorridorTest::BUILD_Corridor()
{
	FVector _CorridorStart = GetActorLocation();
	FVector _CorridorEnd = _CorridorStart + FVector(1500.0f, 0.0f, 0.0f);

	DrawDebugSphere(GetWorld(), _CorridorStart, 20.0f, 10, FColor::Black, true, 0, 10.0f);
	DrawDebugSphere(GetWorld(), _CorridorEnd, 20.0f, 10, FColor::Black, true, 0, 10.0f);
	DrawDebugLine(GetWorld(), _CorridorStart, _CorridorEnd, FColor::Cyan, true, 0, 10.0f);
}