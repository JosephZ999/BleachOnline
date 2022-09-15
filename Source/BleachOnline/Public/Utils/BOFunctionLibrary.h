// Authors MoonDi & JosephZzz for Bleach Online fan game

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"

struct BOLib
{
	static FORCEINLINE float GetAngleBetweenVectors(const FVector& A, const FVector& B);
	static FORCEINLINE float GetAngleBetweenVectors(const FVector2D& A, const FVector2D& B);
	static FORCEINLINE uint8 DivideAngle(const FVector2D& PointA, const FVector2D& PointB);
	static FORCEINLINE uint8 DivideAngle(const FVector2D& PointA, const FVector2D& PointB, uint8 DivisionNum);
};

float BOLib::GetAngleBetweenVectors(const FVector& A, const FVector& B)
{
	auto Angle = FMath::Acos(				//
		(A.X * B.X + A.Y * B.Y + A.Z * B.Z) //
		/ (sqrt(A.X * A.X + A.Y * A.Y + A.Z * A.Z) * sqrt(B.X * B.X + B.Y * B.Y + B.Z * B.Z)));

	return Angle * 180.f / PI;
}

float BOLib::GetAngleBetweenVectors(const FVector2D& A, const FVector2D& B)
{
	return UKismetMathLibrary::FindLookAtRotation(FVector(A, 0.f), FVector(B, 0.f)).Yaw;
}

uint8 BOLib::DivideAngle(const FVector2D& PointA, const FVector2D& PointB)
{
	const float Angle = GetAngleBetweenVectors(PointA, PointB) + 225.f;
	float		Remainder;
	const uint8 outValue = UKismetMathLibrary::FMod(Angle, 90.f, Remainder);
	return (outValue == 0) ? 4 : outValue;
}

uint8 BOLib::DivideAngle(const FVector2D& PointA, const FVector2D& PointB, uint8 DivisionNum)
{
	if (DivisionNum == 0) return 0;

	const float Divisor = 360.f / DivisionNum;
	const float Dividend = GetAngleBetweenVectors(PointA, PointB) + 180.f + Divisor / 2.f;

	float Remainder;
	uint8 OutValue = UKismetMathLibrary::FMod(Dividend, Divisor, Remainder);
	return OutValue == 0 ? DivisionNum : OutValue;
}