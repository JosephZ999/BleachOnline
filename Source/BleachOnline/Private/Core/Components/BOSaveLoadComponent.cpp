// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "BOSaveLoadComponent.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "FileHelper.h"
#include "ModuleManager.h"
#include "Engine\Texture2D.h"

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
