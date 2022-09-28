// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOMeleeAI.h"
#include "BOCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine\World.h"

DEFINE_LOG_CATEGORY_STATIC(LogMeleeAI, All, All);

void ABOMeleeAI::OnInit()
{
	if (const auto P = Cast<ABOCharacterBase>(GetPawn()))
	{
		bCanDash  = P->GetAbilityComp()->HasAbility(AbilityNames::Dash);
		bCanShoot = P->GetAbilityComp()->HasAbility(AbilityNames::Shoot);

		P->OnAttacked.AddDynamic(this, &ABOMeleeAI::OnPawnAttacked);
	}
}

void ABOMeleeAI::AIBody()
{
	Super::AIBody();

	switch (Task)
	{ // clang-format off
	case EAITasks::GoToPoint:   GoToPoint();   return;
	case EAITasks::GoToEnemy:   GoToEnemy();   return;
	case EAITasks::GoToAlly:    GoToAlly();    return;
	case EAITasks::AttackEnemy: AttackEnemy(); return;
	case EAITasks::Dodge:       Dodge();       return;
	} // clang-format on

	// When task is none
	if (SearchEnemy())
	{
		Task = EAITasks::GoToEnemy;
		return;
	}

	if (SearchAlly())
	{
		Task = EAITasks::GoToAlly;
		return;
	}
	Wait(3.f); // Disable AI for 3 seconds
}

void ABOMeleeAI::GoToPoint()
{
	if (FVector::PointsAreNear(GetPawnLocation(), TargetPoint, CloseDistance * 0.5f))
	{
		Task = EAITasks::None;
		StopMoving();
		return;
	}
}

void ABOMeleeAI::GoToEnemy()
{
	if (! SearchEnemy())
	{
		Task = EAITasks::GoToAlly;
		Wait(0.5f);
		return;
	}

	if (IsPointNear(GetEnemyLocation()))
	{
		Task = EAITasks::AttackEnemy;
		return;
	}

	if (IsPointFar(GetEnemyLocation()))
	{
		if (bCanDash)
		{
			const FVector		Direction = MakeForwardVector(GetEnemyLocation()) * ((GetDist(GetTargetPoint()) * 1.2f));
			const FAbilityParam Param(Direction);
			const auto			P = Cast<ABOCharacterBase>(GetPawn());
			P->SetMovementVectorServer(Direction);
			P->GetAbilityComp()->ActivateAbilityWithParam(AbilityNames::Dash, Param);
			return;
		}

		if (SearchAlly())
		{
			Task = EAITasks::GoToAlly;
			return;
		}
	}

	if (bCanShoot)
	{
		const auto P = Cast<ABOCharacterBase>(GetPawn());
		if (P->GetAbilityComp()->ActivateAbility(AbilityNames::Dash))
		{
			StopMoving();
			return;
		}
	}

	MoveToLocation(GetEnemyLocation(), CloseDistance * 0.25f);
	Wait(0.2f);
}
void ABOMeleeAI::GoToAlly()
{
	if (! SearchAlly())
	{
		Task = EAITasks::None;
		StopMoving();
		Wait(0.5f);
		return;
	}

	if (GetDist2D(GetAllyLocation()) < CloseDistance)
	{
		Task = EAITasks::None;
		StopMoving();
		Wait(0.5f);
		return;
	}

	MoveToLocation(GetAllyLocation(), CloseDistance * 0.25f);
}

void ABOMeleeAI::AttackEnemy()
{
	const auto P = Cast<ABOCharacterBase>(GetPawn());

	if (P->GetMovementState() <= 1 && IsPointNear(GetEnemyLocation()))
	{
		const FVector MoveVector = MakeForwardVector(GetEnemyLocation());
		const float	  Distance	 = GetDist2D(GetEnemyLocation());

		/* Set Rotation */

		StopMoving();
		P->LaunchCharacter(MoveVector, Distance * AttackVelocityScale, true, true);
		P->DoActionServer(EActionType::Attack);

		Wait(1.f);
	}
	Task = EAITasks::GoToEnemy;
}

void ABOMeleeAI::Dodge()
{
	ClearEnemy();

	const FVector BoxCenter		  = DangetInfo.DamageBox.GetCenter();
	const FVector FW			  = MakeForwardVector(FVector(BoxCenter.X, BoxCenter.Y, GetPawnLocation().Z)) * -1.f;
	const float	  Radius		  = DangetInfo.DamageBox.GetSize().Size2D();

	FVector SafeLocation;

	FHitResult Hit;
	{
		const FVector TraceStart = GetPawnLocation();
		const FVector TraceEnd	 = GetPawnLocation() + FW * Radius;
		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Vehicle);
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 5.f, 0, 2.f);
	}
	if (Hit.bBlockingHit)
	{
		if (Hit.Distance > Radius * 0.5f)
		{
			FHitResult	  Hit2;
			const FVector TraceStart = Hit.ImpactPoint;
			const FVector TraceEnd	 = Hit.ImpactPoint + FMath::GetReflectionVector(Hit.TraceEnd - Hit.TraceStart, Hit.ImpactNormal);
			GetWorld()->LineTraceSingleByChannel(Hit2, TraceStart, TraceEnd, ECollisionChannel::ECC_Vehicle);
			SafeLocation = Hit2.bBlockingHit ? Hit2.ImpactPoint : Hit2.TraceEnd;
			// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Yellow, false, 5.f, 0, 2.f);
		}
		else
		{
			SafeLocation = Hit.ImpactPoint;
		}
	}
	else
	{
		SafeLocation = Hit.TraceEnd;
	}

	MoveToLocation(SafeLocation);
	Task		= EAITasks::GoToPoint;
	TargetPoint = SafeLocation;

	if (bCanDash)
	{
		const auto P = Cast<ABOCharacterBase>(GetPawn());
		P->SetMovementVectorServer(FW);
		if (P->GetAbilityComp()->ActivateAbilityWithParam(
				AbilityNames::Dash, FAbilityParam(MakeForwardVector(SafeLocation) * (Radius * 5.f))))
		{
			Task = EAITasks::None;
		}
	}
}

void ABOMeleeAI::OnPawnAttacked(AActor* DamageCauser, FAttackInfo Info)
{
	Task = EAITasks::Dodge;
	if (Info.Type > DangetInfo.Type)
	{
		DangetInfo = Info;
	}
	Wait(0.1f);
}
