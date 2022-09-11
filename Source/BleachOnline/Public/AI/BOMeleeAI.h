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
