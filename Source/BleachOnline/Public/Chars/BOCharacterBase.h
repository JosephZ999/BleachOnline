// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BOCoreTypes.h"
#include "AbilityTypes.h"
#include "ASCharacterInterface.h"
#include "BOCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, APawn*, KillerPawn, APawn*, VictimPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackedSignature, AActor*, Causer, FAttackInfo, AttackInfo);

class UCapsuleComponent;
class UBOCharacterMovementComponent;
class UBOIndicatorComponent;
class UBOSpriteComponent;
class UPaperFlipbook;
class UBODamageActorComponent;
class UAbilitySystemComponent;

UCLASS(abstract)
class BLEACHONLINE_API ABOCharacterBase : public APawn, public IASCharacterInterface
{
	GENERATED_BODY()

public:
	ABOCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDead;

	UPROPERTY(BlueprintAssignable)
	FOnAttackedSignature OnAttacked;

	UPROPERTY(EditDefaultsOnly)
	float StandUpDelay = 2.f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOCharacterMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOIndicatorComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOSpriteComponent* SpriteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBODamageActorComponent* DamageActorComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilityComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	uint8 Team;

	FTimerHandle EndActionTimer;
	FTimerHandle StandUpTimer;
	bool		 bDead = false;
	FVector		 MovementVector;

protected:
	bool bHidden = false;

protected:
	virtual void BeginPlay() override;
	virtual void OnLanded(FVector LastVelocity);

	virtual bool DoAction(const uint8 MovementState, const EActionType Action);
	virtual bool DoComboAction(const uint8 MovementState, const EActionType Action);

	// Wrapper Functions |=========================================================================

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& NewTransform) override;

	// clang-format off
	FORCEINLINE UCapsuleComponent*				GetCapsuleComp()		const	{ return CapsuleComp;		}
	FORCEINLINE UBOSpriteComponent*				GetSpriteComp()			const	{ return SpriteComp;		}
	FORCEINLINE UBOCharacterMovementComponent*	GetMoveComp()			const	{ return MovementComp;		}
	FORCEINLINE UBOIndicatorComponent*			GetHealthComp()			const	{ return HealthComp;		}
	FORCEINLINE UBODamageActorComponent*		GetDamageActorComp()	const	{ return DamageActorComp;	}
	FORCEINLINE UAbilitySystemComponent*		GetAbilityComp()		const	{ return AbilityComp;		}
	// clang-format on

	virtual FDamageInfo GetDamageInfo();

	virtual void	AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	virtual FVector GetVelocity() const override;

	void LaunchCharacter(const FVector& Direction, float Impulse, bool bXYOverride = false, bool bZOverride = false);
	void LaunchCharacterDeferred(const FVector& Direction, float Impulse, float Delay, bool bXYOverride = false, bool bZOverride = false);
	void AddVelocity(const FVector& Direction, float Length);

	void SetRotation(float RotationYaw);

	UFUNCTION(BlueprintCallable)
	FRotator TurnCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void TurnCharacterClient(float RotationYaw);
	void TurnCharacterClient_Implementation(float RotationYaw);

	UFUNCTION(BlueprintCallable)
	void SetTeam(uint8 NewTeam) { Team = NewTeam; }

	uint8	GetTeam() const { return Team; }
	uint8	GetMovementState();
	FVector GetMoveVector() const;
	float	GetAnimTime(const float Frame);

	bool IsOnGround() const;
	bool IsDoingAnything() const;
	bool IsLookRight() const { return GetActorForwardVector().X > 0.f; }
	bool IsDead() { return bDead || bHidden; }
	bool IsInvulnerable() const;

	void NewAction(uint8 State, const FName& Animation, float Length = 0.f, bool LoopAnim = false);

	UFUNCTION(NetMulticast, Unreliable)
	void NewActionClient(uint8 NewState, const FName& Animation, float Length, bool LoopAnim);
	void NewActionClient_Implementation(uint8 NewState, const FName& Animation, float Length, bool LoopAnim);

	void		 EndActionDeferred(float WaitTime);
	virtual void EndAction();

	void SetMovementVector(const FVector& NewVector);

	UFUNCTION(Server, UnReliable)
	void SetMovementVectorServer(const FVector& NewVector);
	void SetMovementVectorServer_Implementation(const FVector& NewVector);

	UFUNCTION(NetMulticast, Reliable)
	void SetMovementVectorClient(const FVector& NewVector);
	void SetMovementVectorClient_Implementation(const FVector& NewVector);

	UFUNCTION(Server, UnReliable)
	void DoActionServer(EActionType ActionType);
	void DoActionServer_Implementation(EActionType ActionType);

	UFUNCTION(NetMulticast, Reliable)
	void DoActionClient(uint8 InitialState, EActionType Action);
	void DoActionClient_Implementation(uint8 InitialState, EActionType Action);



	/* On Server */
	void Jump();
	void StandUp();

	void SetCharacterCollision(bool Enabled);
	void SetCharacterVisibility(bool Visible);
	void DestroyDamageActor();

	// AbilitySystem Interface //---------------------------------------------------------//
public:
	virtual UObject* IGetIndicatorComponent(EIndicatorType Type) const override;
	virtual UObject* IGetMovementComponent() const override;
	virtual void	 SetAnimation(const FName& AnimationName, bool bLoop) override;
	//------------------------------------------------------------------------------------//

	// AI Interface //--------------------------------------------------------------------//
public:
	UFUNCTION(BlueprintNativeEvent)
	FAIOptions GetAIOptions();
	FAIOptions GetAIOptions_Implementation() { return FAIOptions(); }
	//------------------------------------------------------------------------------------//

private:
	void SetMovementRotation();

	UFUNCTION()
	void OnTakeAnyDamageHandle(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnDeath();

	UFUNCTION(NetMulticast, Unreliable)
	void OnDeathClient();
	void OnDeathClient_Implementation();
};
