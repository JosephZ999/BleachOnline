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

 * Owner must implement the interface function "GetImage"
 * And On begin play must call function of this class "UploadImage"
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
	void UploadImage(uint8 ImageId);

	// Interface
	virtual void IDM_SendPackage(FIDMPackage& FilePack) override;
	virtual void IDM_SendRequest(uint8 FileId) override;
	virtual void IDM_SendResponse(uint8 FileId) override;
	virtual bool IDM_GetImageAsByte(uint8 ImageId, TArray<uint8>* OutArray) override;

public:
	// Client-Server Functions
	UFUNCTION(Client, Reliable)
	void SendPackageClient(FIDMPackage FilePack);
	void SendPackageClient_Implementation(FIDMPackage FilePack);

	UFUNCTION(Server, Reliable)
	void SendPackageServer(FIDMPackage FilePack);
	void SendPackageServer_Implementation(FIDMPackage FilePack);

	UFUNCTION(Client, Reliable)
	void SendRequestClient(uint8 FileId);
	void SendRequestClient_Implementation(uint8 FileId);

	UFUNCTION(Server, Reliable)
	void SendRequestServer(uint8 FileId);
	void SendRequestServer_Implementation(uint8 FileId);

	UFUNCTION(Client, Reliable)
	void SendResponseClient(uint8 FileId);
	void SendResponseClient_Implementation(uint8 FileId);

	UFUNCTION(Server, Reliable)
	void SendResponseServer(uint8 FileId);
	void SendResponseServer_Implementation(uint8 FileId);

private:
	void CreateObject(bool Send, const uint8 ImageId);
	bool FindObject(bool SendObj, uint8 ImageId, const UObject* OuterObject, UIDMObject* OutObject);
	void SendFile(const FIDMPackage& FilePack);
	void SendRequest(uint8 Id);
	void SendResponse(uint8 Id);

	bool HasAuthority();
};
