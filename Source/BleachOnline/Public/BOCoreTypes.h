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

	Damaged,
	Hit,
	Hit2,
	Hit3,

	StandUp,
	Custom,
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
