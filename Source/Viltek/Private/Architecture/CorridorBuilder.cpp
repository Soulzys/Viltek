#include "Architecture/CorridorBuilder.h"
#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "../../Public/Tools/FunctionsLibrary/Geometry/MeshConvertor.h"
#include "../../Public/Tools/FunctionsLibrary/Geometry/ViltekGeoLib_DoorMaking.h"

DEFINE_LOG_CATEGORY(LogClass_CorridorBuilder);

ACorridorBuilder::ACorridorBuilder()
{
	m_CorridorBaseWidth = 4;
}

void ACorridorBuilder::BeginPlay()
{
	Super::BeginPlay();
}

void ACorridorBuilder::BUILD_Corridor(const FVector& p_StartPos, ECardinalDirection p_StartPosFacingDir, const FVector& p_EndPos, ECardinalDirection p_EndPosFacingDir, uint8 p_SafeRoomSpace)
{
	DrawDebugSphere(GetWorld(), p_StartPos, 35.0f, 12, FColor::Purple, true, 0, 20.0f);
	DrawDebugSphere(GetWorld(), p_EndPos, 35.0f, 12, FColor::Purple, true, 0, 20.0f);

	m_CorridorBaseWidth = 4 / 2;

	float _SafeRoomSpace = (float)p_SafeRoomSpace * 100.0f;

	FVector _FirstPoint;
	FVector _LastPoint;

	if (p_StartPosFacingDir == ECardinalDirection::North)
	{
		_FirstPoint = p_StartPos + FVector(_SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f, 0.0f, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("StartRoom Direction : North"));
	}
	else if (p_StartPosFacingDir == ECardinalDirection::South)
	{
		_FirstPoint = p_StartPos + FVector(- (_SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f), 0.0f, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("StartRoom Direction : South"));
	}
	else if (p_StartPosFacingDir == ECardinalDirection::East)
	{
		_FirstPoint = p_StartPos + FVector(0.0f, _SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("StartRoom Direction : East"));
	}
	else if (p_StartPosFacingDir == ECardinalDirection::West)
	{
		_FirstPoint = p_StartPos + FVector(0.0f, -(_SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f), 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("StartRoom Direction : West"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nonsensewwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww !"));
	}

	//
	//

	if (p_EndPosFacingDir == ECardinalDirection::North)
	{
		_LastPoint = p_EndPos + FVector(_SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f, 0.0f, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("EndRoom Direction : North"));
	}
	else if (p_EndPosFacingDir == ECardinalDirection::South)
	{
		_LastPoint = p_EndPos + FVector(-(_SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f), 0.0f, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("EndRoom Direction : South"));
	}
	else if (p_EndPosFacingDir == ECardinalDirection::East)
	{
		_LastPoint = p_EndPos + FVector(0.0f, _SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("EndRoom Direction : East"));
	}
	else if (p_EndPosFacingDir == ECardinalDirection::West)
	{
		_LastPoint = p_EndPos + FVector(0.0f, -(_SafeRoomSpace + (float)m_CorridorBaseWidth * 100.0f), 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("EndRoom Direction : West"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nonsensewwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww !"));
	}

	DrawDebugSphere(GetWorld(), _FirstPoint, 35.0f, 12, FColor::Purple, true, 0, 20.0f);
	DrawDebugSphere(GetWorld(), _LastPoint, 35.0f, 12, FColor::Purple, true, 0, 20.0f);

	DrawDebugLine(GetWorld(), _FirstPoint, _LastPoint, FColor::Red, true, 0, 10.0f);

	UE_LOG(LogTemp, Warning, TEXT("FirstPoint location : %s"), *_FirstPoint.ToString());
	UE_LOG(LogTemp, Warning, TEXT("LastPoint location : %s"), *_LastPoint.ToString());
	
	
	float _RawDistance = FVector::Dist(_FirstPoint, _LastPoint);

	UE_LOG(LogTemp, Warning, TEXT("Raw distance : %f"), _RawDistance);

	//
	//

	if (p_StartPosFacingDir == ECardinalDirection::North)
	{
		SHOOT_Ray(_FirstPoint, ECardinalDirection::North);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::East);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::West);
	}
	if (p_StartPosFacingDir == ECardinalDirection::South)
	{
		SHOOT_Ray(_FirstPoint, ECardinalDirection::South);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::East);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::West);
	}
	if (p_StartPosFacingDir == ECardinalDirection::East)
	{
		SHOOT_Ray(_FirstPoint, ECardinalDirection::North);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::South);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::East);
	}
	if (p_StartPosFacingDir == ECardinalDirection::West)
	{
		SHOOT_Ray(_FirstPoint, ECardinalDirection::North);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::South);
		SHOOT_Ray(_FirstPoint, ECardinalDirection::West);
	}
}

void ACorridorBuilder::SHOOT_Ray(const FVector p_RayStartLoc, ECardinalDirection p_RayDir)
{
	FHitResult _HitResult;

	float _RayShootingDistance = 10000.0f;

	FVector _RayEndLoc;

	if (p_RayDir == ECardinalDirection::North)
	{
		_RayEndLoc = p_RayStartLoc + FVector(_RayShootingDistance, 0.0f, 0.0f);
	}
	else if (p_RayDir == ECardinalDirection::South)
	{
		_RayEndLoc = p_RayStartLoc + FVector(- _RayShootingDistance, 0.0f, 0.0f);
	}
	else if (p_RayDir == ECardinalDirection::East)
	{
		_RayEndLoc = p_RayStartLoc + FVector(0.0f, _RayShootingDistance, 0.0f);
	}
	else if (p_RayDir == ECardinalDirection::West)
	{
		_RayEndLoc = p_RayStartLoc + FVector(0.0f, - _RayShootingDistance, 0.0f);
	}
	else
	{

	}

	FCollisionQueryParams _QueryParams;
	_QueryParams.bTraceComplex = true;

	bool _bIsHit = GetWorld()->LineTraceSingleByChannel(_HitResult, p_RayStartLoc, _RayEndLoc, ECC_Visibility, _QueryParams);
	DrawDebugLine(GetWorld(), p_RayStartLoc, _RayEndLoc, FColor::Green, true, 0, 10.0f);

	if (_bIsHit)
	{
		DrawDebugSphere(GetWorld(), _HitResult.ImpactPoint, 35.0f, 12, FColor::Purple, true, 0, 10.0f);
	}
}

//
//

// Shoot 3 rays with a length value of 250.0f in all directions but the one it is coming from
// 