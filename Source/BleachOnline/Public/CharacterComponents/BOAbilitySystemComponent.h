// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BOAbilityTypes.h"
#include "BOAbilitySystemComponent.generated.h"

class UBOAbilityBase;

/**/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLEACHONLINE_API UBOAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBOAbilitySystemComponent();

	UPROPERTY(Category = "Ablility Settings", EditAnywhere)
	TArray<FAbilityType> Abilities;

private:
	TMap<FName, UBOAbilityBase*> AbilityObjects;

protected:
	virtual void BeginPlay() override;
};
