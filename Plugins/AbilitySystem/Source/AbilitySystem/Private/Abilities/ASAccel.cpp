// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "ASAccel.h"
#include "AbilityTypes.h"
#include "ASCharacterInterface.h"
#include "ASMovementInterface.h"

#include "GameFramework/Actor.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogAccel, All, All);

UASAccel::UASAccel()
{
	Name = AbilityNames::Accel;
}

void UASAccel::OnActivate()
{
	Super::OnActivate();
	DoSomething();
}

void UASAccel::OnActivateWithParam(const FAbilityParam& Param)
{
	Super::OnActivateWithParam(Param);
	DoSomething();
}

void UASAccel::DoSomething()
{
	auto Char = Cast<IASCharacterInterface>(GetOwner());
	if (! Char) return;

	auto MoveComp = Cast<IASMovementInterface>(Char->IGetMovementComponent());
	if (! MoveComp) return;

	if (GetOwner()->GetWorldTimerManager().IsTimerActive(DisableAbilityTimer))
	{
		Disable();
	}

	const float CurrentSpeed = MoveComp->IGetWalkSpeed();
	const float NewSpeed	 = (CurrentSpeed + AddWalkSpeed) * MultiWalkSpeed;
	AdditionalSpeed			 = NewSpeed - CurrentSpeed;

	MoveComp->IAddWalkSpeed(AdditionalSpeed);
	GetOwner()->GetWorldTimerManager().SetTimer(DisableAbilityTimer, this, &ThisClass::Disable, FMath::Max(Duration, 1.f));
}

void UASAccel::Disable()
{
	auto Char = Cast<IASCharacterInterface>(GetOwner());
	if (! Char) return;

	auto MoveComp = Cast<IASMovementInterface>(Char->IGetMovementComponent());
	if (! MoveComp) return;

	MoveComp->IAddWalkSpeed(-(AdditionalSpeed));
}
