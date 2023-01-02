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

IIDMInterface* UIDMComponent::GetOuterInterface()
{
	if (! OuterInterface)
	{
		OuterInterface = Cast<IIDMInterface>(GetOuter());
	}
	return OuterInterface;
}

void UIDMComponent::CreateObject(bool Send, const EIDMImageType ImageId)
{
	const bool OnServer	  = GetOuterInterface()->IDM_HasAuthority();
	const auto ObjectType = Send ?  EIDMObjectType::Send : EIDMObjectType::Recieve;

	if (auto NewObj = NewObject<UIDMObject>(this, "IDMObj"))
	{
		const FIDMObjectData NewData(ImageId, NewObj, this);
		if (Send)
		{
			Senders.Add(NewData);
		}
		else
		{
			Receivers.Add(NewData);
		}
		NewObj->Init(ObjectType, static_cast<uint8>(ImageId));
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

bool UIDMComponent::IDM_HasAuthority()
{
	if (! GetOwner()) return false;

	if (auto OwnerActor = Cast<AActor>(GetOwner()))
	{
		return OwnerActor->HasAuthority();
	}
	return false;
}

void UIDMComponent::IDM_SendPackage(FIDMPackage FilePack) 
{
	if (IDM_HasAuthority())
	{

	}
	else
	{

	}
}

void UIDMComponent::IDM_SendRequest(uint8 FileId) 
{
	if (IDM_HasAuthority())
	{

	}
	else
	{

	}

}

void UIDMComponent::IDM_SendResponse(int32 FilePart) 
{
	if (IDM_HasAuthority())
	{

	}
	else
	{

	}
}
