#include "Architecture/ArchSas.h"
#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/MeshRepairFunctions.h"
#include "GeometryScript/MeshBasicEditFunctions.h"
#include "Engine/StaticMeshSocket.h"
#include "../../Public/Architecture/ArchBuilderBase.h"

DEFINE_LOG_CATEGORY(LogClass_ArchSas);

AArchSas::AArchSas()
{
	// Variable initialization
	PrimaryActorTick.bCanEverTick = false;
	m_FacingDirection = ECardinalDirection::Unknown;

	const ConstructorHelpers::FObjectFinder<UStaticMesh>OF_SM_Sas(TEXT("StaticMesh'/Game/Geometry/Architecture/SM_Sas_2mX2m5X60cm.SM_Sas_2mX2m5X60cm'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh>OF_SM_SasObstacle(TEXT("StaticMesh'/Game/Geometry/Architecture/SM_SasObstacle_2mX2m5X60cm.SM_SasObstacle_2mX2m5X60cm'"));

	check(OF_SM_Sas.Succeeded());
	check(OF_SM_SasObstacle.Succeeded());

	m_SMC_Sas = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMC_Sas"));
	SetRootComponent(m_SMC_Sas);
	m_SMC_Sas->SetStaticMesh(OF_SM_Sas.Object);
	m_SMSK_AttachBegin = m_SMC_Sas->GetSocketByName(TEXT("SMSK_AttachBegin"));
	m_SMSK_AttachEnd   = m_SMC_Sas->GetSocketByName(TEXT("SMSK_AttachEnd"  ));

	check(m_SMSK_AttachBegin);
	check(m_SMSK_AttachEnd);

	m_SMC_SasObstacle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMC_SasObstacle"));
	m_SMC_SasObstacle->SetupAttachment(RootComponent);
	//m_SMC_SasObstacle->SetStaticMesh(OF_SM_SasObstacle.Object);

	m_DMC_Testing = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_Testing"));
	m_DMC_Testing->SetupAttachment(m_SMC_Sas);
}

void AArchSas::BeginPlay()
{
	Super::BeginPlay();	

	UE_LOG(LogClass_ArchSas, Warning, TEXT("I'm here, ArchSas !"));

	//BUILD_Sas();
}

void AArchSas::BUILD_Sas()
{
	check(m_DMC_Testing);

	UDynamicMesh* _DynMesh = m_DMC_Testing->GetDynamicMesh();
	check(_DynMesh);

	FGeometryScriptPrimitiveOptions _PrimOpts;
	FTransform _DefaultTrans;

	_DynMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		_DynMesh, 
		_PrimOpts, 
		_DefaultTrans, 
		200.0f, 
		40.0f, 
		200.0f
	);

	// Cutout setup
	UDynamicMesh* _MainCutout = NewObject<UDynamicMesh>(this);
	_MainCutout = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		_MainCutout,
		_PrimOpts, 
		_DefaultTrans, 
		180.0f, 
		45.0f, 
		188.0f
	);

	FGeometryScriptMeshBooleanOptions _BooleanOpts;
	FTransform _BooleanTransform;
	_BooleanTransform.SetLocation(FVector(0.0f, 0.0f, 2.0f));

	// First boolean operation
	_DynMesh = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		_DynMesh, 
		_DefaultTrans, 
		_MainCutout,
		_BooleanTransform, 
		EGeometryScriptBooleanOperation::Subtract,
		_BooleanOpts
	);

	// Find a way to "refresh" _Cutout geometry

	UDynamicMesh* _SideCutout = NewObject<UDynamicMesh>(this);
	_SideCutout = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		_SideCutout,
		_PrimOpts, 
		_BooleanTransform, 
		50.0f,
		30.0f,
		220.0f
	);

	// First side boolean operation
	_BooleanTransform.SetLocation(FVector(120.0f, 0.0f, -20.0f));
	_DynMesh = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		_DynMesh, 
		_DefaultTrans, 
		_SideCutout,
		_BooleanTransform, 
		EGeometryScriptBooleanOperation::Subtract, 
		_BooleanOpts
	);

	// Second side boolean operation
	_BooleanTransform.SetLocation(FVector(-120.0f, 0.0f, -20));
	_DynMesh = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		_DynMesh,
		_DefaultTrans,
		_SideCutout,
		_BooleanTransform,
		EGeometryScriptBooleanOperation::Subtract,
		_BooleanOpts
	);

	// Third side boolean operation
	FRotator _Rot = FRotator(90.0f, 0.0f, 0.0f);
	_BooleanTransform.SetRotation(_Rot.Quaternion());
	_BooleanTransform.SetLocation(FVector(110.0f, 0.0f, 220.0f));
	_DynMesh = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		_DynMesh,
		_DefaultTrans,
		_SideCutout,
		_BooleanTransform,
		EGeometryScriptBooleanOperation::Subtract,
		_BooleanOpts
	);
}

