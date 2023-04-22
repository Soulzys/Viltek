#include "Testing/TestingMerging.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "Components/DynamicMeshComponent.h"
#include "MeshDescription.h"
#include "StaticMeshDescription.h"
#include "StaticMeshAttributes.h"
#include "MeshConversionOptions.h"
#include "DynamicMeshToMeshDescription.h"
//#include "DynamicMesh/DynamicMesh3.h"
#include "DynamicMesh/MeshNormals.h"
#include "DynamicMesh/MeshTangents.h"
#include "DynamicMesh/MeshTransforms.h"
#include "DynamicMesh/DynamicVertexSkinWeightsAttribute.h"
#include "DynamicMesh/DynamicMeshAttributeSet.h"
#include "DynamicMesh/DynamicMeshOverlay.h"
#include "BoneWeights.h"
#include "SkeletalMeshAttributes.h"
#include "MeshDescriptionBuilder.h"

using namespace UE::Geometry;

ATestingMerging::ATestingMerging()
{
	PrimaryActorTick.bCanEverTick = false;

	//const ConstructorHelpers::FObjectFinder<UStaticMesh> OF_SM_RESOURCE_00(TEXT("StaticMesh'/Game/Geometry/Architecture/SM_Arch_BCube_00.SM_Arch_BCube_00'"));

	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_MI_SKTC_red(TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Red.MI_SKTC_Red'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_MI_SKTC_green(TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Green.MI_SKTC_Green'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInstance> OF_MI_SKTC_blue(TEXT("MaterialInstanceConstant'/Game/Materials/Sketching/MI_SKTC_Blue.MI_SKTC_Blue'"));

	//m_SM_test = OF_SM_RESOURCE_00.Object;

	m_MI_SKTC_red = OF_MI_SKTC_red.Object;
	m_MI_SKTC_green = OF_MI_SKTC_green.Object;
	m_MI_SKTC_blue = OF_MI_SKTC_blue.Object;

	m_SC_root = CreateDefaultSubobject<USceneComponent>(TEXT("SC_root"));
	SetRootComponent(m_SC_root);

	m_DMC_testing = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("DMC_testing"));
	m_DMC_testing->SetupAttachment(m_SC_root);

	m_SMC_test = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMC_test"));
	m_SMC_test->SetupAttachment(m_SC_root);
	//m_SMC_test->SetRelativeLocation(FVector(500.0f, 0.0f, 0.0f));
}

void ATestingMerging::BeginPlay()
{
	Super::BeginPlay();

	//TESTING_Fourth(); // Literally the only one that works as intented
	//TESTING_Seventh(); // The main testing one
	//TESTING_Tenth();
	TESTING_Eleventh();
}

