// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BOSaveLoadComponent.generated.h"

class UTexture2D;

/**
 *
 */
UCLASS()
class BLEACHONLINE_API UBOSaveLoadComponent : public UObject
{
	GENERATED_BODY()

public:
	UTexture2D* LoadImageToTexture2D(const FString& ImagePath);
	bool LoadImageAsByte(const FString& FilePath, TArray<uint8>& OutValue);
	bool ConvertByteToImage(TArray<uint8> OutValue, UTexture2D* OutTexture);
	bool LoadImageFromFileDialog(FString& OutFilePath);
};
