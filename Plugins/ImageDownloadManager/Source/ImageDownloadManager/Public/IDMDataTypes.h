// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "IDMDataTypes.generated.h"

UENUM()
enum class EIDMObjectType
{
	SendServer,
	SendClient,
	RecieveServer,
	RecieveClient,
};

UENUM()
enum class EIDMImageType
{
	None,
	Avatar,
};

//-----------------------------------------------------------------//
class UIDMObject;
class UObject;

USTRUCT()
struct FIDMObjectData
{
	GENERATED_BODY()

	FIDMObjectData() {}

	FIDMObjectData(EIDMImageType InId, UIDMObject* InObject, UObject* InOuterObject)
		: Id(InId)
		, Object(InObject)
		, OuterObject(InOuterObject)
	{
	}

	UPROPERTY()
	EIDMImageType Id;

	UPROPERTY()
	UIDMObject* Object;

	UPROPERTY()
	UObject* OuterObject; // for example PlayerState
};

USTRUCT()
struct FIDMPackage
{
	GENERATED_BODY()
};
