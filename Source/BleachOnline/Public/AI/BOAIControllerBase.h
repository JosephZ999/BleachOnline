// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\Controller.h"
#include "BOAIControllerBase.generated.h"

class ABOCharacterBase;

/**
 *
 */
UCLASS(abstract)
class BLEACHONLINE_API ABOAIControllerBase : public AController
{
	GENERATED_BODY()

public:
	ABOAIControllerBase();

protected:
	float TickFrequency;
	float FindEnemyRadius;
	uint8 FindEnemyChunks;
	float CloseDistance;
	float LongDistance;

private:
	UPROPERTY()
	ABOCharacterBase* Enemy;

	UPROPERTY()
	ABOCharacterBase* Ally;

	FTimerHandle TickTimer;
	FVector		 TargetPoint;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void AIBody() {}
	virtual void OnInit() {}

	bool SearchEnemy();
	bool SearchAlly();
	bool IsPointNear(const FVector& InTargetPoint);
	bool IsPointFar(const FVector& InTargetPoint);

	void MoveToLocation(const FVector& InLocation, float Distance = 0.f);
	void StopMoving();

	FORCEINLINE ABOCharacterBase* GetEnemy() { return Enemy; }
	FORCEINLINE ABOCharacterBase* GetAlly() { return Ally; }

	FORCEINLINE FVector GetEnemyLocation() const;
	FORCEINLINE FVector GetPawnLocation() const;
	FORCEINLINE FVector GetAllyLocation() const;

	FORCEINLINE FVector MakeForwardVector(const FVector& TargetLocation);
	FORCEINLINE float	GetDist(const FVector& TargetLocation);
	FORCEINLINE float	GetDist2D(const FVector& TargetLocation);
	FORCEINLINE FVector GetTargetPoint() { return TargetPoint; }

public:
	UFUNCTION(BlueprintCallable)
	void Wait(float Delay);

	template <typename Predicate> ABOCharacterBase* FindCharacter(Predicate Pred);

private:
	void SetTickTimer(float Delay);

	UFUNCTION()
	void OnDeadHandle(APawn* Killer, APawn* Victim);
};
