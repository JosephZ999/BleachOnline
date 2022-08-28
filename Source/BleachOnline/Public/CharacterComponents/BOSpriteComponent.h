// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "BOSpriteComponent.generated.h"

class UPaperFlipbook;
class UBOCharacterMovementComponent;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOSpriteComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()

public:
	UBOSpriteComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main")
	float AnimationUpdateTick;

private:
	UBOCharacterMovementComponent* OwnerMoveComp;
	TMap<FName, UPaperFlipbook*> Animations;
	FTimerHandle AnimationUpdateTimer;

protected:
	void BeginPlay() override;

public:
	void Construction();
	void SetBaseAnimations(TMap<FName, UPaperFlipbook*>& NewAnimations);
	static void InitBaseAnimations(TMap<FName, UPaperFlipbook*>& OutAnimations, FString AnimsFolder);

	void SetAnimation(const FName& AnimationName, bool Looping);

private:
	void AnimationUpdateHandle();
};
