#pragma once

#include "BOPlayerDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FPlayerProfile
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FPlayerGameProfile
{
	GENERATED_BODY()

	FPlayerGameProfile() {}

	FPlayerGameProfile(int32 nId, const FPlayerProfile& nProfile)
		: Id(nId)
		, Profile(nProfile)
	{
	}

	UPROPERTY()
	int32 Id;

	UPROPERTY()
	FPlayerProfile Profile;

	// Statuses...
};
