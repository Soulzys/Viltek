#include "Tools/Environment/Procedural/FillingCube.h"
#include "../../../../Public/Architecture/StartingRoomBuilder.h"
#include "../../../../Public/Architecture/EndingRoomBuilder.h"

DEFINE_LOG_CATEGORY(LogClass_FillingCube);

AFillingCube::AFillingCube()
{
	PrimaryActorTick.bCanEverTick = false;

	// Var initialization
	m_Face_1_FriendType = EFillingCubeFriendTypes::Void;
	m_Face_2_FriendType = EFillingCubeFriendTypes::Void;
	m_Face_3_FriendType = EFillingCubeFriendTypes::Void;
	m_Face_4_FriendType = EFillingCubeFriendTypes::Void;
	m_Face_5_FriendType = EFillingCubeFriendTypes::Void;
	m_Face_6_FriendType = EFillingCubeFriendTypes::Void;

	m_ObstaclesCount = 0;

	// "_z5" is to be read "zero 5", meaning 0.5 meters
	const ConstructorHelpers::FObjectFinder<UStaticMesh>       OF_FillingMesh_z5(TEXT("StaticMesh'/Game/Geometry/Tools/SM_FillingCube_z5.SM_FillingCube_z5'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_GreenMat      (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Green.MI_SKTC_Green'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_BlueMat       (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Blue.MI_SKTC_Blue'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_YellowMat     (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Yellow.MI_SKTC_Yellow'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_MagentaMat    (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Magenta.MI_SKTC_Magenta'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_CyanMat       (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Cyan.MI_SKTC_Cyan'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_RedMat        (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Red.MI_SKTC_Red'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_BlackMat      (TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Black.MI_SKTC_Black'"));

	check(OF_FillingMesh_z5.Succeeded()) ;
	check(OF_GreenMat      .Succeeded()) ;
	check(OF_BlueMat       .Succeeded()) ;
	check(OF_YellowMat     .Succeeded()) ;
	check(OF_MagentaMat    .Succeeded()) ;
	check(OF_CyanMat       .Succeeded()) ;
	check(OF_RedMat        .Succeeded()) ;
	check(OF_BlackMat      .Succeeded()) ;

	m_MI_SKTC_Green   = OF_GreenMat  .Object ;
	m_MI_SKTC_Blue    = OF_BlueMat   .Object ;
	m_MI_SKTC_Yellow  = OF_YellowMat .Object ;
	m_MI_SKTC_Magenta = OF_MagentaMat.Object ;
	m_MI_SKTC_Cyan    = OF_CyanMat   .Object ;
	m_MI_SKTC_Red     = OF_RedMat    .Object ;
	m_MI_SKTC_Black   = OF_BlackMat  .Object ;

	m_SMC_FillingCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMC_FillingCube"));
	SetRootComponent(m_SMC_FillingCube);
	m_SMC_FillingCube->SetStaticMesh(OF_FillingMesh_z5.Object);
	m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Green);
}

void AFillingCube::BeginPlay()
{
	Super::BeginPlay();
}

EFillingCubeFriendTypes AFillingCube::SHOOT_Ray(const FVector& p_RayTargetLoc)
{
	FHitResult _HitResult;

	FVector _RayStartLoc = GetActorLocation()            ;
	FVector _RayEndLoc   = p_RayTargetLoc + _RayStartLoc ;

	FCollisionQueryParams _QueryParams;
	_QueryParams.bTraceComplex = true; // Is this necessary ? So far, we are only using simple collisions...
	
	bool _bIsHit = GetWorld()->LineTraceSingleByChannel(_HitResult, _RayStartLoc, _RayEndLoc, ECC_Visibility, _QueryParams);
	DrawDebugLine(GetWorld(), _RayStartLoc, _RayEndLoc, FColor::Orange, false, 10.0f, 0, 2.5f);

	if (_bIsHit)
	{
		if (_HitResult.GetActor()->IsA<AStartingRoomBuilder>() || _HitResult.GetActor()->IsA<AEndingRoomBuilder>()) // Probably need to use template here I suppose ? 
		{
			m_ObstaclesCount++; 
			return EFillingCubeFriendTypes::RoomWall;
		}
		else if (_HitResult.GetActor()->IsA<AFillingCube>())
		{
			return EFillingCubeFriendTypes::OtherFillingCube;
		}
		else
		{
			UE_LOG(LogClass_FillingCube, Error, TEXT("FUNCTION ''SHOOT_Ray'' ==>> What we have hit is of unknown type !"));
			return EFillingCubeFriendTypes::Error;
		}
	}
	else
	{
		return EFillingCubeFriendTypes::Void;
	}
}

EFillingCubeFriendTypes AFillingCube::SHOOT_Ray(EFillingCubeFaces p_Face)
{
	// Why 160 ? Because AFillingCube is set by default to be a 50x50x50 cube and we are evaluating by default 100x100x100 space area
	// So 50 + 100 + 10, the latter's purpose being to make sure we hit a potential neighbour AFillingCube
	// NOTE : This eventually needs to be put in a variable based of 'm_SMC_FillingCube' UStaticMesh size
	// Wait a second... isn't it 80 then ? Seems like above statements are kind of bullshit. Need to review that.
	if (p_Face == EFillingCubeFaces::Face_1)
	{
		return SHOOT_Ray(FVector(-80.0f, 0.0f, 0.0f));
	}
	else if (p_Face == EFillingCubeFaces::Face_2)
	{
		return SHOOT_Ray(FVector(0.0f, 80.0f, 0.0f));
	}
	else if (p_Face == EFillingCubeFaces::Face_3)
	{
		return SHOOT_Ray(FVector(80.0f, 0.0f, 0.0f));
	}
	else if (p_Face == EFillingCubeFaces::Face_4)
	{
		return SHOOT_Ray(FVector(0.0f, -80.0f, 0.0f));
	}
	else if (p_Face == EFillingCubeFaces::Face_5)
	{
		return SHOOT_Ray(FVector(0.0f, 0.0f, 80.0f));
	}
	else
	{
		return SHOOT_Ray(FVector(0.0f, 0.0f, -80.0f));
	}
}