void ATestingMerging::TESTING_Fourth()
{
	UStaticMeshDescription* _SMDescription = NewObject<UStaticMeshDescription>(UStaticMeshDescription::StaticClass());
	check(_SMDescription);

	FVector _offset = FVector(500.0f, 0.0, 0.0f);

	// VERTICES
	//
	_SMDescription->ReserveNewVertices(3);
	FVertexID _v00 = _SMDescription->CreateVertex();
	FVertexID _v01 = _SMDescription->CreateVertex();
	FVertexID _v02 = _SMDescription->CreateVertex();

	_SMDescription->SetVertexPosition(_v00, FVector(0.0f, 0.0f, 0.0f) + _offset);
	_SMDescription->SetVertexPosition(_v01, FVector(0.0f, 100.0f, 0.0f) + _offset);
	_SMDescription->SetVertexPosition(_v02, FVector(100.0f, 100.0f, 0.0f) + _offset);

	/*FVertexInstanceID _vi00 = _SMDescription->CreateVertexInstance(_v00);
	FVertexInstanceID _vi01 = _SMDescription->CreateVertexInstance(_v01);
	FVertexInstanceID _vi02 = _SMDescription->CreateVertexInstance(_v02);*/

	// EDGES
	//
	_SMDescription->ReserveNewEdges(3);
	FEdgeID _e00 = _SMDescription->CreateEdge(_v00, _v01);
	FEdgeID _e01 = _SMDescription->CreateEdge(_v00, _v02);
	FEdgeID _e02 = _SMDescription->CreateEdge(_v01, _v02);

	// POLYGROUPS
	//
	_SMDescription->ReserveNewPolygonGroups(3);
	FPolygonGroupID _pg00 = _SMDescription->CreatePolygonGroup();
	FPolygonGroupID _pg01 = _SMDescription->CreatePolygonGroup();
	FPolygonGroupID _pg02 = _SMDescription->CreatePolygonGroup();

	// TRIANGLES
	//
	TArray<FVertexInstanceID> _triVertexArray;
	TArray<FEdgeID> _triEdgeArray;
	_SMDescription->ReserveNewTriangles(1);

	// tri00
	_triVertexArray.Add(_v00);
	_triVertexArray.Add(_v01);
	_triVertexArray.Add(_v02);
	_triEdgeArray.Add(_e00);
	_triEdgeArray.Add(_e01);
	_triEdgeArray.Add(_e02);

	FTriangleID _tri00 = _SMDescription->CreateTriangle(_pg00, _triVertexArray, _triEdgeArray);
	_triVertexArray.Empty();
	_triEdgeArray.Empty();

	// UV
	//

	//
	//

	_SMDescription->RegisterAttributes();

	// TESTING
	//
	TArray<UStaticMeshDescription*> _toto;
	_toto.Add(_SMDescription);
	m_SM_test = NewObject<UStaticMesh>(UStaticMesh::StaticClass());
	check(m_SM_test);
	m_SM_test->BuildFromStaticMeshDescriptions(_toto);
	check(m_SM_test);
	m_SMC_test->SetStaticMesh(m_SM_test);

	// TEXTUAL DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Mesh vertex count : %d"), _SMDescription->GetVertexCount());
	UE_LOG(LogTemp, Warning, TEXT("Mesh edge count : %d"), _SMDescription->GetEdgeCount());
	UE_LOG(LogTemp, Warning, TEXT("Mesh triangle count : %d"), _SMDescription->GetTriangleCount());

	// VISUAL DEBUG
	//
	DrawDebugSphere(GetWorld(), FVector(0.0f, 0.0f, 0.0f) + _offset, 10.0f, 8, FColor::Green, true, 1, 2.0f);
	DrawDebugSphere(GetWorld(), FVector(0.0f, 100.0f, 0.0f) + _offset, 10.0f, 8, FColor::Blue, true, 1, 2.0f);
	DrawDebugSphere(GetWorld(), FVector(100.0f, 100.0f, 0.0f) + _offset, 10.0f, 8, FColor::Red, true, 1, 2.0f);	

	DrawDebugLine(GetWorld(), _SMDescription->GetVertexPosition(_v00), _SMDescription->GetVertexPosition(_v01), FColor::Cyan, true, 1, 10.0f);
	DrawDebugLine(GetWorld(), _SMDescription->GetVertexPosition(_v00), _SMDescription->GetVertexPosition(_v02), FColor::Cyan, true, 1, 10.0f);
	DrawDebugLine(GetWorld(), _SMDescription->GetVertexPosition(_v01), _SMDescription->GetVertexPosition(_v02), FColor::Cyan, true, 1, 10.0f);
}

