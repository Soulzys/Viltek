#include "Tools/VolumeShell.h"
#include "../../Public/Architecture/ArchBuilderBase.h"
#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"

AVolumeShell::AVolumeShell()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setting root
	m_SC_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SC_RootComponent"));
	SetRootComponent(m_SC_RootComponent);

	// Setting the shell components
	m_DMC_ShellSide_1 = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ShellSide_1"));
	m_DMC_ShellSide_1->SetMobility(EComponentMobility::Movable);
	m_DMC_ShellSide_1->SetGenerateOverlapEvents(false);
	m_DMC_ShellSide_1->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ShellSide_1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ShellSide_1->EnableComplexAsSimpleCollision();
	m_DMC_ShellSide_1->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ShellSide_1->SetupAttachment(RootComponent);

	m_DMC_ShellSide_2 = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ShellSide_2"));
	m_DMC_ShellSide_2->SetMobility(EComponentMobility::Movable);
	m_DMC_ShellSide_2->SetGenerateOverlapEvents(false);
	m_DMC_ShellSide_2->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ShellSide_2->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ShellSide_2->EnableComplexAsSimpleCollision();
	m_DMC_ShellSide_2->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ShellSide_2->SetupAttachment(RootComponent);

	m_DMC_ShellSide_3 = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ShellSide_3"));
	m_DMC_ShellSide_3->SetMobility(EComponentMobility::Movable);
	m_DMC_ShellSide_3->SetGenerateOverlapEvents(false);
	m_DMC_ShellSide_3->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ShellSide_3->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ShellSide_3->EnableComplexAsSimpleCollision();
	m_DMC_ShellSide_3->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ShellSide_3->SetupAttachment(RootComponent);

	m_DMC_ShellSide_4 = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ShellSide_4"));
	m_DMC_ShellSide_4->SetMobility(EComponentMobility::Movable);
	m_DMC_ShellSide_4->SetGenerateOverlapEvents(false);
	m_DMC_ShellSide_4->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ShellSide_4->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ShellSide_4->EnableComplexAsSimpleCollision();
	m_DMC_ShellSide_4->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ShellSide_4->SetupAttachment(RootComponent);

	m_DMC_ShellSide_5 = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ShellSide_5"));
	m_DMC_ShellSide_5->SetMobility(EComponentMobility::Movable);
	m_DMC_ShellSide_5->SetGenerateOverlapEvents(false);
	m_DMC_ShellSide_5->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ShellSide_5->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ShellSide_5->EnableComplexAsSimpleCollision();
	m_DMC_ShellSide_5->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ShellSide_5->SetupAttachment(RootComponent);

	m_DMC_ShellSide_6 = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_ShellSide_6"));
	m_DMC_ShellSide_6->SetMobility(EComponentMobility::Movable);
	m_DMC_ShellSide_6->SetGenerateOverlapEvents(false);
	m_DMC_ShellSide_6->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_DMC_ShellSide_6->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_ShellSide_6->EnableComplexAsSimpleCollision();
	m_DMC_ShellSide_6->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_DMC_ShellSide_6->SetupAttachment(RootComponent);


	//
	//

	
	UE_LOG(LogTemp, Warning, TEXT("VOLUME SPAWNED !"));
}

void AVolumeShell::GENERATE_RoomShell()
{
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f);

	DrawDebugSphere(GetWorld(), GetActorLocation(), 25.0f, 8, FColor::Green, true, 0, 10.0f);

	FGeometryScriptPrimitiveOptions _PrimOpts;
	FTransform _Transform;

	m_DMC_ShellSide_1->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_1->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Height,
		m_Width
	));	

	m_DMC_ShellSide_2->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_2->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Height
	));	

	m_DMC_ShellSide_3->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_3->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Height,
		m_Width
	));

	m_DMC_ShellSide_4->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_4->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Height
	));	

	m_DMC_ShellSide_5->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_5->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Width
	));	

	m_DMC_ShellSide_6->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_6->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Width
	));	
	
	m_DMC_ShellSide_1->SetRelativeLocation(FVector(  m_Length / 2.0f ,   0.0f           ,   0.0f           )) ;
	m_DMC_ShellSide_2->SetRelativeLocation(FVector(  0.0f            ,   m_Width / 2.0f ,   0.0f           )) ;
	m_DMC_ShellSide_3->SetRelativeLocation(FVector(- m_Length / 2.0f ,   0.0f           ,   0.0f           )) ;
	m_DMC_ShellSide_4->SetRelativeLocation(FVector(  0.0f            , - m_Width / 2.0f ,   0.0f           )) ;
	m_DMC_ShellSide_5->SetRelativeLocation(FVector(  0.0f            ,   0.0f           ,   m_Height / 2.0f)) ;
	m_DMC_ShellSide_6->SetRelativeLocation(FVector(  0.0f            ,   0.0f           , - m_Height / 2.0f)) ;

	m_DMC_ShellSide_1->SetRelativeRotation(FRotator(- 90.0f , 0.0f,   0.0f  )) ;
	m_DMC_ShellSide_2->SetRelativeRotation(FRotator(  0.0f  , 0.0f,   90.0f )) ;
	m_DMC_ShellSide_3->SetRelativeRotation(FRotator(  90.0f , 0.0f,   0.0f  )) ;
	m_DMC_ShellSide_4->SetRelativeRotation(FRotator(  0.0f  , 0.0f, - 90.0f )) ;
	m_DMC_ShellSide_5->SetRelativeRotation(FRotator(  0.0f  , 0.0f,   0.0f  )) ;
	m_DMC_ShellSide_6->SetRelativeRotation(FRotator(  0.0f  , 0.0f,   180.0f)) ;

	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_1->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_2->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_3->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_4->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_5->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_6->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
}

