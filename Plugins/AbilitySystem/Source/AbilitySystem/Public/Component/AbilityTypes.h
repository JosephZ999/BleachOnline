#pragma once

#include "AbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EAbilityParamType : uint8
{
	None,
	Float,
	Integer,
	Vector,
	Object,
};

UENUM(BlueprintType)
enum class EIndicatorType : uint8
{
	None,
	Health,
	Power,
	Stamina,
	Custom,
};

USTRUCT(BlueprintType)
struct FAbilityParam
{
	GENERATED_BODY()

	FAbilityParam()
		: Type(EAbilityParamType::None)
		, FloatValue(0.f)
		, IntegerValue(0)
		, VectorValue(FVector::ZeroVector)
		, ObjectValue(nullptr)
	{
	}

	explicit FAbilityParam(float InValue)
		: Type(EAbilityParamType::Float)
		, FloatValue(InValue)
	{
	}

	explicit FAbilityParam(int32 InValue)
		: Type(EAbilityParamType::Integer)
		, IntegerValue(InValue)
	{
	}

	explicit FAbilityParam(const FVector& InValue)
		: Type(EAbilityParamType::Vector)
		, VectorValue(InValue)
	{
	}

	explicit FAbilityParam(UObject* InObject)
		: Type(EAbilityParamType::Object)
		, ObjectValue(InObject)
	{
	}

	// clang-format off
	UPROPERTY(BlueprintReadOnly) EAbilityParamType	Type;
	UPROPERTY(BlueprintReadOnly) float				FloatValue		= 0.f;
	UPROPERTY(BlueprintReadOnly) int32				IntegerValue	= 0;
	UPROPERTY(BlueprintReadOnly) FVector			VectorValue		= FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) UObject*			ObjectValue		= nullptr;
	// clang-format on
};

namespace AbilityNames
{
const FName Dash("Dash");
const FName Accel("Accel");
const FName Shoot("Shoot");
}
