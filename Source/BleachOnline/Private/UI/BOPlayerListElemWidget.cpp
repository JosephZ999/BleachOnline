// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerListElemWidget.h"
#include "BOPlayerState.h"

#include "TimerManager.h"
#include "Components\TextBlock.h"
#include "Components\Image.h"

#include "BOFunctionLibrary.h"
#include "BOGameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerListElemWidget, All, All);

void UBOPlayerListElemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateProfile();
}

void UBOPlayerListElemWidget::SetPlayer(APlayerState* NewPlayer)
{
	Player = NewPlayer;
	Player->OnEndPlay.AddDynamic(this, &ThisClass::OnPlayerEndPlay);
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
	if (! PS) return;

	if (PS->GetProfile().Name.IsEmpty())
	{
		PS->GetWorldTimerManager().SetTimer(UpdateTimer, this, &ThisClass::UpdateProfile, 0.5f, true);
		return;
	}

	OnProfileUpdate(PS->GetProfile());
	PS->GetWorldTimerManager().ClearTimer(UpdateTimer);
}

void UBOPlayerListElemWidget::OnPlayerEndPlay(AActor* Target, EEndPlayReason::Type EndPlayReason)
{
	RemoveFromParent();
}
