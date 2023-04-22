#pragma once

#include "CoreMinimal.h"
#include "Architecture/ArchBuilderBase.h"
#include "CorridorTest.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogClass_CorridorTest, Log, All);

UCLASS()
class VILTEK_API ACorridorTest : public AArchBuilderBase
{
	GENERATED_BODY()

public:

	ACorridorTest();
	void BUILD_Corridor();

protected:

};