void AVolumeShell::GENERATE_WorldShell()
{
	UE_LOG(LogTemp, Warning, TEXT("GENERATING WORLD SHELL !!"));

	DrawDebugSphere(GetWorld(), GetActorLocation(), 25.0f, 8, FColor::Green, true, 0, 10.0f);

	FGeometryScriptPrimitiveOptions _PrimOpts;
	FTransform _Transform;

	m_DMC_ShellSide_1->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_1->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Height,
		m_Width
	));	

	m_DMC_ShellSide_2->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_2->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Height
	));	

	m_DMC_ShellSide_3->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_3->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Height,
		m_Width
	));

	m_DMC_ShellSide_4->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_4->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Height
	));	

	m_DMC_ShellSide_5->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_5->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Width
	));	

	m_DMC_ShellSide_6->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_ShellSide_6->GetDynamicMesh(),
		_PrimOpts,
		_Transform, 
		m_Length,
		m_Width
	));	
	
	m_DMC_ShellSide_1->SetRelativeLocation(FVector(  m_Length / 2.0f ,   0.0f           ,   0.0f           )) ;
	m_DMC_ShellSide_2->SetRelativeLocation(FVector(  0.0f            ,   m_Width / 2.0f ,   0.0f           )) ;
	m_DMC_ShellSide_3->SetRelativeLocation(FVector(- m_Length / 2.0f ,   0.0f           ,   0.0f           )) ;
	m_DMC_ShellSide_4->SetRelativeLocation(FVector(  0.0f            , - m_Width / 2.0f ,   0.0f           )) ;
	m_DMC_ShellSide_5->SetRelativeLocation(FVector(  0.0f            ,   0.0f           ,   m_Height / 2.0f)) ;
	m_DMC_ShellSide_6->SetRelativeLocation(FVector(  0.0f            ,   0.0f           , - m_Height / 2.0f)) ;

	m_DMC_ShellSide_1->SetRelativeRotation(FRotator(  90.0f , 0.0f,   0.0f  )) ;
	m_DMC_ShellSide_2->SetRelativeRotation(FRotator(  0.0f  , 0.0f, - 90.0f )) ;
	m_DMC_ShellSide_3->SetRelativeRotation(FRotator(- 90.0f , 0.0f,   0.0f  )) ;
	m_DMC_ShellSide_4->SetRelativeRotation(FRotator(  0.0f  , 0.0f,   90.0f )) ;
	m_DMC_ShellSide_5->SetRelativeRotation(FRotator(  0.0f  , 0.0f,   180.0f)) ;
	m_DMC_ShellSide_6->SetRelativeRotation(FRotator(  0.0f  , 0.0f,   0.0f  )) ;

	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_1->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_2->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_3->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_4->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_5->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
	DrawDebugSphere(GetWorld(), GetActorLocation() + m_DMC_ShellSide_6->GetRelativeLocation(), 25.0f, 8, FColor::Red, true, 0, 10.0f) ;
}

void AVolumeShell::SET_ArchVolume(FArchVolume* p_ArchVolumeRef)
{
	if (p_ArchVolumeRef == nullptr)
	{
		return;
	}

	m_ArchVolume = p_ArchVolumeRef;
}

void AVolumeShell::SET_ArchVolumeCuboid(FArchVolumeCuboid* p_ArchVolumeCuboidRef)
{
	if (p_ArchVolumeCuboidRef == nullptr)
	{
		return;
	}

	m_ArchVolumeCuboid = p_ArchVolumeCuboidRef;
}

