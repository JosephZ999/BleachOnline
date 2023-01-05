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
	if (SaveLoadComp->LoadImageAsByte(Path + ".png", File))
	{
		return Avatar = SaveLoadComp->ConvertByteToImage(File, SaveLoadComp->GetFileExtension(Path + ".png"));
	}
	else if (SaveLoadComp->LoadImageAsByte(Path + ".jpg", File))
	{
		return Avatar = SaveLoadComp->ConvertByteToImage(File, SaveLoadComp->GetFileExtension(Path + ".jpg"));
	}
	return false;
}

bool UBOGameInstanceSubsystem::SetAvatarFromFile(UTexture2D*& Avatar)
{
	FString FilePath;
	Avatar = GetImageFromFile(FilePath);
	if (Avatar)
	{
		auto Ex = SaveLoadComp->GetFileExtension(FilePath);
		switch (Ex)
		{
		case EImageFormat::PNG:
		{
			SaveLoadComp->DeleteFile(FPaths::ProjectSavedDir() + AvatarPath + ".jpg");
			return SaveLoadComp->CopyFile(FilePath, FPaths::ProjectSavedDir() + AvatarPath + ".png");
		}
		case EImageFormat::JPEG:
		{
			SaveLoadComp->DeleteFile(FPaths::ProjectSavedDir() + AvatarPath + ".png");
			return SaveLoadComp->CopyFile(FilePath, FPaths::ProjectSavedDir() + AvatarPath + ".jpg");
		}
		}
	}
	return false;
}

bool UBOGameInstanceSubsystem::GetAvatarRaw(TArray<uint8>& Avatar)
{
	const FString Path = FPaths::ProjectSavedDir() + AvatarPath;
	if (SaveLoadComp->LoadImageRaw(Path + ".png", Avatar) || SaveLoadComp->LoadImageAsByte(Path + ".jpg", Avatar))
	{
		return true;
	}
	return false;
}

bool UBOGameInstanceSubsystem::RawToTexture2D(const TArray<uint8>& ImageRaw, int32 Width, int32 Height, UTexture2D *& OutTexture)
{
	return OutTexture = SaveLoadComp->ConvertRawToImage(ImageRaw, Width, Height);
}

bool UBOGameInstanceSubsystem::CropAvatar(const FIntPoint& CropStart, const FIntPoint& CropEnd)
{
	TArray<uint8> File;
	const FString Path = FPaths::ProjectSavedDir() + AvatarPath;
	if (SaveLoadComp->LoadImageAsByte(Path + ".png", File))
	{
		return SaveLoadComp->CropImage(File, Path + ".png", CropStart, CropEnd);
	}
	else if (SaveLoadComp->LoadImageAsByte(Path + ".jpg", File))
	{
		return SaveLoadComp->CropImage(File, Path + ".jpg", CropStart, CropEnd);
	}
	return false;
}
