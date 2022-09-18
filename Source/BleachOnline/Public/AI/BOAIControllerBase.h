// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BOAIControllerBase.generated.h"

class ABOCharacterBase;

/**
 *
 */
UCLASS(abstract)
class BLEACHONLINE_API ABOAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	ABOAIControllerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings", Meta = (ClampMin = "0", ClampMax = "10"))
	float TickFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float FindEnemyRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	uint8 FindEnemyChunks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float CloseDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
	float LongDistance;
	
	ABOCharacterBase* Enemy;
	ABOCharacterBase* Ally;


private:
	ABOCharacterBase* ControlledCharacter;
	FTimerHandle	  TickTimer;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void AIBody() {}
	virtual void OnInit() {}

	bool SearchEnemy();
	bool SearchAlly();
	bool IsEnemyNear();
	bool IsEnemyFar();
	bool IsAllyNear();
	bool IsAllyFar();

	void MoveToPoint(const FVector& NewLocation, float Distance);
	void StopMoving();

	
	FVector MakeForwardVector(const FVector& TargetLocation);

	FORCEINLINE ABOCharacterBase* GetControlledChar() { return ControlledCharacter; }

public:
	UFUNCTION(BlueprintCallable)
	void Wait(float Delay);

	template <typename Predicate>
	ABOCharacterBase* FindCharacter(Predicate Pred);

private:
	void SetTickTimer(float Delay);

	UFUNCTION()
	void OnDeadHandle(APawn* Killer, APawn* Victim);

	bool IsPointNear(const FVector& TargetPoint);
};
