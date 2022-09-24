// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "ASDash.h"
#include "AbilityTypes.h"
#include "ASMovementInterface.h"
#include "ASCharacterInterface.h"

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
	UE_LOG(LogDash, Display, TEXT("Dash Dash Dash - %f, %f, %f"), Direction.X, Direction.Y, Direction.Z);
}
