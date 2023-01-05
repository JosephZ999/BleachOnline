// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "IDMObject.h"
#include "TimerManager.h"
#include "IDMInterface.h"
#include "Engine/World.h"

#define PACK_SIZE 2048

void UIDMObject::Init(EIDMObjectType NewType, uint8 ImageId)
{
	Type = NewType;
	Id	 = ImageId;
}

void UIDMObject::BeginPlay()
{
	switch (Type)
	{
	case EIDMObjectType::Send:
	{
		GetImageAsByte();
		break;
	}
	case EIDMObjectType::Recieve:
	{
		auto OuterInterface = GetOuterInterface();
		OuterInterface->IDM_SendRequest(Id);
		break;
	}
	} // swich end
}

IIDMInterface* UIDMObject::GetOuterInterface()
{
	if (Interface)
	{
		return Interface;
	}
	return Interface = Cast<IIDMInterface>(GetOuter());
}

void UIDMObject::OnPackSent()
{
	if (--FilePartCount == 0)
	{
		// Success
		return;
	}
	SendPack();
}

void UIDMObject::SendPack()
{
	if (FileLastPart >= File.Num() - 1) return;

	auto OuterInterface = GetOuterInterface();
	if (OuterInterface)
	{
		TArray<uint8> FileData;
		const bool	  LastPart		= ! (FileLastPart + (PACK_SIZE) < File.Num() - 1);
		const int32	  PartLastIndex = (LastPart) ? File.Num() - 1 : FileLastPart + (PACK_SIZE);

		for (int32 i = FileLastPart; i < PartLastIndex; i++)
		{
			FileData.Add(File[i]);
		}
		FIDMPackage Pack(Id, FileLastPart, File.Num(), FileData);
		FileLastPart += FileData.Num();
		OuterInterface->IDM_SendPackage(Pack);
	}
}

void UIDMObject::ReceiveFile(const FIDMPackage& FilePack)
{
	if (File.Num() == 0)
	{
		File.Init(0, FilePack.Length);
		FilePartCount = ceil(File.Num() / ((float)PACK_SIZE));
	}

	for (int32 i = FilePack.Part; i < FilePack.Part + FilePack.Data.Num(); i++)
	{
		File[i] = FilePack.Data[i - FilePack.Part];
	}

	if (++FilePartCount == FilePartCount)
	{
		OnLoadingFinish.ExecuteIfBound(this);
	}

	auto OuterInterface = GetOuterInterface();
	if (OuterInterface)
	{
		OuterInterface->IDM_SendResponse(Id);
	}
}

void UIDMObject::GetImageAsByte()
{
	if (! GetWorld()) return;

	auto	   OuterInterface		 = GetOuterInterface();
	const bool FileLoadedSuccessfull = OuterInterface->IDM_GetImageAsByte(Id, &File);
	if (FileLoadedSuccessfull)
	{
		FilePartCount = ceil(File.Num() / ((float)PACK_SIZE));
		SendPack();
		SendPack();
		SendPack();
		return;
	}

	// Repeat this function
	FTimerHandle   GetImageTimer;
	GetWorld()->GetTimerManager().SetTimer(GetImageTimer, this, &ThisClass::GetImageAsByte, 1.f);
}
