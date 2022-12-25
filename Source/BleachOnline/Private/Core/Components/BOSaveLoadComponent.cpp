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
#include "ImageUtils.h"

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

	if (ImageWrapperptr.IsValid() //
		&& ImageWrapperptr->SetCompressed(File.GetData(), File.GetAllocatedSize()))
	{
		const TArray<uint8>* OutRawData = nullptr;
		ImageWrapperptr->GetRaw(ERGBFormat::BGRA, 8, OutRawData);

		const int32 Width	   = ImageWrapperptr->GetWidth();
		const int32 Height	   = ImageWrapperptr->GetHeight();
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

bool UBOSaveLoadComponent::CropImage(const TArray<uint8>& File, const FString& Path, const FIntPoint& CropStart, const FIntPoint& CropEnd)
{
	IImageWrapperModule&	  ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
	TSharedPtr<IImageWrapper> ImageWrapperptr	 = ImageWrapperModule.CreateImageWrapper(GetFileExtension(Path));

	if (ImageWrapperptr.IsValid() //
		&& ImageWrapperptr->SetCompressed(File.GetData(), File.GetAllocatedSize()))
	{
		const TArray<uint8>* OutRawData = nullptr;
		ImageWrapperptr->GetRaw(ERGBFormat::BGRA, 8, OutRawData);
		const FIntPoint Size = FIntPoint(ImageWrapperptr->GetWidth(), ImageWrapperptr->GetHeight());

		checkf(CropStart.X < CropEnd.X && CropStart.Y < CropEnd.Y, TEXT("Incorrect Crop Region"));
		checkf(CropStart.X >= 0 && CropStart.Y >= 0 && CropEnd.X <= Size.X && CropEnd.Y <= Size.Y, TEXT("Incorrect Crop Region"));

		const auto NewRaw = CropRaw(*OutRawData, Size, CropStart, CropEnd);

		ImageWrapperptr->SetRaw(		  //
			NewRaw.GetData(),			  //
			NewRaw.Num(),				  //
			abs(CropStart.X - CropEnd.X), //
			abs(CropStart.Y - CropEnd.Y), //
			ERGBFormat::BGRA, 8);

		UE_LOG(LogTemp, Display, TEXT("-- Warning -- %i"), ImageWrapperptr->GetCompressed().Num());

		return FFileHelper::SaveArrayToFile(ImageWrapperptr->GetCompressed(), *Path);
	}
	return false;
}

TArray<uint8> UBOSaveLoadComponent::CropRaw(
	const TArray<uint8>& SourceRaw, const FIntPoint& SourceImageSize, const FIntPoint& CropStart, const FIntPoint& CropEnd)
{
	const FIntPoint NewSize = FIntPoint(abs(CropStart.X - CropEnd.X), abs(CropStart.Y - CropEnd.Y));

	TArray<uint8> NRaw;
	NRaw.Init(255, NewSize.X * NewSize.Y * 4);

	for (int32 y = CropStart.Y + 1; y <= CropEnd.Y; y++)
	{
		for (int32 x = CropStart.X + 1; x <= CropEnd.X; x++)
		{
			int32 a = (NewSize.X * (y - CropStart.Y)) - (NewSize.X - (x - CropStart.X)) - 1; // Cropped index
			int32 b = (SourceImageSize.X * y) - (SourceImageSize.X - x) - 1;				 // Source Index

			if (a < 0 || b < 0) continue;

			a *= 4;
			b *= 4;

			NRaw[a]		= SourceRaw[b];
			NRaw[a + 1] = SourceRaw[b + 1];
			NRaw[a + 2] = SourceRaw[b + 2];
			NRaw[a + 2] = SourceRaw[b + 2];
		}
	}
	return NRaw;
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
		OutFilePath	  = FileName[0];
		auto FileSize = FPlatformFileManager::Get().GetPlatformFile().FileSize(*OutFilePath);
		return (FileSize < 512000 * 12 && FileSize > 0) ? true : false;
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
