// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IDMDataTypes.h"
#include "IDMObject.generated.h"

/**
 * Object for sending or receiving a image
 */
UCLASS()
class IMAGEDOWNLOADMANAGER_API UIDMObject : public UObject
{
	GENERATED_BODY()

private:
	EIDMObjectType Type;

protected:
	void SetType(EIDMObjectType NewType) { Type = NewType; }

private:
	friend class UIDMComponent;
};
