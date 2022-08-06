#pragma once

#include "CoreMinimal.h"

struct BOLib
{
	static FORCEINLINE float GetAngleBetweenVectors(const FVector& A, const FVector& B)
	{
		auto Angle = FMath::Acos(				//
			(A.X * B.X + A.Y * B.Y + A.Z * B.Z) //
			/ (sqrt(A.X * A.X + A.Y * A.Y + A.Z * A.Z) * sqrt(B.X * B.X + B.Y * B.Y + B.Z * B.Z)));

		return Angle * 180.f / PI;
	}
};
