// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDMInterface.generated.h"

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
	virtual bool IDM_HasAuthority() = 0;
};
