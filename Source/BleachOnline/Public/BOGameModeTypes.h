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
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameParamIndex Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameParamType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue = 1.f;
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