FVector AVolumeShell::COMPUTE_VectorsMiddleLocation(const FVector& p_V1, const FVector& p_V2)
{
	FVector _MiddleLocation = (p_V1 - p_V2) / 2.0f + p_V2;

	return _MiddleLocation;
}

void AVolumeShell::COMPUTESET_VolumeDimensions
(
	float p_LargestX , 
	float p_LargestY , 
	float p_LargestZ , 
	float p_LowestX  , 
	float p_LowestY  , 
	float p_LowestZ
)
{
	m_LargestX = p_LargestX ;
	m_LargestY = p_LargestY ;
	m_LargestZ = p_LargestZ ;
	m_LowestX  = p_LowestX  ;
	m_LowestY  = p_LowestY  ;
	m_LowestZ  = p_LowestZ  ;

	COMPUTE_VolumeSize();

	UE_LOG(LogTemp, Log, TEXT("VolumeShell m_X : %f"), m_Length);
	UE_LOG(LogTemp, Log, TEXT("VolumeShell m_Y : %f"), m_Width );
	UE_LOG(LogTemp, Log, TEXT("VolumeShell m_Z : %f"), m_Height);
}

void AVolumeShell::COMPUTESET_SpaceOffset(uint8 p_SpaceOffset)
{
	m_LargestX += (float)(p_SpaceOffset) * 100.0f;
	m_LargestY += (float)(p_SpaceOffset) * 100.0f;
	m_LargestZ += (float)(p_SpaceOffset) * 100.0f;
	m_LowestX  -= (float)(p_SpaceOffset) * 100.0f;
	m_LowestY  -= (float)(p_SpaceOffset) * 100.0f;
	m_LowestZ  -= (float)(p_SpaceOffset) * 100.0f;

	COMPUTE_VolumeSize();
}

void AVolumeShell::COMPUTE_VolumeSize()
{
	m_Length = FMath::Abs(m_LargestX - m_LowestX);
	m_Width  = FMath::Abs(m_LargestY - m_LowestY);
	m_Height = FMath::Abs(m_LargestZ - m_LowestZ);
}

void AVolumeShell::SET_LargestX(float p_LargestX)
{
	m_LargestX = p_LargestX;
}

void AVolumeShell::SET_LargestY(float p_LargestY)
{
	m_LargestY = p_LargestY;
}

void AVolumeShell::SET_LargestZ(float p_LargestZ)
{
	m_LargestZ = p_LargestZ;
}

void AVolumeShell::SET_LowestX(float p_LowestX)
{
	m_LowestX = p_LowestX;
}

void AVolumeShell::SET_LowestY(float p_LowestY)
{
	m_LowestY = p_LowestY;
}

void AVolumeShell::SET_LowestZ(float p_LowestZ)
{
	m_LowestZ = p_LowestZ;
}

float AVolumeShell::GET_LargestX() const
{
	return m_LargestX;
}

float AVolumeShell::GET_LargestY() const
{
	return m_LargestY;
}

float AVolumeShell::GET_LargestZ() const
{
	return m_LargestZ;
}

float AVolumeShell::GET_LowestX() const
{
	return m_LowestX;
}

float AVolumeShell::GET_LowestY() const
{
	return m_LowestY;
}

float AVolumeShell::GET_LowestZ() const
{
	return m_LowestZ;
}

FVector AVolumeShell::GET_CornerPosition(ECornerType p_CornerType)
{
	FVector _ReturnCorner = FVector(-1);

	switch (p_CornerType)
	{
		case ECornerType::TopRight_Bottom:
			_ReturnCorner = FVector(m_LargestX, m_LargestY, m_LowestZ);
			break;

		case ECornerType::BottomRight_Bottom:
			_ReturnCorner = FVector(m_LowestX, m_LargestY, m_LowestZ);
			break;

		case ECornerType::BottomLeft_Bottom:
			_ReturnCorner = FVector(m_LowestX, m_LowestY, m_LowestZ);
			break;

		case ECornerType::TopLeft_Bottom:
			_ReturnCorner = FVector(m_LargestX, m_LowestY, m_LowestZ);
			break;

		case ECornerType::TopRight_Top:
			_ReturnCorner = FVector(m_LargestX, m_LargestY, m_LargestZ);
			break;

		case ECornerType::BottomRight_Top:
			_ReturnCorner = FVector(m_LowestX, m_LargestY, m_LargestZ);
			break;

		case ECornerType::BottomLeft_Top:
			_ReturnCorner = FVector(m_LowestX, m_LowestY, m_LargestZ);
			break;

		case ECornerType::TopLeft_Top:
			_ReturnCorner = FVector(m_LargestX, m_LowestY, m_LargestZ);
			break;

		default:
			_ReturnCorner = FVector(-1);
			break;
	}

	return _ReturnCorner;
}