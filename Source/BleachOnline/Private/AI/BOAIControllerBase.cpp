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
	CloseDistance	= 100.f;
	LongDistance	= 200.f;
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

template <typename Predicate> //
inline ABOCharacterBase* ABOAIControllerBase::FindCharacter(Predicate Pred)
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

		auto LocalPredicate = [&](FOverlapResult& Overlap)
		{
			ABOCharacterBase* Char = Cast<ABOCharacterBase>(Overlap.GetActor());
			return Char && ! Char->IsDead() && Pred(Char);
		};

		const FOverlapResult* Result = Overlaps.FindByPredicate(LocalPredicate);
		if (Result)
		{
			return Cast<ABOCharacterBase>(Result->GetActor());
		}
	}
	return nullptr;
}

bool ABOAIControllerBase::SearchEnemy()
{
	if (Enemy && ! Enemy->IsDead()) return true;
	return Enemy = FindCharacter([&](ABOCharacterBase* Char) { return ControlledCharacter->GetTeam() != Char->GetTeam(); });
}

bool ABOAIControllerBase::SearchAlly()
{
	if (Ally && ! Ally->IsDead()) return true;
	return Ally = FindCharacter([&](ABOCharacterBase* Char) { return ControlledCharacter->GetTeam() == Char->GetTeam(); });
}

bool ABOAIControllerBase::IsEnemyNear() 
{
	return FVector::Dist2D(ControlledCharacter->GetActorLocation(), Enemy->GetActorLocation()) < CloseDistance;
}

bool ABOAIControllerBase::IsEnemyFar() 
{
	return FVector::Dist2D(ControlledCharacter->GetActorLocation(), Enemy->GetActorLocation()) > LongDistance;
}

bool ABOAIControllerBase::IsAllyNear() 
{
	return FVector::Dist2D(ControlledCharacter->GetActorLocation(), Ally->GetActorLocation()) < CloseDistance;
}

bool ABOAIControllerBase::IsAllyFar() 
{
	return FVector::Dist2D(ControlledCharacter->GetActorLocation(), Ally->GetActorLocation()) > LongDistance;
}

void ABOAIControllerBase::MoveToPoint(const FVector & NewLocation)
{
	const FVector Location = ControlledCharacter->GetActorLocation();
	const FVector ForwardVector = FRotationMatrix::MakeFromX(NewLocation - Location).Rotator().Vector();
	ControlledCharacter->SetMoveVector(ForwardVector);
}
