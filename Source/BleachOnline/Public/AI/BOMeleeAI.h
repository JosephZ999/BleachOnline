// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "AI/BOAIControllerBase.h"
#include "BOMeleeAI.generated.h"

class ABOCharacterBase;

enum class EAITasks : uint8
{
	None,
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

protected:
	virtual void AIBody() override;

private:
	void GoToEnemy();
	void GoToAlly();
	void AttackEnemy();
	void Dodge();
};
