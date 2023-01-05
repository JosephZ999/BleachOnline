// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IDMDataTypes.h"
#include "IDMObject.generated.h"

class IIDMInterface;

/**
 * Object for sending or receiving a image
 */
UCLASS()
class IMAGEDOWNLOADMANAGER_API UIDMObject : public UObject
{
	GENERATED_BODY()

private:
	EIDMObjectType Type;
	IIDMInterface* Interface;
	uint8		   Id;
	TArray<uint8>  File;
	int32		   FileLastPart = 0;
	int32		   FilePartCount;

public:
	uint8 GetId() { return Id; }

protected:
	void		 Init(EIDMObjectType NewType, uint8 ImageId);
	virtual void BeginPlay();
	void		 OnPackSent();
	void		 ReceiveFile(const FIDMPackage& FilePack);

private:
	IIDMInterface* GetOuterInterface();
	void		   SendPack();
	void		   GetImageAsByte();

private:
	friend class UIDMComponent;
};
