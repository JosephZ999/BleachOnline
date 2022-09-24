// Authors MoonDi & JosephZzz for BleachOnline fan game.

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
	Super::OnPossess(InPawn);
	auto P = Cast<ABOCharacterBase>(InPawn);
	if (P)
	{
		SetTickTimer(1.f);
		P->OnDead.AddDynamic(this, &ABOAIControllerBase::OnDeadHandle);

		FTimerHandle InitTimer;
		GetWorldTimerManager().SetTimer(InitTimer, this, &ThisClass::OnInit, 1.f);
	}
}

void ABOAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
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

FORCEINLINE bool ABOAIControllerBase::IsPointNear(const FVector& InTargetPoint)
{
	const auto Location	 = GetPawn()->GetActorLocation();
	const auto DistanceX = FMath::Sqrt(FMath::Square(Location.X - InTargetPoint.X));
	const auto DistanceY = FMath::Sqrt(FMath::Square(Location.Y - InTargetPoint.Y));

	return DistanceX < CloseDistance * 0.8f && DistanceY < CloseDistance * 0.2f;
}

FORCEINLINE bool ABOAIControllerBase::IsPointFar(const FVector& InTargetPoint)
{
	return FVector::Dist2D(GetPawn()->GetActorLocation(), InTargetPoint) > LongDistance;
}

template <typename Predicate> //
inline ABOCharacterBase* ABOAIControllerBase::FindCharacter(Predicate Pred)
{
	if (! GetWorld()) return nullptr;

	if (! GetPawn()) return nullptr;

	const FVector Pos = GetPawnLocation();

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(GetPawn());

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

	auto P		 = Cast<ABOCharacterBase>(GetPawn());
	return Enemy = FindCharacter([&](ABOCharacterBase* Char) { return P->GetTeam() != Char->GetTeam(); });
}

bool ABOAIControllerBase::SearchAlly()
{
	if (Ally && ! Ally->IsDead()) return true;

	auto P		= Cast<ABOCharacterBase>(GetPawn());
	return Ally = FindCharacter([&](ABOCharacterBase* Char) { return P->GetTeam() == Char->GetTeam(); });
}

void ABOAIControllerBase::MoveToLocation(const FVector& InLocation, float Distance)
{
	const FVector Location = GetPawn()->GetActorLocation();
	TargetPoint			   = FVector((Location.X > InLocation.X) ? InLocation.X + Distance : InLocation.X - Distance, //
		InLocation.Y, InLocation.Z);

	const FVector ForwardVector = FRotationMatrix::MakeFromX(TargetPoint - Location).Rotator().Vector();
	const auto	  P				= Cast<ABOCharacterBase>(GetPawn());

	P->SetMovementVectorServer(ForwardVector);

	DrawDebugLine(GetWorld(), Location, TargetPoint, FColor::Cyan, false, TickFrequency, 0, 2.f);
	DrawDebugCapsule(GetWorld(), TargetPoint, 15.f, 7.f, FQuat(FRotator::ZeroRotator), FColor::Cyan, false, TickFrequency, 1, 2.f);
}

void ABOAIControllerBase::StopMoving()
{
	auto P = Cast<ABOCharacterBase>(GetPawn());
	P->SetMovementVectorServer(FVector::ZeroVector);
}

FVector ABOAIControllerBase::GetEnemyLocation() const
{
	return Enemy->GetActorLocation();
}

FVector ABOAIControllerBase::GetPawnLocation() const
{
	return GetPawn()->GetActorLocation();
}

FVector ABOAIControllerBase::GetAllyLocation() const
{
	return Ally->GetActorLocation();
}

FVector ABOAIControllerBase::MakeForwardVector(const FVector& TargetLocation)
{
	return FRotationMatrix::MakeFromX(TargetLocation - GetPawn()->GetActorLocation()).Rotator().Vector();
}

float ABOAIControllerBase::GetDist(const FVector& TargetLocation)
{
	return FVector::Dist(GetPawnLocation(), TargetLocation);
}

float ABOAIControllerBase::GetDist2D(const FVector& TargetLocation)
{
	return FVector::Dist2D(GetPawnLocation(), TargetLocation);
}
