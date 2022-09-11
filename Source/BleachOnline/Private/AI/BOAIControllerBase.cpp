// Fill out your copyright notice in the Description page of Project Settings.

#include "BOAIControllerBase.h"
#include "BOCharacterBase.h"

#include "TimerManager.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogAI, All, All);

ABOAIControllerBase::ABOAIControllerBase()
{
	SetActorTickEnabled(false);
	TickFrequency	= 0.1f;
	FindEnemyRadius = 500.f;
	FindEnemyChunks = 5;
}

void ABOAIControllerBase::OnPossess(APawn* InPawn)
{
	ControlledCharacter = Cast<ABOCharacterBase>(InPawn);
	if (ControlledCharacter)
	{
		SetTickTimer(1.f);
	}
}

void ABOAIControllerBase::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(TickTimer);
}

void ABOAIControllerBase::SetTickTimer(float Delay)
{
	GetWorldTimerManager().SetTimer(TickTimer, this, &ABOAIControllerBase::AIBody, FMath::Max(TickFrequency, 0.05f), true, Delay);
}

void ABOAIControllerBase::Wait(float Delay)
{
	SetTickTimer(Delay);
}

ABOCharacterBase* ABOAIControllerBase::FindEnemy()
{
	if (! GetWorld()) return nullptr;

	if (! ControlledCharacter) return nullptr;

	const FVector Pos = ControlledCharacter->GetActorLocation();

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(ControlledCharacter);

	for (uint8 i = 1; i <= FindEnemyChunks; ++i)
	{
		const float Radius = (FindEnemyRadius / (float)FindEnemyChunks) * i;

		TArray<FOverlapResult> Overlaps;
		GetWorld()->OverlapMultiByChannel(Overlaps, Pos, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(Radius), Params);

		for (auto O : Overlaps)
		{
			ABOCharacterBase* Char = Cast<ABOCharacterBase>(O.GetActor());

			if (! Char) continue;
			if (Char->GetTeam() == ControlledCharacter->GetTeam() || Char->IsDead()) continue;
			return Char;
		}
	}
	return nullptr;
}

void ABOAIControllerBase::AIBody()
{
	//
}
