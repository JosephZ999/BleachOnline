// Authors MoonDi & JosephZzz for BleachOnline fan game.

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

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UPaperFlipbook*> Animations;

	FTimerHandle AnimationUpdateTimer;
	FName		 LastAnimName;

protected:
	void BeginPlay() override;

public:
	void Construction();
	void SetBaseAnimations(TMap<FName, UPaperFlipbook*>& NewAnimations);
	void InitAnimations(TMap<FName, UPaperFlipbook*>& OutAnimations, const FString& AnimsFolder);
	void InitAnimations(const FString& AnimsFolder);
	void SetAnimation(const FName& AnimationName, bool Looping);

	bool ContainsAnim(const FName& AnimName);
	bool IsAnimsEmpty() const { return Animations.Num() == 0; }
	bool CheckLastAnim(const FName& AnimName) const;

private:
	void AnimationUpdateHandle();
};
