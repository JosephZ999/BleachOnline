// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOSaveLoadComponent.h"
#include "Engine\Texture2D.h"
#include "FileHelper.h"

#include "ModuleManager.h"
#include "IImageWrapperModule.h"

#include "DesktopPlatformModule.h"
#include "PlatformFilemanager.h"

#include "Engine\Engine.h"
#include "Engine\GameViewportClient.h"
#include "Widgets\SWindow.h"

UTexture2D* UBOSaveLoadComponent::LoadImageToTexture2D(const FString& ImagePath)
{
	TArray<uint8> ImageReasultData;
	ImageReasultData.Empty();

	// read the binary data of the picture
	FFileHelper::LoadFileToArray(ImageReasultData, *ImagePath);

	// Get a file suffix
	FString Extension = FPaths::GetExtension(ImagePath, false);

	EImageFormat ImageFormat = EImageFormat::PNG;
	if (! Extension.Equals(TEXT("png"), ESearchCase::IgnoreCase)) return nullptr;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");

	// Create a corresponding format picture shell
	TSharedPtr<IImageWrapper> ImageWrapperptr = ImageWrapperModule.CreateImageWrapper(ImageFormat);

	UTexture2D* OutTexture = nullptr;

	bool PtrIsValid = ImageWrapperptr.IsValid();
	if (PtrIsValid && ImageWrapperptr->SetCompressed(ImageReasultData.GetData(), ImageReasultData.GetAllocatedSize()))
	{
		const TArray<uint8>* OutRawData = nullptr;				  // Color data independentled with data
		ImageWrapperptr->GetRaw(ERGBFormat::BGRA, 8, OutRawData); // Extract data according to rules
		int32 Width	 = ImageWrapperptr->GetWidth();
		int32 Height = ImageWrapperptr->GetHeight();
		OutTexture	 = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
		if (OutTexture)
		{
			void* TextureData = OutTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, OutRawData->GetData(), OutRawData->Num()); // Write data * TextureData
			OutTexture->PlatformData->Mips[0].BulkData.Unlock();
			OutTexture->UpdateResource();
			return OutTexture;
		}
	}
	return nullptr;
}

bool UBOSaveLoadComponent::LoadImageAsByte(const FString& FilePath, TArray<uint8>& OutValue)
{
	return FFileHelper::LoadFileToArray(OutValue, *FilePath);
}

UTexture2D* UBOSaveLoadComponent::ConvertByteToImage(const TArray<uint8>& File, EImageFormat Format)
{
	IImageWrapperModule&	  ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
	TSharedPtr<IImageWrapper> ImageWrapperptr	 = ImageWrapperModule.CreateImageWrapper(Format);

	bool PtrIsValid = ImageWrapperptr.IsValid();
	if (PtrIsValid && ImageWrapperptr->SetCompressed(File.GetData(), File.GetAllocatedSize()))
	{
		const TArray<uint8>* OutRawData = nullptr;
		ImageWrapperptr->GetRaw(ERGBFormat::BGRA, 8, OutRawData);
		int32 Width	 = ImageWrapperptr->GetWidth();
		int32 Height = ImageWrapperptr->GetHeight();

		UTexture2D* OutTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
		if (OutTexture)
		{
			void* TextureData = OutTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, OutRawData->GetData(), OutRawData->Num());
			OutTexture->PlatformData->Mips[0].BulkData.Unlock();
			OutTexture->UpdateResource();
			return OutTexture;
		}
	}
	return nullptr;
}

bool UBOSaveLoadComponent::LoadImageFromFileDialog(FString& OutFilePath)
{
	if (! GEngine) return false;

	auto Platform = FDesktopPlatformModule::Get();

	TArray<FString> FileName;
	FString			Title	  = "Select A Image";
	FString			Path	  = "";
	FString			File	  = "";
	FString			FileTypes = "Image|*.png; *.jpg";

	auto Window = GEngine->GameViewport->GetWindow().Get();
	auto Handle = Window->GetNativeWindow().Get()->GetOSWindowHandle();

	if (Platform->OpenFileDialog(Handle, Title, Path, File, FileTypes, 0, FileName))
	{
		OutFilePath = FileName[0];
		return true;
	}
	return false;
}

EImageFormat UBOSaveLoadComponent::GetFileExtension(const FString& FilePath)
{
	FString Extension = FPaths::GetExtension(FilePath, false);
	if (Extension.Equals(TEXT("png"), ESearchCase::IgnoreCase))
	{
		return EImageFormat::PNG;
	}
	else if (Extension.Equals(TEXT("jpg"), ESearchCase::IgnoreCase))
	{
		return EImageFormat::JPEG;
	}
	return EImageFormat::Invalid;
}

bool UBOSaveLoadComponent::CopyFile(const FString& InitialFilePath, const FString& FinalFilePath)
{
	TArray<uint8> File;
	if (FFileHelper::LoadFileToArray(File, *InitialFilePath))
	{
		return FFileHelper::SaveArrayToFile(File, *FinalFilePath);
	}
	return false;
}

bool UBOSaveLoadComponent::DeleteFile(const FString& FilePath)
{
	return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath);
}
