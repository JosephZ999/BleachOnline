// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ASIndicatorInterface.generated.h"

class UActorComponent;

typedef TMulticastDelegate<void, UActorComponent*, float> FOnChangeDelegate;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UASIndicatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ABILITYSYSTEM_API IASIndicatorInterface
{
	GENERATED_BODY()

public:
	virtual void			  ISetValue(float InValue);
	virtual float			  IGetValue() const;
	virtual float			  IGetPercent() const;
	virtual FOnChangeDelegate IGetDelegateOnChanged() const;
};
