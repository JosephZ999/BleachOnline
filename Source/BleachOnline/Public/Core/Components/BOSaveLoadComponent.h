// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IImageWrapper.h"
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

	UTexture2D* ConvertByteToImage(const TArray<uint8>& File, EImageFormat Format);
	bool		LoadImageAsByte(const FString& FilePath, TArray<uint8>& OutValue);
	bool		LoadImageFromFileDialog(FString& OutFilePath);

	EImageFormat GetFileExtension(const FString& FilePath);

	bool CopyFile(const FString& InitialFilePath, const FString& FinalFilePath);
	bool DeleteFile(const FString& FilePath);

	TArray<uint8> CropImage(const TArray<uint8>& SourceRaw, FIntPoint CropStart, FIntPoint CropEnd);
};
