#pragma once

#include "CoreMinimal.h"
#include "Architecture/ArchBuilderBase.h"
#include "SasBuilder.generated.h"

UCLASS()
class VILTEK_API ASasBuilder : public AArchBuilderBase
{
	GENERATED_BODY()

public:

	ASasBuilder();

public:

	ECardinalDirection m_FacingDirection;
};