void ATestingMerging::TESTING_Seventh()
{
	m_MeshDescription = new FMeshDescription();

	if (m_MeshDescription)
	{
		UE_LOG(LogTemp, Warning, TEXT("VALID"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}

	//m_MeshDescription->CreateVertex();

	FGeometryScriptPrimitiveOptions _primitiveOptions;
	FTransform _defaultTransform;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		m_DMC_testing->GetDynamicMesh(),
		_primitiveOptions,
		_defaultTransform,
		200.0f, 200.0f, 500.0f
	));

	FGeometryScriptCalculateNormalsOptions _normalOptions;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals
	(
		m_DMC_testing->GetDynamicMesh(), 
		_normalOptions
	));

	FGeometryScriptTangentsOptions _tangentOpts;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::ComputeTangents
	(
		m_DMC_testing->GetDynamicMesh(), 
		_tangentOpts
	));

	/*m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals
	(
		m_DMC_testing->GetDynamicMesh()
	));*/

	m_DMC_testing->SetMaterial(0, m_MI_SKTC_blue);

	FDynamicMesh3* _dynMesh = m_DMC_testing->GetDynamicMesh()->GetMeshPtr();
	check(_dynMesh);

	//_dynMesh->Normals

	check(m_MeshDescription);

	if (_dynMesh->HasAttributes())
	{
		UE_LOG(LogTemp, Warning, TEXT("HAS ATTRIBUTES"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DOESN'T HAVE ATTRIBUTES"));
	}


	
	FConversionToMeshDescriptionOptions _conversionPts;
	_conversionPts.bSetPolyGroups = true;
	_conversionPts.bUpdatePositions = true;
	_conversionPts.bUpdateNormals = true;
	_conversionPts.bUpdateTangents = false;
	_conversionPts.bUpdateUVs = false;
	_conversionPts.bUpdateVtxColors = false;

	check(m_MeshDescription);

	//FDynamicMeshToMeshDescription _convertor(_conversionPts);

	// Debugging
	UE_LOG(LogTemp, Warning, TEXT("FMeshDescription Vertex count : %d"), m_MeshDescription->Vertices().Num());
	UE_LOG(LogTemp, Warning, TEXT("FMeshDescription UVs count : %d"), m_MeshDescription->UVs(0).Num());
	UE_LOG(LogTemp, Warning, TEXT("FMeshDescription Edge count : %d"), m_MeshDescription->Edges().Num());
	UE_LOG(LogTemp, Warning, TEXT("FMeshDescription Triangle count : %d"), m_MeshDescription->Triangles().Num());
	UE_LOG(LogTemp, Warning, TEXT("FMeshDescription Polygon count : %d"), m_MeshDescription->Polygons().Num());
	UE_LOG(LogTemp, Warning, TEXT("FMeshDescription Polygon group count : %d"), m_MeshDescription->PolygonGroups().Num());

	UStaticMeshDescription* _SMDescription = NewObject<UStaticMeshDescription>(UStaticMeshDescription::StaticClass());
	check(_SMDescription);


	_SMDescription->SetMeshDescription(*m_MeshDescription);
	_SMDescription->RegisterAttributes();

	UE_LOG(LogTemp, Warning, TEXT("FStaticMeshDescription Vertex count : %d"), _SMDescription->Vertices().Num());
	//UE_LOG(LogTemp, Warning, TEXT("FStaticMeshDescription UVs count : %d"), _SMDescription->UVs(0).Num());
	UE_LOG(LogTemp, Warning, TEXT("FStaticMeshDescription Edge count : %d"), _SMDescription->Edges().Num());
	UE_LOG(LogTemp, Warning, TEXT("FStaticMeshDescription Triangle count : %d"), _SMDescription->Triangles().Num());
	UE_LOG(LogTemp, Warning, TEXT("FStaticMeshDescription Polygon count : %d"), _SMDescription->Polygons().Num());
	UE_LOG(LogTemp, Warning, TEXT("FStaticMeshDescription Polygon group count : %d"), _SMDescription->PolygonGroups().Num());

	//m_MeshDescription_TEST = new FMeshDescription();
	//check(m_MeshDescription_TEST);

	
	

	//

	TArray<UStaticMeshDescription*> _toto;
	_toto.Add(_SMDescription);

	/*m_SM_test = NewObject<UStaticMesh>(UStaticMesh::StaticClass());
	check(m_SM_test);*/

	m_SM_test->BuildFromStaticMeshDescriptions(_toto);
	check(m_SM_test);
	//m_SM_test->Str

	m_SMC_test->SetStaticMesh(m_SM_test);
	m_SMC_test->bIgnoreStreamingManagerUpdate = false;
	m_SMC_test->SetMaterial(0, m_MI_SKTC_red);

	//m_SMC_test->GetStaticMesh()->GetMeshDescription()->Recal

	//FStaticMeshLODResources _LOD;

	//m_SM_test->BuildFromMeshDescription(*m_MeshDescription, _LOD);

	check(m_SM_test);
	
	//m_SMC_test->SetStaticMesh(m_SM_test);

	/*
	// StaticMeshDescription creation
	//
	UStaticMeshDescription* _SMDescription = NewObject<UStaticMeshDescription>(UStaticMeshDescription::StaticClass());
	check(_SMDescription);

	// VERTICES
	//
	UE::Geometry::TDynamicVector<FVector3d> _vertices_array = _dynMesh->GetVerticesBuffer();
	int32 _vertexCount = _dynMesh->VertexCount();
	TArray<FVertexID> _vertexID_array;
	_SMDescription->ReserveNewVertices(_vertexCount);

	for (int32 i = 0; i < _vertexCount; i++)
	{
		_vertexID_array.Add(_SMDescription->CreateVertex());
	}

	for (int32 i = 0; i < _vertexCount; i++)
	{
		_SMDescription->SetVertexPosition(_vertexID_array[i], _vertices_array[i]);
	}

	// EDGES
	//
	UE::Geometry::TDynamicVector<UE::Geometry::FDynamicMesh3::FEdge> _edges_array = _dynMesh->GetEdgesBuffer();
	int32 _edgesCount = _dynMesh->EdgeCount();
	TArray<FEdgeID> _edgeID_array;
	_SMDescription->ReserveNewEdges(_edgesCount);

	//_vertices_array[0].Verte

	_dynMesh->GetTrianglesBuffer();

	for (int32 i = 0; i < _dynMesh->VertexCount(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vertex[%d] : %s"), i, *_vertices_array[i].ToString());
	}

	UE_LOG(LogTemp, Warning, TEXT("Finished !"));*/
}

void ATestingMerging::TESTING_Tenth()
{
	FGeometryScriptPrimitiveOptions _primitiveOptions;
	FTransform _defaultTransform;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		m_DMC_testing->GetDynamicMesh(),
		_primitiveOptions,
		_defaultTransform,
		300.0f, 100.0f, 100.0f
	));

	FGeometryScriptCalculateNormalsOptions _calculateNormalOpts;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals
	(
		m_DMC_testing->GetDynamicMesh(), 
		_calculateNormalOpts
	));

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals
	(
		m_DMC_testing->GetDynamicMesh()
	));

	FDynamicMesh3* _DynMesh = m_DMC_testing->GetDynamicMesh()->GetMeshPtr();
	check(_DynMesh);

	m_DMC_testing->ConditionalBeginDestroy();
	m_DMC_testing = nullptr;

	check(_DynMesh);

	FMeshDescription _MeshDesc;
	FStaticMeshAttributes _Attributes(_MeshDesc);
	_Attributes.Register();

	FMeshDescriptionBuilder _MeshDescBuilder;
	_MeshDescBuilder.SetMeshDescription(&_MeshDesc);
	_MeshDescBuilder.EnablePolyGroups();
	_MeshDescBuilder.SetNumUVLayers(1);

	TArray<FVertexID> _VertexIDs;
	_VertexIDs.SetNum(8);

	UE_LOG(LogTemp, Warning, TEXT("Vertex count : %d"), _DynMesh->VertexCount());

	_VertexIDs[0] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(0));
	_VertexIDs[1] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(1));
	_VertexIDs[2] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(2));
	_VertexIDs[3] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(3));
	_VertexIDs[4] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(4));
	_VertexIDs[5] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(5));
	_VertexIDs[6] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(6));
	_VertexIDs[7] = _MeshDescBuilder.AppendVertex(_DynMesh->GetVertexRef(7));

	/*DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(0), FVector(10.0f), FColor::Red, true, 1, 2.0f);
	DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(1), FVector(10.0f), FColor::Green, true, 1, 2.0f);
	DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(2), FVector(10.0f), FColor::Magenta, true, 1, 2.0f);
	DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(3), FVector(10.0f), FColor::Orange, true, 1, 2.0f);
	DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(4), FVector(10.0f), FColor::Black, true, 1, 2.0f);
	DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(5), FVector(10.0f), FColor::Blue, true, 1, 2.0f);
	DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(6), FVector(10.0f), FColor::Cyan, true, 1, 2.0f);
	DrawDebugBox(GetWorld(), _DynMesh->GetVertexRef(7), FVector(10.0f), FColor::White, true, 1, 2.0f);*/

	//
	//

	// Face 1
	TArray<FVertexInstanceID> _VertexInsts;
	// 0
	FVertexInstanceID _instance = _MeshDescBuilder.AppendInstance(_VertexIDs[0]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(0));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(0));
	_VertexInsts.Add(_instance);

	// 1
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[1]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(1));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(1));
	_VertexInsts.Add(_instance);

	// 2
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[2]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(2));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(2));
	_VertexInsts.Add(_instance);

	// 3
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[3]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(3));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(3));
	_VertexInsts.Add(_instance);

	//

	// Face 2
	// 4
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[0]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(0));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(0));
	_VertexInsts.Add(_instance);

	// 5
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[4]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(4));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(4));
	_VertexInsts.Add(_instance);

	// 6
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[5]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(5));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(5));
	_VertexInsts.Add(_instance);

	// 7
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[1]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(1));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(1));
	_VertexInsts.Add(_instance);

	//

	// Face 3
	// 8
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[1]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(1));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(1));
	_VertexInsts.Add(_instance);

	//9
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[5]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(5));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(5));
	_VertexInsts.Add(_instance);

	// 10
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[6]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(6));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(6));
	_VertexInsts.Add(_instance);

	// 11
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[2]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(2));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(2));
	_VertexInsts.Add(_instance);

	//

	// Face 4
	// 12
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[2]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(2));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(2));
	_VertexInsts.Add(_instance);

	// 13
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[6]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(6));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(6));
	_VertexInsts.Add(_instance);

	// 14
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[7]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(7));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(7));
	_VertexInsts.Add(_instance);

	// 15
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[3]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(3));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(3));
	_VertexInsts.Add(_instance);

	//

	// Face 5
	// 16
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[3]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(3));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(3));
	_VertexInsts.Add(_instance);

	// 17
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[7]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(7));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(7));
	_VertexInsts.Add(_instance);

	// 18
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[4]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(4));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(4));
	_VertexInsts.Add(_instance);

	// 19
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[0]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(0));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(0));
	_VertexInsts.Add(_instance);

	//

	// Face 5
	// 20
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[4]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(4));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(4));
	_VertexInsts.Add(_instance);

	// 21
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[5]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(5));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(5));
	_VertexInsts.Add(_instance);

	// 22
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[6]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(6));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(6));
	_VertexInsts.Add(_instance);

	// 23
	_instance = _MeshDescBuilder.AppendInstance(_VertexIDs[7]);
	_MeshDescBuilder.SetInstanceNormal(_instance, (FVector)_DynMesh->GetVertexNormal(7));
	_MeshDescBuilder.SetInstanceUV(_instance, (FVector2D)_DynMesh->GetVertexUV(7));
	_VertexInsts.Add(_instance);


	// Alocate PolygonGroup
	FPolygonGroupID _polygonGroup = _MeshDescBuilder.AppendPolygonGroup();

	// Triangles
	//
	// Face 1
	_MeshDescBuilder.AppendTriangle(_VertexInsts[0], _VertexInsts[1], _VertexInsts[2], _polygonGroup);
	_MeshDescBuilder.AppendTriangle(_VertexInsts[0], _VertexInsts[2], _VertexInsts[3], _polygonGroup);

	// Face 2
	_MeshDescBuilder.AppendTriangle(_VertexInsts[4], _VertexInsts[5], _VertexInsts[6], _polygonGroup);
	_MeshDescBuilder.AppendTriangle(_VertexInsts[4], _VertexInsts[6], _VertexInsts[7], _polygonGroup);

	// Face 3
	_MeshDescBuilder.AppendTriangle(_VertexInsts[8], _VertexInsts[9], _VertexInsts[10], _polygonGroup);
	_MeshDescBuilder.AppendTriangle(_VertexInsts[8], _VertexInsts[10], _VertexInsts[11], _polygonGroup);

	// Face 4
	_MeshDescBuilder.AppendTriangle(_VertexInsts[12], _VertexInsts[13], _VertexInsts[14], _polygonGroup);
	_MeshDescBuilder.AppendTriangle(_VertexInsts[12], _VertexInsts[14], _VertexInsts[15], _polygonGroup);

	// Face 5
	_MeshDescBuilder.AppendTriangle(_VertexInsts[16], _VertexInsts[17], _VertexInsts[18], _polygonGroup);
	_MeshDescBuilder.AppendTriangle(_VertexInsts[16], _VertexInsts[18], _VertexInsts[19], _polygonGroup);

	// Face 6
	_MeshDescBuilder.AppendTriangle(_VertexInsts[22], _VertexInsts[21], _VertexInsts[20], _polygonGroup);
	_MeshDescBuilder.AppendTriangle(_VertexInsts[23], _VertexInsts[22], _VertexInsts[20], _polygonGroup);


	UStaticMesh* _SMesh = NewObject<UStaticMesh>(this);
	_SMesh->GetStaticMaterials().Add(FStaticMaterial());

	UStaticMesh::FBuildMeshDescriptionsParams _MeshDescParams;
	_MeshDescParams.bBuildSimpleCollision = true;
	_MeshDescParams.bFastBuild = true;

	// Building
	TArray<const FMeshDescription*> _MeshDescPtrs;
	_MeshDescPtrs.Emplace(&_MeshDesc);
	_SMesh->BuildFromMeshDescriptions(_MeshDescPtrs, _MeshDescParams);

	m_SMC_test->SetStaticMesh(_SMesh);
	m_SMC_test->SetMaterial(0, m_MI_SKTC_red);

	_SMesh->ConditionalBeginDestroy();
	_SMesh = nullptr;


	/*FIndex3i NormalTri = _NormalOverlay->GetTriangle(TriID);
	for (int32 j = 0; j < 3; ++j)
	{
		FVertexInstanceID CornerInstanceID = TriVertInstances[j];
		FVector TriVertNormal = (_NormalOverlay->IsElement(NormalTri[j])) ? (FVector)_NormalOverlay->GetElement(NormalTri[j]) : FVector::UpVector;
		_Builder.SetInstanceNormal(CornerInstanceID, TriVertNormal);
	}*/

}

