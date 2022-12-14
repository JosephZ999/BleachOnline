// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BODamageActor.h"
#include "BOCharacterBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogDamageActor, All, All);

ABODamageActor::ABODamageActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComp");
	SetRootComponent(SceneComponent);
	IgnoreList.Empty(3);
}

void ABODamageActor::Init(uint8 CharacterTeam, const FDamageInfo& DamageOptions)
{
	Team = CharacterTeam;
	Damage.Damage *= DamageOptions.Damage;
	Damage.CritRate *= DamageOptions.CritRate;
	Damage.CritScale += DamageOptions.CritScale;
	Impulse *= DamageOptions.ImpulseScale;
	Damage.ArmorPiercing += DamageOptions.ArmorPiercing;
}

void ABODamageActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (HasAuthority())
	{
		if (IgnoreList.Contains(OtherActor)) return;

		float FinalDamage = Damage.Damage;
		OtherActor->TakeDamage(FinalDamage, FDamageEvent(), nullptr, this);
		IgnoreList.Add(OtherActor);

		/*auto Actor = Cast<ABOCharacterBase>(OtherActor);
		if (Actor)
		{
			Actor->OnAttacked.Broadcast(GetInstigator(), FAttackInfo(EAttackType::Radial, GetDamageBox()));
		}*/
	}
}

// Utils

FVector ABODamageActor::GetImpulseVector(const AActor* TargetActor) const
{
	auto TargetLoc = FVector2D(TargetActor->GetActorLocation());
	auto ThisLoc   = FVector2D(this->GetActorLocation());

	if (bRadialImpulse)
	{
		auto LookRotation  = FRotationMatrix::MakeFromX(FVector(TargetLoc, 0.f) - FVector(ThisLoc, 0.f)).Rotator();
		auto ImpulseVector = FVector(Impulse.X, 0.f, Impulse.Y);
		return LookRotation.RotateVector(ImpulseVector);
	}
	return FVector(Impulse.X * FMath::Sign(GetActorForwardVector().X), 0.f, Impulse.Y);
}
