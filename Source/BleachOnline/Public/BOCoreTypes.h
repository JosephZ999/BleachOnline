#pragma once

#include "CoreMinimal.h"
#include "BOCoreTypes.generated.h"

// Damage Actor |==================================================================================
USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "100", ToolTip = "Damage multiplier"))
	float Damage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Crit chance multiplier"))
	float CritRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Additional damage multiplier"))
	float CritScale = 0.5f; // The value of Damage Actor will add up to the value of the characters's stats

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "10", ToolTip = "Impulse multiplier"))
	float ImpulseScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ClampMin = "0", ClampMax = "1", ToolTip = "1.0 - will ignore enemy's armor"))
	float ArmorPiercing = 0.f; // From 0 to 1
};

// Movement Component |============================================================================
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Stand,
	Walk,
	JumpUp,
	JumpHold,
	JumpDown,
	JumpLand,

	Damaged,
	Hit,
	Hit2,
	Hit3,

	Fall,
	StandUp,
	Custom,
};
