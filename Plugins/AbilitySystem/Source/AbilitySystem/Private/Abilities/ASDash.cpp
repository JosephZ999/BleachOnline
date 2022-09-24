// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "ASDash.h"
#include "AbilityTypes.h"
#include "ASMovementInterface.h"
#include "ASCharacterInterface.h"
#include "TimerManager.h"
#include "Engine\World.h"

DEFINE_LOG_CATEGORY_STATIC(LogDash, All, All);

UASDash::UASDash()
{
	Name = AbilityNames::Dash;
}

void UASDash::OnActivate()
{
	Super::OnActivate();
}

void UASDash::OnActivateWithParam(const FAbilityParam& Param)
{
	Super::OnActivateWithParam(Param);

	if (Param.Type == EAbilityParamType::Vector)
	{
		DoSomething(Param.VectorValue);
	}
}

void UASDash::DoSomething(FVector Direction) 
{
	auto Char = Cast<IASCharacterInterface>(GetOwner());
	if (!Char) return;

	auto MoveComp = Cast<IASMovementInterface>(Char->IGetMovementComponent());
	if (!MoveComp) return;

	Direction.Z = 100.f;
	MoveComp->ILaunch(Direction.GetClampedToMaxSize(MaxLength), true, true);
	Char->SetAnimation("Dash", true);

	UE_LOG(LogDash, Warning, TEXT("Dash dash"));

	// Åemporary solution
	FTimerHandle EndAnimHandle;
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(EndAnimHandle, this, &ThisClass::EndAnim, 0.2f);
}

void UASDash::EndAnim()
{
	auto Char = Cast<IASCharacterInterface>(GetOwner());
	if (!Char) return;

	Char->SetAnimation(FName(), false);
}
