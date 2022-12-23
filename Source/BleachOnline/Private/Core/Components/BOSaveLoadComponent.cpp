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

	bool PtrIsValid = ImageWrapperptr.IsValid();
	if (PtrIsValid && ImageWrapperptr->SetCompressed(File.GetData(), File.GetAllocatedSize()))
	{
		const TArray<uint8>* OutRawData = nullptr;
		ImageWrapperptr->GetRaw(ERGBFormat::BGRA, 8, OutRawData);

		auto NewRaw = CropImage(*OutRawData, FIntPoint(400, 400), FIntPoint(800, 800));
		ImageWrapperptr->SetRaw(NewRaw.GetData(), 640000, 400, 400, ERGBFormat::BGRA, 8);

		// return FImageUtils::CreateTexture2D(								  //
		//	400, 400,														  //
		//	CropImage(*OutRawData, FIntPoint(400, 400), FIntPoint(800, 800)), //
		//	GetOuter(),														  //
		//	"Img", EObjectFlags::RF_Transient, FCreateTexture2DParameters());

		int32 Width	 = ImageWrapperptr->GetWidth();
		int32 Height = ImageWrapperptr->GetHeight();

		UE_LOG(LogTemp, Display, TEXT("-- Warning -- %i - %i"), Width, Height);

		UTexture2D* OutTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
		if (OutTexture)
		{
			void* TextureData = OutTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, NewRaw.GetData(), NewRaw.Num());
			OutTexture->PlatformData->Mips[0].BulkData.Unlock();
			OutTexture->UpdateResource();
			return OutTexture;
		}
	}
	return nullptr;
}

TArray<uint8> UBOSaveLoadComponent::CropImage(const TArray<uint8>& SourceRaw, FIntPoint CropStart, FIntPoint CropEnd)
{
	TArray<uint8> NRaw;
	NRaw.Init(255, 400 * 400 * 4);

	// TArray<FColor> NRaw;
	// NRaw.Init(FColor(255, 255, 255, 255), 400 * 400 * 4);

	for (int32 y = CropStart.Y; y <= CropEnd.Y; y++)
	{
		for (int32 x = CropStart.X; x <= CropEnd.X; x++)
		{
			// if (x - CropStart.X <= 0 || y - CropEnd.Y <= 0) continue;

			// int32 a = (400 * y) - (400 - x); // Cropped index
			int32 a = (400 * (y - CropStart.Y)) - (400 - (x - CropStart.X)); // Cropped index
			int32 b = (800 * y) - (800 - x);								 // Source Index
			--a;
			--b;

			//if ((b % 4) != 0) continue;
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
		return (FileSize < 256000 * 4 && FileSize > 0) ? true : false;
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
