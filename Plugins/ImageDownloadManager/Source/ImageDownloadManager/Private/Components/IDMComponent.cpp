// Authors MoonDi & JosephZzz for BleachOnline fan game.

#include "IDMComponent.h"
#include "IDMInterface.h"
#include "IDMObject.h"

UIDMComponent::UIDMComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
	const auto ObjectType = Send ? (OnServer ? EIDMObjectType::SendServer : EIDMObjectType::SendClient) //
								 : (OnServer ? EIDMObjectType::RecieveServer : EIDMObjectType::RecieveClient);

	auto NewObj = NewObject<UIDMObject>(GetOuter(), "IDMObj");
	NewObj->SetType(ObjectType);

	FIDMObjectData NewData(ImageId, NewObj, GetOuter());
	Senders.Add(NewData);
}