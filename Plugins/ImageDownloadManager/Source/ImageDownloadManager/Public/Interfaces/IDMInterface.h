// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDMDataTypes.h"
#include "IDMInterface.generated.h"

class UIDMComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDMInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for Client-Server messaging with Outer of IDM component class
 */
class IMAGEDOWNLOADMANAGER_API IIDMInterface
{
	GENERATED_BODY()

public:
	// For IDM Component
	virtual void IDM_SendPackage(FIDMPackage& FilePack) {}
	virtual void IDM_SendRequest(uint8 FileId) {}
	virtual void IDM_SendResponse(uint8 FileId) {}

	// For Actor
	virtual bool IDM_GetImageAsByte(uint8 ImageId, TArray<uint8>* OutArray) { return false; }
};
