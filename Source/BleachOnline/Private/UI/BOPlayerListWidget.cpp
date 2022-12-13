// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOPlayerListWidget.h"
#include "BOPlayerListComponent.h"
#include "BOGameState.h"

// UI Components
#include "Components\VerticalBox.h"
#include "BOPlayerListElemWidget.h"

void UBOPlayerListWidget::ResetWidget()
{
	if (! bBindedToJoining)
	{
		BindToJoining();
	}

	if (GetPlayerListComp())
	{
		for (auto OtherPlayer : GetPlayerListComp()->GetOtherPlayers())
		{
			CreateElemnt(OtherPlayer);
		}
	}

}

void UBOPlayerListWidget::BindToJoining()
{
	if (! GetPlayerListComp()) return;

	GetPlayerListComp()->OnPlayerJoin.AddUObject(this, &ThisClass::OnPlayerJoin);
	bBindedToJoining = true;
}

void UBOPlayerListWidget::OnPlayerJoin(APlayerState* NewPlayer)
{
	CreateElemnt(NewPlayer);
}

void UBOPlayerListWidget::CreateElemnt(APlayerState* Player)
{
	if (GetPlayerListComp()->GetLocalPlayer() == Player) return;

	// check if player already added to list
	for (auto Child : List->GetAllChildren())
	{
		auto Elem = Cast<UBOPlayerListElemWidget>(Child);
		if (Elem && Elem->ComparePlayer(Player)) return;
	}

	auto NewElement = CreateWidget<UBOPlayerListElemWidget>(GetOwningPlayer(), ElemClass);
	NewElement->SetPlayer(Player);

	List->AddChildToVerticalBox(NewElement);
}

PlayerList* UBOPlayerListWidget::GetPlayerListComp()
{
	if (PlayerListCompCache)
	{
		return PlayerListCompCache;
	}

	if (! GetWorld()) return nullptr;

	auto GS = GetWorld()->GetGameState<ABOGameState>();
	if (! GS) return nullptr;

	return PlayerListCompCache = GS->GetPlayerListComponent();
}
