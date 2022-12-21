// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOGameInstanceSubsystem.h"

#include "BOSaveLoadComponent.h"
#include "BOPlayerDataComponent.h"
#include "BOGameDataComponent.h"
#include "Paths.h"
#include "Engine\Texture2D.h"

DEFINE_LOG_CATEGORY_STATIC(LogGISubsystem, All, All);

UBOGameInstanceSubsystem::UBOGameInstanceSubsystem()
{
	SaveLoadComp   = CreateDefaultSubobject<UBOSaveLoadComponent>("SaveLoad");
	PlayerDataComp = CreateDefaultSubobject<UBOPlayerDataComponent>("PlayerData");
	GameDataComp   = CreateDefaultSubobject<UBOGameDataComponent>("GameData");
}

FPlayerProfile UBOGameInstanceSubsystem::GetPlayerProfile()
{
	FPlayerProfile NewProfile;
	NewProfile.Name = FText::FromName("New Player");
	return NewProfile;
}

UTexture2D* UBOGameInstanceSubsystem::GetImageFromFile(FString& OutPath)
{
	if (SaveLoadComp->LoadImageFromFileDialog(OutPath))
	{
		TArray<uint8> FileInByte;
		if (SaveLoadComp->LoadImageAsByte(OutPath, FileInByte))
		{
			return SaveLoadComp->ConvertByteToImage(FileInByte, SaveLoadComp->GetFileExtension(OutPath));
		}
	}
	return nullptr;
}

bool UBOGameInstanceSubsystem::LoadAvatarAsImage(UTexture2D*& Avatar)
{
	TArray<uint8> File;
	const FString Path = FPaths::ProjectSavedDir() + AvatarPath;
	if (SaveLoadComp->LoadImageAsByte(Path, File))
	{
		return Avatar = SaveLoadComp->ConvertByteToImage(File, SaveLoadComp->GetFileExtension(Path));
	}
	return false;
}

bool UBOGameInstanceSubsystem::SetAvatarFromFile(UTexture2D*& Avatar)
{
	FString FilePath;
	Avatar = GetImageFromFile(FilePath);
	if (Avatar)
	{
		return SaveLoadComp->CopyFile(FilePath, FPaths::ProjectSavedDir() + AvatarPath);
	}
	return false;
}
