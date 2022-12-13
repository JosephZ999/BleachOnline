// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerListElemWidget.h"
#include "BOPlayerState.h"

#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerListElemWidget, All, All);

void UBOPlayerListElemWidget::NativeConstruct()
{
	UpdateProfile();
}

void UBOPlayerListElemWidget::SetPlayer(APlayerState* NewPlayer)
{
	Player = NewPlayer;
}

bool UBOPlayerListElemWidget::ComparePlayer(APlayerState* OtherPlayer)
{
	return Player ? Player == OtherPlayer : false;
}

void UBOPlayerListElemWidget::UpdateProfile()
{
	if (! GetWorld()) return;

	if (! Player)
	{
		RemoveFromParent();
		return;
	}

	auto PS = Cast<ABOPlayerState>(Player);
	checkf(PS, TEXT("PlayerState is null or incorrect type"));

	if (PS->GetProfile().Name.IsEmpty())
	{
		PS->GetWorldTimerManager().SetTimer(UpdateTimer, this, &ThisClass::UpdateProfileHandle, true, 1.f);
	}
}

void UBOPlayerListElemWidget::UpdateProfileHandle()
{
	if (! GetWorld()) return;

	if (! Player)
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateTimer);
		RemoveFromParent();
	}

	auto PS = Cast<ABOPlayerState>(Player);
	if (PS && ! PS->GetProfile().Name.IsEmpty())
	{
		PS->GetWorldTimerManager().ClearTimer(UpdateTimer);

		// Updtate Profile
		// ...
	}
}