const FVector AArchSas::GET_SocketBeginLocation() const
{
	if (m_SMSK_AttachBegin == nullptr)
	{
		UE_LOG(LogClass_ArchSas, Error, TEXT("FUNCTION ''GET_SocketBeginLocation'' ==>> SMSK_AttachBegin socket is NULL !"));
		//return FVector(0);
	}

	return GetActorLocation() + m_SMSK_AttachBegin->RelativeLocation;
}

const FVector AArchSas::GET_SocketEndLocation() const
{
	if (m_SMSK_AttachEnd == nullptr)
	{
		UE_LOG(LogClass_ArchSas, Error, TEXT("FUNCTION ''GET_SocketEndLocation'' ==>> SMSK_AttachEnd socket is NULL !"));
		//return FVector(0);
	}

	return GetActorLocation() + m_SMSK_AttachEnd->RelativeLocation;
}

const FRotator AArchSas::GET_SocketBeginRotation() const
{
	return m_SMSK_AttachBegin->RelativeRotation;
}

const FRotator AArchSas::GET_SocketEndRotation() const
{
	return GetActorRotation();
	//return m_SMSK_AttachEnd->RelativeRotation;
}

void AArchSas::SET_Rotation(ECardinalDirection p_WallDirection, float p_WallsThickness, const FVector& p_WallLocation)
{
	// Safety check
	if (p_WallDirection == ECardinalDirection::Unknown)
	{
		UE_LOG(LogClass_ArchSas, Error, TEXT("FUNCTION ''SET_Rotation'' ==>> p_WallDirection is Unknown !"));
		m_FacingDirection = ECardinalDirection::Unknown;
		return;
	}

	if (p_WallDirection == ECardinalDirection::North)
	{
		SetActorLocation(p_WallLocation + FVector(p_WallsThickness / 2.0f, 0.0f, 0.0f));
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
		m_FacingDirection = ECardinalDirection::North;
	}
	else if (p_WallDirection == ECardinalDirection::South)
	{
		SetActorLocation(p_WallLocation + FVector(- p_WallsThickness / 2.0f, 0.0f, 0.0f));
		SetActorRotation(FRotator(0.0f, 270.0f, 0.0f));
		m_FacingDirection = ECardinalDirection::South;
	}
	else if (p_WallDirection == ECardinalDirection::East)
	{
		SetActorLocation(p_WallLocation + FVector(0.0f, p_WallsThickness / 2.0f, 0.0f));
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		m_FacingDirection = ECardinalDirection::East;
	}
	else
	{
		SetActorLocation(p_WallLocation + FVector(0.0f, - p_WallsThickness / 2.0f, 0.0f));
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		m_FacingDirection = ECardinalDirection::West;
	}	
}

const ECardinalDirection AArchSas::GET_ArchFacingDirection()
{
	return m_FacingDirection;
}