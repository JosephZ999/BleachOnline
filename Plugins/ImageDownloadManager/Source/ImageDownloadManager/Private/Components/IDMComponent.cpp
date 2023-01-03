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

void UIDMComponent::CreateObject(bool Send, const EIDMImageType ImageId)
{
	const auto ObjectType = Send ? EIDMObjectType::Send : EIDMObjectType::Recieve;
	const auto Id		  = static_cast<uint8>(ImageId);
	if (auto NewObj = NewObject<UIDMObject>(this, "IDMObj"))
	{
		const FIDMObjectData NewData(ImageId, NewObj, this);
		auto*				 ObjArr = (Send) ? &Senders : &Receivers;
		ObjArr->Add(NewData);
		NewObj->Init(ObjectType, Id, GetImageAsByte(Id));
		NewObj->BeginPlay();
	}
}

bool UIDMComponent::FindObject(bool SendObj, EIDMImageType ImageId, const UObject* OuterObject, UIDMObject* OutObject)
{
	for (const auto& Data : (SendObj ? Senders : Receivers))
	{
		if (OuterObject != Data.OuterObject) continue;

		if (ImageId != Data.Id) continue;

		return OutObject = Data.Object;
	}
	return OutObject = nullptr;
}

void UIDMComponent::SendRequest(uint8 Id)
{
	UIDMObject* Obj = nullptr;
	if (FindObject(true, static_cast<EIDMImageType>(Id), this, Obj) && Obj)
	{
	}
	else
	{
		CreateObject(true, static_cast<EIDMImageType>(Id));
	}
}

void UIDMComponent::SendFile(FIDMPackage FilePack)
{
	UIDMObject* Obj = nullptr;
	if (FindObject(false, static_cast<EIDMImageType>(FilePack.Id), this, Obj) && Obj)
	{
		Obj->ReceiveFile(FilePack);
	}
	// Error
}

void UIDMComponent::IDM_SendPackage(FIDMPackage FilePack)
{
	if (HasAuthority())
	{
	}
	else
	{
	}
}

void UIDMComponent::IDM_SendRequest(uint8 FileId)
{
	if (HasAuthority())
	{
	}
	else
	{
	}
}

void UIDMComponent::IDM_SendResponse(int32 FilePart)
{
	if (HasAuthority())
	{
	}
	else
	{
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

TArray<uint8>* UIDMComponent::GetImageAsByte(uint8 Id)
{
	if (! GetOwner()) return nullptr;

	if (const auto OwnerInterface = Cast<IIDMInterface>(GetOwner()))
	{
		TArray<uint8> Arr;
		if (OwnerInterface->IDM_GetImageAsByte(Id, &Arr))
		{
			return &Arr;
		}
	}
	return nullptr;
}
