// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "IDMDataTypes.generated.h"

UENUM()
enum class EIDMObjectType : uint8
{
	Send,
	Recieve,
};

UENUM()
enum class EIDMImageType : uint8
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

	FIDMPackage() {}
	FIDMPackage(uint8 InId, int32 InPart, int32 InLength, TArray<uint8>& InData)
		: Id(InId)
		, Part(InPart)
		, Length(InLength)
		, Data(InData)
	{
	}

	uint8		  Id;	  // Image Id
	int32		  Part;	  // Array first elem
	int32		  Length; // Image Length
	TArray<uint8> Data;	  // Image in byte
};
