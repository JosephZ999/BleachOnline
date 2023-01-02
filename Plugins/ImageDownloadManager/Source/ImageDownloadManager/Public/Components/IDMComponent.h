// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IDMInterface.h"
#include "IDMComponent.generated.h"

class IDMObject;
class IIDMInterface;

/**
 * Component for PlayerState class
 * Component will create Objects to send or recieve image
 * Receivers can only be created using senders
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class IMAGEDOWNLOADMANAGER_API UIDMComponent : public UActorComponent, public IIDMInterface
{
	GENERATED_BODY()

public:
	UIDMComponent();

private:
	UPROPERTY()
	TArray<FIDMObjectData> Senders;

	UPROPERTY()
	TArray<FIDMObjectData> Receivers;

	IIDMInterface* OuterInterface;

public:
	void CreateObject(bool Send, const EIDMImageType ImageId);
	bool FindObject(bool SendObj, EIDMImageType ImageId, const UObject* OuterObject, UIDMObject* OutObject);
	void SendRequest(uint8 Id);
	void SendFile(FIDMPackage FilePack);

	// Interface
	virtual bool IDM_HasAuthority() override;
	virtual void IDM_SendPackage(FIDMPackage FilePack) override;
	virtual void IDM_SendRequest(uint8 FileId) override;
	virtual void IDM_SendResponse(int32 FilePart) override;

private:
	IIDMInterface* GetOuterInterface();
};