void AFillingCube::SHOOT_Rays()
{	
	m_Face_1_FriendType = SHOOT_Ray(EFillingCubeFaces::Face_1); // Shoot from the center toward the Face 1
	m_Face_2_FriendType = SHOOT_Ray(EFillingCubeFaces::Face_2); // Shoot from the center toward the Face 2
	m_Face_3_FriendType = SHOOT_Ray(EFillingCubeFaces::Face_3); // ...
	m_Face_4_FriendType = SHOOT_Ray(EFillingCubeFaces::Face_4); 
	m_Face_5_FriendType = SHOOT_Ray(EFillingCubeFaces::Face_5); 
	m_Face_6_FriendType = SHOOT_Ray(EFillingCubeFaces::Face_6); 

	if (m_Face_1_FriendType == EFillingCubeFriendTypes::Void)
	{
		AFillingCube* _NewFillingCube = GetWorld()->SpawnActor<AFillingCube>(GetActorLocation() + FVector(-100.0f, 0.0f, 0.0f), FRotator(0));
		check(_NewFillingCube);
		m_FriendCubes.Add(_NewFillingCube);

		m_Face_1_FriendType = EFillingCubeFriendTypes::OtherFillingCube;
	}

	if (m_Face_2_FriendType == EFillingCubeFriendTypes::Void)
	{
		AFillingCube* _NewFillingCube = GetWorld()->SpawnActor<AFillingCube>(GetActorLocation() + FVector(0.0f, 100.0f, 0.0f), FRotator(0));
		check(_NewFillingCube);
		m_FriendCubes.Add(_NewFillingCube);

		m_Face_2_FriendType = EFillingCubeFriendTypes::OtherFillingCube;
	}

	if (m_Face_3_FriendType == EFillingCubeFriendTypes::Void)
	{
		AFillingCube* _NewFillingCube = GetWorld()->SpawnActor<AFillingCube>(GetActorLocation() + FVector(100.0f, 0.0f, 0.0f), FRotator(0));
		check(_NewFillingCube);
		m_FriendCubes.Add(_NewFillingCube);

		m_Face_3_FriendType = EFillingCubeFriendTypes::OtherFillingCube;
	}

	if (m_Face_4_FriendType == EFillingCubeFriendTypes::Void)
	{
		AFillingCube* _NewFillingCube = GetWorld()->SpawnActor<AFillingCube>(GetActorLocation() + FVector(0.0f, -100.0f, 0.0f), FRotator(0));
		check(_NewFillingCube);
		m_FriendCubes.Add(_NewFillingCube);

		m_Face_4_FriendType = EFillingCubeFriendTypes::OtherFillingCube;
	}

	if (m_Face_5_FriendType == EFillingCubeFriendTypes::Void)
	{
		AFillingCube* _NewFillingCube = GetWorld()->SpawnActor<AFillingCube>(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f), FRotator(0));
		check(_NewFillingCube);
		m_FriendCubes.Add(_NewFillingCube);

		m_Face_5_FriendType = EFillingCubeFriendTypes::OtherFillingCube;
	}

	if (m_Face_6_FriendType == EFillingCubeFriendTypes::Void)
	{
		AFillingCube* _NewFillingCube = GetWorld()->SpawnActor<AFillingCube>(GetActorLocation() + FVector(0.0f, 0.0f, -100.0f), FRotator(0));
		check(_NewFillingCube);
		m_FriendCubes.Add(_NewFillingCube);

		m_Face_6_FriendType = EFillingCubeFriendTypes::OtherFillingCube;
	}
}

void AFillingCube::UPDATE_CubeColor()
{
	if (m_ObstaclesCount == 0)
	{
		m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Green);
	}
	else if (m_ObstaclesCount == 1)
	{
		m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Blue);
	}
	else if (m_ObstaclesCount == 2)
	{
		m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Yellow);
	}
	else if (m_ObstaclesCount == 3)
	{
		m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Magenta);
	}
	else if (m_ObstaclesCount == 4)
	{
		m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Cyan);
	}
	else if (m_ObstaclesCount == 5)
	{
		m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Red);
	}
	else
	{
		m_SMC_FillingCube->SetMaterial(0, m_MI_SKTC_Black);
	}
}

TArray<AFillingCube*>& AFillingCube::GET_FriendCubesArray()
{
	return m_FriendCubes;
}

bool AFillingCube::CHECK_IsEveryFriendFound()
{
	if (m_Face_1_FriendType == EFillingCubeFriendTypes::Void)
	{
		return false;
	}

	if (m_Face_2_FriendType == EFillingCubeFriendTypes::Void)
	{
		return false;
	}

	if (m_Face_3_FriendType == EFillingCubeFriendTypes::Void)
	{
		return false;
	}

	if (m_Face_4_FriendType == EFillingCubeFriendTypes::Void)
	{
		return false;
	}

	if (m_Face_5_FriendType == EFillingCubeFriendTypes::Void)
	{
		return false;
	}

	if (m_Face_6_FriendType == EFillingCubeFriendTypes::Void)
	{
		return false;
	}

	return true;
}