// Fill out your copyright notice in the Description page of Project Settings.

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
	bool	 bCanDash;

protected:
	virtual void OnInit() override;
	virtual void AIBody() override;

private:
	void GoToEnemy();
	void GoToAlly();
	void AttackEnemy();
	void Dodge();
};
