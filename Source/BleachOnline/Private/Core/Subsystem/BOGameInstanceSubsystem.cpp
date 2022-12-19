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

UTexture2D* UBOGameInstanceSubsystem::GetAvatarByIndex(int32 Index)
{
	FString Path = "F:/TestAva.png"; // FPaths::ProjectContentDir() + "TestAva.png";
	return SaveLoadComp->LoadImageToTexture2D(Path);
}

UTexture2D* UBOGameInstanceSubsystem::GetImageFromFile()
{
	FString FilePath;
	if (SaveLoadComp->LoadImageFromFileDialog(FilePath))
	{
		TArray<uint8> FileInByte;
		if (SaveLoadComp->LoadImageAsByte(FilePath, FileInByte))
		{
			return SaveLoadComp->ConvertByteToImage(FileInByte, SaveLoadComp->GetFileExtension(FilePath));
		}
	}
	return nullptr;
}
