// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "IDMComponent.h"
#include "IDMInterface.h"
#include "IDMObject.h"
#include "GameFramework/Actor.h"

UIDMComponent::UIDMComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicates						  = true;
}

void UIDMComponent::UploadImage(uint8 ImageId)
{
	CreateObject(true, ImageId);
}

void UIDMComponent::IDM_SendPackage(FIDMPackage& FilePack)
{
	return (HasAuthority()) ? SendPackageClient(FilePack) : SendPackageServer(FilePack);
}

void UIDMComponent::IDM_SendRequest(uint8 FileId)
{
	return (HasAuthority()) ? SendRequestClient(FileId) : SendRequestServer(FileId);
}

void UIDMComponent::IDM_SendResponse(uint8 FileId)
{
	return (HasAuthority()) ? SendResponseClient(FileId) : SendResponseServer(FileId);
}

bool UIDMComponent::IDM_GetImageAsByte(uint8 ImageId, TArray<uint8>* OutArray)
{
	auto OwnerActor = Cast<IIDMInterface>(GetOwner());
	if (OwnerActor)
	{
		return OwnerActor->IDM_GetImageAsByte(ImageId, OutArray);
	}
	return false;
}

void UIDMComponent::SendPackageClient_Implementation(FIDMPackage FilePack)
{
	SendFile(FilePack);
}

void UIDMComponent::SendPackageServer_Implementation(FIDMPackage FilePack)
{
	SendFile(FilePack);
}

void UIDMComponent::SendRequestClient_Implementation(uint8 FileId)
{
	SendRequest(FileId);
}

void UIDMComponent::SendRequestServer_Implementation(uint8 FileId)
{
	SendRequest(FileId);
}

void UIDMComponent::SendResponseClient_Implementation(uint8 FileId)
{
	SendResponse(FileId);
}

void UIDMComponent::SendResponseServer_Implementation(uint8 FileId)
{
	SendResponse(FileId);
}

void UIDMComponent::CreateObject(bool Send, const uint8 ImageId)
{
	const auto ObjectType = Send ? EIDMObjectType::Send : EIDMObjectType::Recieve;
	const auto Id		  = ImageId;
	if (auto NewObj = NewObject<UIDMObject>(this, "IDMObj"))
	{
		NewObj->OnLoadingFinish.BindUObject(this, &ThisClass::OnLoadingFinished);
		const FIDMObjectData NewData(ImageId, NewObj, this);
		auto*				 ObjArr = (Send) ? &Senders : &Receivers;
		ObjArr->Add(NewData);
		NewObj->Init(ObjectType, Id);
		NewObj->BeginPlay();
	}
}

bool UIDMComponent::FindObject(bool SendObj, uint8 ImageId, const UObject* OuterObject, UIDMObject* OutObject)
{
	for (const auto& Data : (SendObj ? Senders : Receivers))
	{
		if (OuterObject != Data.OuterObject) continue;

		if (ImageId != Data.Id) continue;

		return OutObject = Data.Object;
	}
	return OutObject = nullptr;
}

void UIDMComponent::SendFile(const FIDMPackage& FilePack)
{
	UIDMObject* Obj = nullptr;
	if (FindObject(false, FilePack.Id, this, Obj))
	{
		Obj->ReceiveFile(FilePack);
	}
}

void UIDMComponent::SendRequest(uint8 Id)
{
	CreateObject(true, Id);
}

void UIDMComponent::SendResponse(uint8 Id)
{
	UIDMObject* Obj = nullptr;
	if (FindObject(false, Id, this, Obj))
	{
		Obj->OnPackSent();
	}
}

bool UIDMComponent::HasAuthority()
{
	if (! GetOwner()) return false;

	if (auto OwnerActor = Cast<AActor>(GetOwner()))
	{
		return OwnerActor->HasAuthority();
	}
	return false;
}

void UIDMComponent::OnLoadingFinished(UIDMObject* Obj)
{
	if (Obj->GetType() == EIDMObjectType::Recieve)
	{
		if (auto OwnerActor = Cast<IIDMInterface>(GetOwner()))
		{
			OwnerActor->IDM_SetImage(Obj->GetId(), Obj->GetFile());
		}
	}
	Obj->ConditionalBeginDestroy();
}
