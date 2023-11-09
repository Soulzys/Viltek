#pragma once

#include "CoreMinimal.h"
#include "Architecture/ArchBase.h"
#include "ArchBuilderBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogStruct_RoomCorner         , Log, All) ;
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_SplitCorner        , Log, All) ;
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_RoomDynamicWall    , Log, All) ;
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_RoomStaticWall     , Log, All) ;
DECLARE_LOG_CATEGORY_EXTERN(LogStruct_RoomFillingEntity  , Log, All) ;
DECLARE_LOG_CATEGORY_EXTERN(LogClass_ArchBuilderBase     , Log, All) ;

class UDynamicMeshComponent;
class APlayerStart;
class UBuilderManager;
class AFillingCube;
class UMaterialInstance;
class ULevelSpaceManager;
class AVolumeShell;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ENUMS
//
UENUM()
enum class ESplitType : uint8
{
	InsideSplit  = 0,
	OutsideSplit = 1,
	None         = 2
};

//
//

UENUM()
enum class EWallNature : uint8
{
	InsideWall    = 0 , 
	OutsideWall   = 1 , 
	InbetweenWall = 2 , 
	Unknown
};

//
//

UENUM()
enum class EFillingEntityFriendType : uint8
{
	Wall               = 0, 
	OtherFillingEntity = 1, 
	Void               = 2, 
	Error              = 3
};

UENUM()
enum class EFillingEntityFace : uint8
{
	Face_1 = 0, 
	Face_2 = 1, 
	Face_3 = 2, 
	Face_4 = 3, 
	Face_5 = 4,  // Top
	Face_6 = 5   // Bottom
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// STRUCTS
//

USTRUCT()
struct VILTEK_API FArchCorner
{
	GENERATED_BODY()

public:

	FArchCorner();
	FArchCorner(const FVector& p_cornerLocation);

public:

	void SET_PrecedingCorner(FArchCorner* p_precedingCorner);
	void SET_FollowingCorner(FArchCorner* p_followingCorner);

	//bool CLEAN_StructData();

private:

	ECardinalDirection COMPUTE_CornerDirectionEnum (FArchCorner*        p_corner                                                      ) ;
	FIntPoint          COMPUTE_CornerDirectionPoint(ECardinalDirection  p_cornerDirectionEnum                                         ) ;
	uint32             COMPUTE_DistanceToCorner    (FArchCorner*        p_corner              , ECardinalDirection p_directionToCorner) ;

public:

	FVector s_cornerLocation = FVector(0);

	bool s_bIsFirstArrayElement = false;
	bool s_bIsLastArrayElement  = false;

	FArchCorner* s_precedingCorner = nullptr;
	FArchCorner* s_followingCorner = nullptr;

	ECardinalDirection s_directionToPrecedingCornerEnum = ECardinalDirection::Unknown;
	ECardinalDirection s_directionToFollowingCornerEnum = ECardinalDirection::Unknown;

	FIntPoint s_directionToPrecedingCornerPoint = FIntPoint(-2, -2);
	FIntPoint s_directionToFollowingCornerPoint = FIntPoint(-2, -2);

	uint32 s_distanceToPrecedingCorner = 0;
	uint32 s_distanceToFollowingCorner = 0;
};

//
//

USTRUCT()
struct VILTEK_API FSplitCorner
{
	GENERATED_BODY()

public:

	FSplitCorner();
	FSplitCorner(bool p_bIsSplitting); // Used when splitting has reached its maximum limit
	FSplitCorner(const FIntPoint& p_splitSurface, const FVector& p_splitSurfaceCenterLocation, bool p_bIsSplitting);

public:

	void TDEBUG_FullStructInfo();

public:

	FIntPoint  s_splitSurface               = FIntPoint(0, 0)  ;
	FVector    s_splitSurfaceCenterLocation = FVector(0)       ;
	ESplitType s_splitType                  = ESplitType::None ;
	bool       s_bIsSplitting               = false            ;
};

//
//

USTRUCT()
struct VILTEK_API FRoomDynamicWall
{
	GENERATED_BODY()

public:

