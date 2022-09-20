// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityTypes.h"
#include "ASCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UASCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ABILITYSYSTEM_API IASCharacterInterface
{
	GENERATED_BODY()

public:
	virtual UObject* IGetIndicator(EIndicatorType Type) const { return nullptr; }
};
