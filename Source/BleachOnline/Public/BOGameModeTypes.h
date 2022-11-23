// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "BOGameModeTypes.generated.h"

UENUM(BlueprintType)
enum class EGameParamIndex : uint8
{
	GameTime_I, // I - Value Type - Integer
	FrendlyFire_B,
	Max,
};

UENUM(BlueprintType)
enum class EGameParamType : uint8
{
	Bool,
	Float,
	Integer,
};

USTRUCT(BlueprintType)
struct FGameParam
{
	GENERATED_BODY()

	FGameParam()
		: Index(EGameParamIndex::Max)
		, Type(EGameParamType::Bool)
		, bBoolValue(false)
		, FloatValue(0.f)
		, IntegerValue(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameParamIndex Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameParamType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBoolValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FloatValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IntegerValue;
};

USTRUCT(BlueprintType)
struct FGameSettings
{
	GENERATED_BODY()

	FGameSettings()
		: Params({})
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameParam> Params;
};
