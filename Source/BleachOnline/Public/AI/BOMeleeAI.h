// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "AI/BOAIControllerBase.h"
#include "BOMeleeAI.generated.h"

class ABOCharacterBase;

enum class EAITasks : uint8
{
	None,
	GoToPoint,
	GoToEnemy,
	GoToAlly,
	AttackEnemy,
	Dodge,
};

/**
 *
 */
UCLASS()
class BLEACHONLINE_API ABOMeleeAI : public ABOAIControllerBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float AttackVelocityScale = 3.f;

private:
	EAITasks Task;
	bool	 bCanDash;
	FVector	 TargetPoint;

protected:
	virtual void OnInit() override;
	virtual void AIBody() override;

private:
	inline void GoToPoint();
	inline void GoToEnemy();
	inline void GoToAlly();
	inline void AttackEnemy();
	inline void Dodge();
};