	FRoomDynamicWall();
	FRoomDynamicWall(FArchCorner* p_startCorner);

public:

	void SET_PrecedingWall(FRoomDynamicWall* p_precedingWall);
	void SET_FollowingWall(FRoomDynamicWall* p_followingWall);

	void DETERMINE_WallNature          (); // Actually initialize s_wallNature
	void DETERMINE_WallFlowingDirection(); // Actually initialize s_wallFlowingDirection
	void CORRECT_WallLocation          (); // Effectively offsets the wall toward the outside of the room so the inside room area is "clean"
	
	bool CLEAN_StructData();

private:

	FVector            COMPUTE_WallLocation       (FArchCorner* p_startCorner, FArchCorner* p_endCorner                          ) ;
	ECardinalDirection COMPUTE_WallFacingDirection(FArchCorner* p_startCorner                                                    ) ;
	ECardinalAxis      COMPUTE_WallAxis           (FArchCorner* p_startCorner                                                    ) ;
	float              COMPUTE_WallLength         (FArchCorner* p_startCorner, FArchCorner* p_endCorner, ECardinalAxis p_wallAxis) ;

public:

	FVector s_wallLocation = FVector(0);

	class UDynamicMeshComponent* s_DMC_wall = nullptr;

	FArchCorner* s_startCorner = nullptr ;
	FArchCorner* s_endCorner   = nullptr ;

	ECardinalDirection s_wallFacingDirection  = ECardinalDirection::Unknown ; // The direction the wall's length side faces the 'outside'
	ECardinalDirection s_wallFlowingDirection = ECardinalDirection::Unknown ; // The direction the wall prolongs itself. Based of s_startCorner->s_directionToFollowingCornerEnum
	ECardinalAxis      s_wallAxis             = ECardinalAxis     ::Unknown ;

	FRoomDynamicWall* s_precedingWall = nullptr;
	FRoomDynamicWall* s_followingWall = nullptr;

	EWallNature s_wallNature = EWallNature::Unknown;

	bool s_bIsLocationFixed = false;

	float s_wallLength    = 0.0f ;
	float s_wallThickness = 0.0f ;
	float s_wallHeight    = 0.0f ;

	bool s_bHasBeenPierced = false;
};

//
//

USTRUCT()
struct VILTEK_API FRoomStaticWall
{
	GENERATED_BODY()

public:

	FRoomStaticWall();
	FRoomStaticWall(const FRoomDynamicWall& p_DynamicWall);

public:

	void SET_WallMeshComponent(UStaticMeshComponent* p_Component);

public:

	class UStaticMeshComponent* s_SMC_Wall = nullptr;

private:

	float s_WallLength    = 0.0f ;
	float s_WallThickness = 0.0f ;
	float s_WallHeight    = 0.0f ;
};

//
//

USTRUCT()
struct VILTEK_API FRoomFillingEntity
{
	GENERATED_BODY()

public:

	FRoomFillingEntity();
	FRoomFillingEntity(UWorld* p_World, const FVector& p_EntityLocation);

public:

	void SHOOT();
	EFillingEntityFriendType SHOOT_Ray(UWorld* p_World, const FVector& p_RayTargetLoc);
	EFillingEntityFriendType SHOOT_Ray(UWorld* p_World, EFillingEntityFace p_Face);
	
	void SET_FatherEntity(FRoomFillingEntity* p_Entity);
	bool CHECK_WereEntityFriendsAllFind();

public:

	FVector s_EntityLocation = FVector(0);

	FRoomFillingEntity* s_FatherEntity = nullptr;

	TArray<FRoomFillingEntity*> s_FriendEntities;

	//bool s_bAreFacesFriendsAllFound = false;