void ATestingMerging::TESTING_Eleventh()
{
	FGeometryScriptPrimitiveOptions _primitiveOptions;
	FTransform _defaultTransform;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		m_DMC_testing->GetDynamicMesh(),
		_primitiveOptions,
		_defaultTransform,
		200.0f, 200.0f, 200.0f
	));

	UDynamicMesh* _Cutout = NewObject<UDynamicMesh>(this);
	check(_Cutout);

	_Cutout = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox
	(
		_Cutout, 
		_primitiveOptions, 
		_defaultTransform, 
		200.0f, 200.0f, 200.0f
	);

	FTransform _booleanTransform;
	_booleanTransform.SetLocation(FVector(100.0f, 100.0f, 100.0f));

	FGeometryScriptMeshBooleanOptions _booleanOptions;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean
	(
		m_DMC_testing->GetDynamicMesh(), 
		_defaultTransform, 
		_Cutout, 
		_booleanTransform, 
		EGeometryScriptBooleanOperation::Union, 
		_booleanOptions
	));

	_Cutout->ConditionalBeginDestroy();
	_Cutout = nullptr;


	FGeometryScriptCalculateNormalsOptions _calculateNormalOpts;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals
	(
		m_DMC_testing->GetDynamicMesh(),
		_calculateNormalOpts
	));

	FGeometryScriptTangentsOptions _tangentOpts;

	m_DMC_testing->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::ComputeTangents
	(
		m_DMC_testing->GetDynamicMesh(),
		_tangentOpts
	));

	FDynamicMesh3* _DynMesh = m_DMC_testing->GetDynamicMesh()->GetMeshPtr();
	check(_DynMesh);

	m_DMC_testing->ConditionalBeginDestroy();
	m_DMC_testing = nullptr;

	check(_DynMesh);

	FMeshDescription _MeshDesc;
	FStaticMeshAttributes _Attributes(_MeshDesc);
	_Attributes.Register();

	FMeshDescriptionBuilder _MeshDescBuilder;
	_MeshDescBuilder.SetMeshDescription(&_MeshDesc);
	_MeshDescBuilder.EnablePolyGroups();
	_MeshDescBuilder.SetNumUVLayers(1);

	FPolygonGroupID _polygonGroup = _MeshDescBuilder.AppendPolygonGroup();




	const int32 _NumUVLayers = _DynMesh->HasAttributes() ? _DynMesh->Attributes()->NumUVLayers() : 0; // _NumUVLayers = 1


	const FDynamicMeshNormalOverlay* _NormalOverlay = _DynMesh->HasAttributes() ? _DynMesh->Attributes()->PrimaryNormals() : nullptr;
	const FDynamicMeshNormalOverlay* _TangentOverlay = _DynMesh->HasAttributes() ? _DynMesh->Attributes()->PrimaryTangents() : nullptr;
	const FDynamicMeshNormalOverlay* _BiTangentOverlay = _DynMesh->HasAttributes() ? _DynMesh->Attributes()->PrimaryBiTangents() : nullptr;



	// create "vertex buffer" in MeshDescription
	TArray<FVertexID> _MapV;
	_MapV.SetNum(_DynMesh->MaxVertexID());
	for (int _VertID : _DynMesh->VertexIndicesItr())
	{
		_MapV[_VertID] = _MeshDescBuilder.AppendVertex((FVector)_DynMesh->GetVertex(_VertID));
	}


	TArray<const FDynamicMeshUVOverlay*> _UVLayers;
	for (int32 k = 0; k < _NumUVLayers; ++k)
	{
		_UVLayers.Add(_DynMesh->Attributes()->GetUVLayer(k));
	}


	TArray<TArray<FUVID>> _MapUVArray;
	_MapUVArray.SetNum(_NumUVLayers);
	TArray<FUVID> _UnsetUVIDs;
	_UnsetUVIDs.Init(INDEX_NONE, _NumUVLayers); // UVIDs for unset UVs to share, if needed
	for (int32 k = 0; k < _NumUVLayers; ++k)
	{
		const FDynamicMeshUVOverlay* _UVOverlay = _UVLayers[k];
		TArray<FUVID>& _MapUV = _MapUVArray[k];

		_MapUV.SetNum(_UVOverlay->MaxElementID());
		_MeshDescBuilder.ReserveNewUVs(_UVOverlay->ElementCount(), k);

		for (int32 _ElementID : _UVOverlay->ElementIndicesItr()) // Loops 24 times
		{
			const FVector2D _UVvalue = (FVector2D)_UVOverlay->GetElement(_ElementID);
			_MapUV[_ElementID] = _MeshDescBuilder.AppendUV(_UVvalue, k);
		}
	}

	TArray<FTriangleID> IndexToTriangleIDMap;
	IndexToTriangleIDMap.SetNum(_DynMesh->MaxTriangleID());

	for (int TriID : _DynMesh->TriangleIndicesItr())
	{
		FIndex3i Triangle = _DynMesh->GetTriangle(TriID);

		// create new vtx instances for each triangle
		FVertexInstanceID TriVertInstances[3];
		for (int32 j = 0; j < 3; ++j)
		{
			const FVertexID TriVertex = _MapV[Triangle[j]];
			TriVertInstances[j] = _MeshDescBuilder.AppendInstance(TriVertex);
		}

		// transfer material index to MeshDescription polygon group (by convention)
		FPolygonGroupID UsePolygonGroupID = _polygonGroup;

		// add the triangle to MeshDescription
		FTriangleID NewTriangleID = _MeshDescBuilder.AppendTriangle(TriVertInstances[0], TriVertInstances[1], TriVertInstances[2], UsePolygonGroupID);
		IndexToTriangleIDMap[TriID] = NewTriangleID;

		// transfer UVs.  Note the Builder sets both the shared and per-instance UVs from this
		for (int32 k = 0; k < _NumUVLayers; ++k)
		{
			FUVID UVIDs[3] = { FUVID(-1), FUVID(-1), FUVID(-1) };

			// add zero uvs for unset triangles.
			if (!_UVLayers[k]->IsSetTriangle(TriID))
			{
				if (_UnsetUVIDs[k].GetValue() == INDEX_NONE)
				{
					_UnsetUVIDs[k] = _MeshDescBuilder.AppendUV(FVector2D::ZeroVector, k);
				}
				for (int32 j = 0; j < 3; ++j)
				{
					UVIDs[j] = _UnsetUVIDs[k];
				}
			}
			else
			{
				const TArray<FUVID>& MapUV = _MapUVArray[k];

				// triangle of UV element ids from dynamic mesh.  references values already stored in MeshDescription.
				FIndex3i UVTri = _UVLayers[k]->GetTriangle(TriID);

				// translate to MeshDescription Ids
				for (int32 j = 0; j < 3; ++j)
				{
					UVIDs[j] = MapUV[UVTri[j]];
				}
			}

			// append the UV triangle - builder takes care of the rest
			_MeshDescBuilder.AppendUVTriangle(NewTriangleID, UVIDs[0], UVIDs[1], UVIDs[2], k);
		}

		// transfer tangent space. 
		// NB: MeshDescription doesn't store and explicit bitangent, so this conversion isn't perfect.
		// NB: only per-instance normals , tangents, bitangent sign are supported in MeshDescription at this time
		// NB: will need to be updated to follow the pattern used in UVs above when MeshDescription supports shared tangent space elements. 
	
		if (_NormalOverlay != nullptr)
		{
			if (_TangentOverlay != nullptr && _BiTangentOverlay != nullptr)
			{
				FIndex3i NormalTri = _NormalOverlay->GetTriangle(TriID);
				FIndex3i TangentTri = _TangentOverlay->GetTriangle(TriID);
				FIndex3i BiTangentTri = _BiTangentOverlay->GetTriangle(TriID);

				for (int32 j = 0; j < 3; ++j)
				{
					const FVertexInstanceID CornerInstanceID = TriVertInstances[j];

					const FVector3f TriVertNormal = (_NormalOverlay->IsElement(NormalTri[j])) ? _NormalOverlay->GetElement(NormalTri[j]) : FVector3f(FVector::UpVector);
					const FVector3f TriVertTangent = (_TangentOverlay->IsElement(TangentTri[j])) ? _TangentOverlay->GetElement(TangentTri[j]) : FVector3f(FVector::ForwardVector);
					const FVector3f TriVertBiTangent = (_BiTangentOverlay->IsElement(BiTangentTri[j])) ? _BiTangentOverlay->GetElement(BiTangentTri[j]) : FVector3f(FVector::RightVector);

					// infer sign
					float BiTangentSign = VectorUtil::BitangentSign(TriVertNormal, TriVertTangent, TriVertBiTangent);

					// set the tangent space
					_MeshDescBuilder.SetInstanceTangentSpace(CornerInstanceID, (FVector)TriVertNormal, (FVector)TriVertTangent, BiTangentSign);
				}
			}
			else
			{
				FIndex3i NormalTri = _NormalOverlay->GetTriangle(TriID);
				for (int32 j = 0; j < 3; ++j)
				{
					FVertexInstanceID CornerInstanceID = TriVertInstances[j];
					FVector TriVertNormal = (_NormalOverlay->IsElement(NormalTri[j])) ? (FVector)_NormalOverlay->GetElement(NormalTri[j]) : FVector::UpVector;
					_MeshDescBuilder.SetInstanceNormal(CornerInstanceID, TriVertNormal);
				}
			}
		}
	}

	UStaticMesh* _SMesh = NewObject<UStaticMesh>(this);
	_SMesh->GetStaticMaterials().Add(FStaticMaterial());

	UStaticMesh::FBuildMeshDescriptionsParams _MeshDescParams;
	_MeshDescParams.bBuildSimpleCollision = true;
	_MeshDescParams.bFastBuild = true;

	// Building
	TArray<const FMeshDescription*> _MeshDescPtrs;
	_MeshDescPtrs.Emplace(&_MeshDesc);
	_SMesh->BuildFromMeshDescriptions(_MeshDescPtrs, _MeshDescParams);

	m_SMC_test->SetStaticMesh(_SMesh);
	m_SMC_test->SetMaterial(0, m_MI_SKTC_red);
	m_SMC_test->SetMaterial(0, m_MI_SKTC_blue);
	//m_SMC_test->bDrawMeshCollisionIfComplex = true;
	m_SMC_test->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);

	UE_LOG(LogTemp, Warning, TEXT("Coucou !"));

	_SMesh->ConditionalBeginDestroy();
	_SMesh = nullptr;
}