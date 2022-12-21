// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BOPlayerDataTypes.h"
#include "BOGameInstanceSubsystem.generated.h"

class UBOSaveLoadComponent;
class UBOPlayerDataComponent;
class UBOGameDataComponent;
class UTexture2D;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBOGameInstanceSubsystem();

	const FString AvatarPath = "PlayerProfile/Avatar";

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

	FPlayerProfile GetPlayerProfile();

	// UFUNCTION(BlueprintCallable)
	UTexture2D* GetImageFromFile(FString& OutPath);

	UFUNCTION(BlueprintCallable)
	bool LoadAvatarAsImage(UTexture2D*& Avatar);

	UFUNCTION(BlueprintCallable)
	bool SetAvatarFromFile(UTexture2D*& Avatar);
};
