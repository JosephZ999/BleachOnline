// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BOGameInstanceSubsystem.generated.h"

class UBOSaveLoadComponent;
class UBOPlayerDataComponent;
class UBOGameDataComponent;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBOGameInstanceSubsystem();

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOSaveLoadComponent* SaveLoadComp;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOPlayerDataComponent* PlayerDataComp;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBOGameDataComponent* GameDataComp;

public:
	template <typename T> T* GetSubsystemComponent()
	{
		TArray<UObject*> Objects;
		GetDefaultSubobjects(Objects);
		for (auto Component : Objects)
		{
			if (Component->IsA<T>())
			{
				return Cast<T>(Component);
			}
		}
		return nullptr;
	}
};