	EFillingEntityFriendType s_Face1FriendType = EFillingEntityFriendType::Void;
	EFillingEntityFriendType s_Face2FriendType = EFillingEntityFriendType::Void;
	EFillingEntityFriendType s_Face3FriendType = EFillingEntityFriendType::Void;
	EFillingEntityFriendType s_Face4FriendType = EFillingEntityFriendType::Void;
	EFillingEntityFriendType s_Face5FriendType = EFillingEntityFriendType::Void; // Top
	EFillingEntityFriendType s_Face6FriendType = EFillingEntityFriendType::Void; // Bottom

private:

	UWorld* s_World = nullptr;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// CLASS
//
UCLASS()
class VILTEK_API AArchBuilderBase : public AArchBase
{
	GENERATED_BODY()
	
public:	

	AArchBuilderBase();

	void SET_BuilderManager(UBuilderManager* p_builderRef);
	void SET_LevelSpaceManager(ULevelSpaceManager* p_LevelSpaceManagerRef);

	AVolumeShell* GET_VolumeShell() const;

protected:

	virtual void BeginPlay() override;

protected:

	void CREATE_DynamicWalls();
	void CONVERT_DynamicToStatic();
	void CLEAN_DynamicData();

protected:

	// *** Splitting functions *** //
	//
	FSplitCorner SPLIT_CornerBasic
	(
		TArray<FArchCorner*>& p_workingArray       ,
		uint32                p_targetCornerIndex  ,
		const FIntPoint&      p_maximumSplitAmount ,
		const FIntPoint&      p_minimumSplitAmount
	);

	FSplitCorner SPLIT_CornerInside
	(
		TArray<FArchCorner*>& p_workingArray                         ,
		uint32                p_targetCornerIndex                    ,
		uint32                p_maximumSplitAmount = 3               ,
		const FIntPoint&      p_minimumSplitAmount = FIntPoint(2, 2)
	);

	FSplitCorner SPLIT_CornerOutside
	(
		TArray<FArchCorner*>& p_workingArray                  ,
		uint32                p_targetCornerIndex             , 
		const FIntPoint&      p_maximumSplitAmount            , 
		const FSplitCorner&   p_insideSplit                   , 
		uint32                p_minimumExtraSplitDistance = 2
	);

protected: // Debug functions || "VDEBUG" ==>> Visual Debug (usually using DrawDebugHelper), "TDEBUG" ==>> Textual Debug (usually using UE_LOG)

	void VDEBUG_RoomCornersLocations(); // Draw a DebugSphere at each FRoomCorner.s_cornerLocation
	void VDEBUG_RoomCornersLinks(); // Draw a DebugLine between every FRoomCorner

	void TDEBUG_RoomDynamicWall(FRoomDynamicWall* p_wall);
	void TDEBUG_RoomDynamicWallsArray();
	void TDEBUG_SplitCornersArray();

protected:

	TArray<FArchCorner*         > m_ArchCorners      ;
	TArray<FSplitCorner         > m_splitCorners     ;
	TArray<FRoomDynamicWall*    > m_ArchDynamicWalls ;
	TArray<UStaticMeshComponent*> m_StaticWalls      ;

	UDynamicMeshComponent* m_DMC_ground  = nullptr ;
	UDynamicMeshComponent* m_DMC_ceiling = nullptr ;

	uint32 m_roomLength ;
	uint32 m_roomWidth  ;
	uint32 m_roomHeight ;

	// Used mainly for debugging purposes
	UMaterialInstance* m_MI_SKTC_Red   ;
	UMaterialInstance* m_MI_SKTC_Green ;
	UMaterialInstance* m_MI_SKTC_Blue  ;

	// So far initialize in the class constructor. That needs to change
	float m_roomDynamicWallsThickness ; 
	float m_roomDynamicWallsHeight    ;

	// Currently not in use. Don't I need only a FVector though ? What is its purpose ? Might be better off it
	APlayerStart* m_playerStart = nullptr;

	UBuilderManager*    m_builderManager    = nullptr ;
	ULevelSpaceManager* m_LevelSpaceManager = nullptr ;
	AVolumeShell*       m_VolumeShell       = nullptr ;
};
