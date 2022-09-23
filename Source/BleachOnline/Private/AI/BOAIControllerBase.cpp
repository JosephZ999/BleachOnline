// Authors MoonDi & JosephZzz for Bleach Online fan game

#include "BOAIControllerBase.h"
#include "BOCharacterBase.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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
		ControlledCharacter->OnDead.AddDynamic(this, &ABOAIControllerBase::OnDeadHandle);

		FTimerHandle InitTimer;
		GetWorldTimerManager().SetTimer(InitTimer, this, &ThisClass::OnInit, 1.f);
	}
}

void ABOAIControllerBase::OnUnPossess()
{
	GetWorldTimerManager().ClearTimer(TickTimer);
}

void ABOAIControllerBase::SetTickTimer(float Delay)
{
	GetWorldTimerManager().SetTimer(TickTimer, this, &ThisClass::AIBody, FMath::Max(TickFrequency, 0.05f), true, Delay);
}

void ABOAIControllerBase::Wait(float Delay)
{
	SetTickTimer(Delay);
}

void ABOAIControllerBase::OnDeadHandle(APawn* Killer, APawn* Victim)
{
	GetWorldTimerManager().ClearTimer(TickTimer);
}

FORCEINLINE bool ABOAIControllerBase::IsPointNear(const FVector& TargetPoint)
{
	const auto Location	 = ControlledCharacter->GetActorLocation();
	const auto DistanceX = FMath::Sqrt(FMath::Square(Location.X - TargetPoint.X));
	const auto DistanceY = FMath::Sqrt(FMath::Square(Location.Y - TargetPoint.Y));

	return DistanceX < CloseDistance * 0.8f && DistanceY < CloseDistance * 0.2f;
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
		GetWorld()->OverlapMultiByChannel(Overlaps, Pos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params);

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

	UE_LOG(LogTemp, Warning, TEXT("Search Enemy, Options %f - %d"), FindEnemyRadius, FindEnemyChunks);
	return Enemy = FindCharacter([&](ABOCharacterBase* Char) { return ControlledCharacter->GetTeam() != Char->GetTeam(); });
}

bool ABOAIControllerBase::SearchAlly()
{
	if (Ally && ! Ally->IsDead()) return true;
	return Ally = FindCharacter([&](ABOCharacterBase* Char) { return ControlledCharacter->GetTeam() == Char->GetTeam(); });
}

bool ABOAIControllerBase::IsEnemyNear()
{
	return IsPointNear(Enemy->GetActorLocation());
}

bool ABOAIControllerBase::IsEnemyFar()
{
	return FVector::Dist2D(ControlledCharacter->GetActorLocation(), Enemy->GetActorLocation()) > LongDistance;
}

bool ABOAIControllerBase::IsAllyNear()
{
	return IsPointNear(Ally->GetActorLocation());
}

bool ABOAIControllerBase::IsAllyFar()
{
	return FVector::Dist2D(ControlledCharacter->GetActorLocation(), Ally->GetActorLocation()) > LongDistance;
}

void ABOAIControllerBase::MoveToPoint(const FVector& NewLocation, float Distance)
{
	const FVector Location		 = ControlledCharacter->GetActorLocation();
	const FVector TargetLocation = FVector((Location.X > NewLocation.X) ? NewLocation.X + Distance : NewLocation.X - Distance, //
		NewLocation.Y, NewLocation.Z);

	const FVector ForwardVector = FRotationMatrix::MakeFromX(TargetLocation - Location).Rotator().Vector();
	ControlledCharacter->SetMovementVectorServer(ForwardVector);
	DrawDebugLine(GetWorld(), Location, TargetLocation, FColor::Cyan, false, TickFrequency, 0, 2.f);
	DrawDebugCapsule(GetWorld(), TargetLocation, 15.f, 7.f, FQuat(FRotator::ZeroRotator), FColor::Cyan, false, TickFrequency, 1, 2.f);
}

void ABOAIControllerBase::StopMoving()
{
	ControlledCharacter->SetMovementVectorServer(FVector::ZeroVector);
}

FVector ABOAIControllerBase::MakeForwardVector(const FVector& TargetLocation)
{
	return FRotationMatrix::MakeFromX(TargetLocation - GetControlledChar()->GetActorLocation()).Rotator().Vector();
}
