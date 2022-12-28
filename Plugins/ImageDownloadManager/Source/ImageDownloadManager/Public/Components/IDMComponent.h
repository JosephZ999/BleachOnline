// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IDMDataTypes.h"
#include "IDMComponent.generated.h"

class IDMObject;
class IIDMInterface;

/**
 * Component for PlayerState class
 * Component will create Objects to send or recieve image
 * Receivers can only be created using senders
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class IMAGEDOWNLOADMANAGER_API UIDMComponent : public UActorComponent
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

private:
	IIDMInterface* GetOuterInterface();
};
