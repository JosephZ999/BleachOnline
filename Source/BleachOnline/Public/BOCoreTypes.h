// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "BOCoreTypes.generated.h"

// Damage Actor |==================================================================================
USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	FDamageInfo()
		: Damage(1.f)
		, CritRate(1.f)
		, CritScale(0.5f)
		, ImpulseScale(1.f)
		, ArmorPiercing(0.f)
	{
	}

	FDamageInfo(float InDamage, float InCritRate, float InCritScale, float InImpulseScale, float InArmorPiercing)
		: Damage(InDamage)
		, CritRate(InCritRate)
		, CritScale(InCritScale)
		, ImpulseScale(InImpulseScale)
		, ArmorPiercing(InArmorPiercing)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0", ClampMax = "100", ToolTip = "Damage multiplier"))
	float Damage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Crit chance multiplier"))
	float CritRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Additional damage multiplier"))
	float CritScale = 0.5f; // The value of Damage Actor will add up to the value of the characters's stats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Impulse multiplier"))
	float ImpulseScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0", ClampMax = "1", ToolTip = "1.0 - will ignore enemy's armor"))
	float ArmorPiercing = 0.f; // From 0 to 1
};

// Movement Component |============================================================================
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Stand, // "Stand" should always be 0;
	Walk,
	JumpUp,
	JumpHold,
	JumpDown,
	JumpLand,
	Fall,
	FallUp,
	FallDown,

	// Then animation will not update every frame
	Damaged,
	Hit,
	Hit2,
	Hit3,

	JumpStart,
	StandUp,
	Teleport,
	Custom,
	Max,
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
	None,
	Attack,
	AttackFW,
	AttackBW,
	Jump,
	Guard,

	// Specials
	SpecialFW,
	SpecialBW,
	PowerCharge,
};

typedef EActionType Action;

USTRUCT()
struct FReceivedActionInfo
{
	GENERATED_BODY()

	FReceivedActionInfo() {}

	FReceivedActionInfo(uint8 nCurrentState, EActionType nActionType, const TArray<EActionType>& nKeys, const FVector& nMoveVector)
		: CurrentState(nCurrentState)
		, ActionType(nActionType)
		, Keys(nKeys)
		, MovementVector(nMoveVector)
	{
	}

	uint8				CurrentState;
	EActionType			ActionType;
	TArray<EActionType> Keys;
	FVector				MovementVector;
};

// AI //-----------------------------------------------------------------------------------------//

USTRUCT(BlueprintType)
struct FAIOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings", Meta = (ClampMin = "0", ClampMax = "10"))
	float TickFrequency = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float FindEnemyRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	uint8 FindEnemyChunks = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float CloseDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float LongDistance = 300.f;
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Hidden,
	Base,
	Distance,
	Radial,
	Explosion,
};

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

	FAttackInfo() {}

	FAttackInfo(const EAttackType InType, const FBox& InBox)
		: Type(InType)
		, DamageBox(InBox)
	{
	}

	UPROPERTY(BlueprintReadWrite)
	EAttackType Type;

	UPROPERTY(BlueprintReadWrite)
	FBox DamageBox;

	FVector FindSafeLocation(const FVector& CurrentLocation);
};

USTRUCT(BlueprintType)
struct FGameSettings
{
	GENERATED_BODY()

	FGameSettings() {}
};

UENUM(BlueprintType)
enum class EGameSettingsIndex : uint8
{
	GameTime_I, // I - Value Type - Integer
	FrendlyFire_B,
	Max,
};

USTRUCT(BlueprintType)
struct FGameSettingsParam
{
	GENERATED_BODY()

	FGameSettingsParam()
		: Index(EGameSettingsIndex::Max)
		, bBoolValue(false)
		, FloatValue(0.f)
		, IntegerValue(0)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameSettingsIndex Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBoolValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FloatValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IntegerValue;
};
