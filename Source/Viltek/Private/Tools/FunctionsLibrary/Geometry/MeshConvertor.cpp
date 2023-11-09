#include "Tools/FunctionsLibrary/Geometry/MeshConvertor.h"
#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "MeshDescription.h"
#include "StaticMeshDescription.h"
#include "StaticMeshAttributes.h"
#include "MeshDescriptionBuilder.h"
//#include "DynamicMesh/DynamicMeshOverlay.h"

DEFINE_LOG_CATEGORY(LogClass_MeshConvertor);

using namespace UE::Geometry;

UStaticMesh* UMeshConvertor::CREATE_StaticMesh(UDynamicMeshComponent* p_DynMeshComp, bool p_bRecomputeNormals, bool p_bComputeTangents)
{
	// Safety check
	if (p_DynMeshComp == nullptr)
	{
		UE_LOG(LogClass_MeshConvertor, Error, TEXT("FUNCTION ''CREATE_StaticMesh'' ==>> DynamicMesh argument is NULL !"));
		return nullptr;
	}

	if (p_bRecomputeNormals == true)
	{
		FGeometryScriptCalculateNormalsOptions _calculateNormalOpts;
		p_DynMeshComp->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(p_DynMeshComp->GetDynamicMesh(), _calculateNormalOpts));
	}

	if (p_bComputeTangents == true)
	{
		FGeometryScriptTangentsOptions _tangentsOpts;
		p_DynMeshComp->SetDynamicMesh(UGeometryScriptLibrary_MeshNormalsFunctions::ComputeTangents(p_DynMeshComp->GetDynamicMesh(), _tangentsOpts));
	}

	// Retrieve FDynamicMesh3, which is what really contains all the data we need. UDynamicMesh is only a wrapper around it.
	FDynamicMesh3* _DynMesh3 = p_DynMeshComp->GetDynamicMesh()->GetMeshPtr();

	// Satefy check
	if (_DynMesh3 == nullptr)
	{
		UE_LOG(LogClass_MeshConvertor, Error, TEXT("FUNCTION ''CREATE_StaticMesh'' ==>> Could not retrieve FDynamicMesh3 from UDynamicMesh"));
		return nullptr;
	}

	FMeshDescription      _MeshDesc              ;
	FStaticMeshAttributes _Attributes(_MeshDesc) ;
	_Attributes.Register()                       ; 

	FMeshDescriptionBuilder _MeshDescBuilder        ;
	_MeshDescBuilder.SetMeshDescription(&_MeshDesc) ;
	_MeshDescBuilder.EnablePolyGroups  ()           ;
	_MeshDescBuilder.SetNumUVLayers    (1)          ; // Need to make this dynamic by putting the argument into a variable instead

	FPolygonGroupID _PolygonGroupID = _MeshDescBuilder.AppendPolygonGroup();
	bool _bHasAttributes                                = _DynMesh3->HasAttributes()                                               ; // Should always be true
	const int32 _NumUVLayers                            = _bHasAttributes ? _DynMesh3->Attributes()->NumUVLayers()       : 0       ; // Should be 1
	const FDynamicMeshNormalOverlay* _NormalsOverlay    = _bHasAttributes ? _DynMesh3->Attributes()->PrimaryNormals()    : nullptr ;
	const FDynamicMeshNormalOverlay* _TangentsOverlay   = _bHasAttributes ? _DynMesh3->Attributes()->PrimaryTangents()   : nullptr ;
	const FDynamicMeshNormalOverlay* _BiTangentsOverlay = _bHasAttributes ? _DynMesh3->Attributes()->PrimaryBiTangents() : nullptr ;

	//
	// ***** Vertices Section ***** //
	//

	// Create a Vertex Buffer in FMeshDescription
	TArray<FVertexID> _VertexBuffer;
	_VertexBuffer.SetNum(_DynMesh3->MaxVertexID());
	for (int _VertID : _DynMesh3->VertexIndicesItr())
	{
		_VertexBuffer[_VertID] = _MeshDescBuilder.AppendVertex((FVector)_DynMesh3->GetVertex(_VertID));
	}

	//
	// ***** UVs Section ***** //
	//

	// Should run only once since _NumUVLayers = 1
	TArray<const FDynamicMeshUVOverlay*> _UVLayers;
	for (int32 i = 0; i < _NumUVLayers; ++i)
	{
		_UVLayers.Add(_DynMesh3->Attributes()->GetUVLayer(i));
	}

	// This whole part, especially the first loop, is made so the function can handle UDynamicMesh made of multiple UV Maps. 
	// However in our case, we should always get UDynamicMesh with only 1 UV Maps. 
	// Consequently, should this part be rewritten ? Or kept as it is for sake of security ? 
	TArray<TArray<FUVID>> _MapUVArray;
	_MapUVArray.SetNum(_NumUVLayers);
	TArray<FUVID> _UnsetUVIDs;
	_UnsetUVIDs.Init(INDEX_NONE, _NumUVLayers);
	for (int32 i = 0; i < _NumUVLayers; ++i)
	{
		const FDynamicMeshUVOverlay* _UVOverlay = _UVLayers[i];
		TArray<FUVID>& _MapUV = _MapUVArray[i];

		_MapUV.SetNum(_UVOverlay->MaxElementID());
		_MeshDescBuilder.ReserveNewUVs(_UVOverlay->ElementCount(), i);

		for (int32 _ElementID : _UVOverlay->ElementIndicesItr())
		{
			const FVector2D _UVValue = (FVector2D)_UVOverlay->GetElement(_ElementID);
			_MapUV[_ElementID] = _MeshDescBuilder.AppendUV(_UVValue, i);
		}
	}

	//
	// ***** Triangles Section (along with normals and tangents) ***** //
	//

	TArray<FTriangleID> _IndexToTriangleIDMap;
	_IndexToTriangleIDMap.SetNum(_DynMesh3->MaxTriangleID());
	for (int _TriID : _DynMesh3->TriangleIndicesItr())
	{
		FIndex3i _Triangle = _DynMesh3->GetTriangle(_TriID);

		// Create new vertex instances for each triangle
		FVertexInstanceID _TriVertInstances[3];
		for (int32 i = 0; i < 3; ++i)
		{
			const FVertexID _TriVertex = _VertexBuffer[_Triangle[i]];
			_TriVertInstances[i] = _MeshDescBuilder.AppendInstance(_TriVertex);
		}

		// Transfer material index to MeshDescription polygon group (by convention)
		FPolygonGroupID _UsePolygonGroupID = _PolygonGroupID; 

		// Add the triangles to FMeshDescription
		FTriangleID _NewTriangleID = _MeshDescBuilder.AppendTriangle(_TriVertInstances[0], _TriVertInstances[1], _TriVertInstances[2], _UsePolygonGroupID);
		_IndexToTriangleIDMap[_TriID] = _NewTriangleID;

		// Transfer UVs.  Note the Builder sets both the shared and per-instance UVs from this.
		// Again, this should only run once since we have only one UV Layer.
		for (int32 i = 0; i < _NumUVLayers; ++i)
		{
			FUVID _UVIDs[3] = { FUVID(-1), FUVID(-1), FUVID(-1) };

			// Add zero UVs for unset triangles.
			if (_UVLayers[i]->IsSetTriangle(_TriID) == false)
			{
				if (_UnsetUVIDs[i].GetValue() == INDEX_NONE)
				{
					_UnsetUVIDs[i] = _MeshDescBuilder.AppendUV(FVector2D::ZeroVector, i);
				}

				for (int8 j = 0; j < 3; ++j)
				{
					_UVIDs[j] = _UnsetUVIDs[i];
				}
			}
			else
			{
				const TArray<FUVID>& _MapUV = _MapUVArray[i];

				// Triangle of UV element ids from dynamic mesh. References values already stored in MeshDescription.
				FIndex3i _UVTri = _UVLayers[i]->GetTriangle(_TriID);

				// Translate to MeshDescription IDs
				for (int8 j = 0; j < 3; ++j)
				{
					_UVIDs[j] = _MapUV[_UVTri[j]];
				}
			}

			// Append the UV triangle - the builder takes care of the rest.
			_MeshDescBuilder.AppendUVTriangle(_NewTriangleID, _UVIDs[0], _UVIDs[1], _UVIDs[2], i);
		}

		// Try to transfer tangent space. If FDynamicMesh3 has no tangents set, then tranfer only its normals
		// NB: MeshDescription doesn't store and explicit bitangent, so this conversion isn't perfect.
		// NB: only per-instance normals , tangents, bitangent sign are supported in MeshDescription at this time
		// NB: will need to be updated to follow the pattern used in UVs above when MeshDescription supports shared tangent space elements. 

		if (_NormalsOverlay != nullptr)
		{
			if (_TangentsOverlay != nullptr && _BiTangentsOverlay != nullptr)
			{
				FIndex3i _NormalTri     = _NormalsOverlay   ->GetTriangle(_TriID) ;
				FIndex3i _TangentsTri   = _TangentsOverlay  ->GetTriangle(_TriID) ;
				FIndex3i _BiTangentsTri = _BiTangentsOverlay->GetTriangle(_TriID) ;

				for (int8 i = 0; i < 3; ++i)
				{
					const FVertexInstanceID _CornerInstanceID = _TriVertInstances[i];

					const FVector3f _TriVertNormals   = (_NormalsOverlay   ->IsElement(_NormalTri    [i])) ? _NormalsOverlay   ->GetElement(_NormalTri    [i]) : FVector3f(FVector::UpVector     ) ;
					const FVector3f _TriVertTangents  = (_TangentsOverlay  ->IsElement(_TangentsTri  [i])) ? _TangentsOverlay  ->GetElement(_TangentsTri  [i]) : FVector3f(FVector::ForwardVector) ;
					const FVector3f _TriVertBiTangents = (_BiTangentsOverlay->IsElement(_BiTangentsTri[i])) ? _BiTangentsOverlay->GetElement(_BiTangentsTri[i]) : FVector3f(FVector::RightVector  ) ;

					// Infer sign
					float _BiTangentSign = VectorUtil::BitangentSign(_TriVertNormals, _TriVertTangents, _TriVertBiTangents);

					// Set the tangent space
					_MeshDescBuilder.SetInstanceTangentSpace(_CornerInstanceID, (FVector)_TriVertNormals, (FVector)_TriVertTangents, _BiTangentSign);
				}
			}
			else // Only Normals set on FDynamicMesh3
			{
				FIndex3i _NormalTri = _NormalsOverlay->GetTriangle(_TriID);
				for (int8 i = 0; i < 3; ++i)
				{
					FVertexInstanceID _CornerInstanceID = _TriVertInstances[i];
					FVector _TriVertNormals = (_NormalsOverlay->IsElement(_NormalTri[i])) ? (FVector)_NormalsOverlay->GetElement(_NormalTri[i]) : FVector::UpVector;
					_MeshDescBuilder.SetInstanceNormal(_CornerInstanceID, _TriVertNormals);
				}
			}
		}
	}

	UStaticMesh* _StaticMesh = NewObject<UStaticMesh>();

	// Safety check
	if (_StaticMesh == nullptr)
	{
		UE_LOG(LogClass_MeshConvertor, Error, TEXT("FUNCTION ''CREATE_StaticMesh'' ==>> Could not create new UStaticMesh !"));
		return nullptr;
	}

	_StaticMesh->GetStaticMaterials().Add(FStaticMaterial());

	// This should be set as varialbes. Perhaps through the function argument as FBUildMeshDescriptionParams ? 
	UStaticMesh::FBuildMeshDescriptionsParams _MeshDescParams;
	_MeshDescParams.bBuildSimpleCollision = true;
	_MeshDescParams.bFastBuild = true;

	// Actual building step
	TArray<const FMeshDescription*> _MeshDescArray;
	_MeshDescArray.Emplace(&_MeshDesc);
	_StaticMesh->BuildFromMeshDescriptions(_MeshDescArray, _MeshDescParams);

	// Destroys UDynamicMeshComponent passed as argument
	// Should this be done here ? Hmmmm... not sure
	/*p_DynMeshComp->UnregisterComponent();
	p_DynMeshComp->ConditionalBeginDestroy();
	p_DynMeshComp = nullptr;*/

	if (_StaticMesh == nullptr)
	{
		UE_LOG(LogClass_MeshConvertor, Error, TEXT("FUNCTION ''CREATE_StaticMesh'' ==>> In the end, we could not build a UStaticMesh"));
		return nullptr;
	}

	return _StaticMesh;
}