#pragma once

#include "BOCoreTypes.h"
#include "BOAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EAbilityParamType : uint8
{
	None,
	Float,
	Integer,
	Vector,
	Object,
};

USTRUCT(BlueprintType)
struct FAbilityParam
{
	GENERATED_BODY()

	EAbilityParamType Type;

	float	 FloatValue;
	int32	 IntegerValue;
	FVector	 VectorValue;
	UObject* ObjectValue;
};
