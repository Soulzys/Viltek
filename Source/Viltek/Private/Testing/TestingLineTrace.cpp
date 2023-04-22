#include "Testing/TestingLineTrace.h"
#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"

ATestingLineTrace::ATestingLineTrace()
{
	PrimaryActorTick.bCanEverTick = false;

	m_DMC_TestingCube = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_TestingCube"));
	SetRootComponent(m_DMC_TestingCube);
	m_DMC_TestingCube->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_DMC_TestingCube->EnableComplexAsSimpleCollision();
	m_DMC_TestingCube->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ATestingLineTrace::BeginPlay()
{
	Super::BeginPlay();

	BUILD_Mesh();
	//TRACE_Line();

	DrawDebugSphere(GetWorld(), FVector(0), 25.0f, 8, FColor::Orange, true, 0, 10.0f);
}

void ATestingLineTrace::BUILD_Mesh()
{
	FGeometryScriptPrimitiveOptions _primitiveOptions;
	FTransform _defaultTransform;
	FRotator _Rotator = FRotator(90.0f, 0.0f, 0.0f);
	_defaultTransform.SetRotation(_Rotator.Quaternion());

	/*m_DMC_TestingCube->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		m_DMC_TestingCube->GetDynamicMesh(), 
		_primitiveOptions, 
		_defaultTransform, 
		200.0f, 200.0f, 200.0f
	));*/

	m_DMC_TestingCube->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendRectangleXY
	(
		m_DMC_TestingCube->GetDynamicMesh(), 
		_primitiveOptions, 
		_defaultTransform, 
		500.0f, 250.0f
	));
}

void ATestingLineTrace::TRACE_Line()
{
	//m_DMC_TestingCube->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//m_DMC_TestingCube->EnableComplexAsSimpleCollision();
	//m_DMC_TestingCube->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	FHitResult _HitResult;

	FVector _RayStartLoc = GetActorLocation() + FVector(250.0f, 0.0f, 50.0f);
	FVector _RayEndLoc = _RayStartLoc + FVector(-550.0f, 0.0f, 0.0f);

	FCollisionQueryParams _QueryParams;
	_QueryParams.bTraceComplex = true; // Is this necessary ? So far, we are only using simple collisions...

	bool _bIsHit = GetWorld()->LineTraceSingleByChannel(_HitResult, _RayStartLoc, _RayEndLoc, ECC_Visibility, _QueryParams);
	DrawDebugLine(GetWorld(), _RayStartLoc, _RayEndLoc, FColor::Orange, false, 10.0f, 0, 2.5f);

	if (_bIsHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("I have hit something !"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I haven't hit shit !"));
	}
}