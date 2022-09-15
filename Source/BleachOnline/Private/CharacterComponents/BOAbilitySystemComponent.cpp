// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAbilitySystemComponent.h"

UBOAbilitySystemComponent::UBOAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBOAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBOAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

