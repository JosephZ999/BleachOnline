// Authors MoonDi & JosephZzz for BleachOnline fan game.

#pragma once

#include "CoreMinimal.h"

struct BOLib
{
	static float GetAngleBetweenVectors(const FVector& A, const FVector& B);
	static float GetAngleBetweenVectors(const FVector2D& A, const FVector2D& B);
	static uint8 DivideAngle(const FVector2D& PointA, const FVector2D& PointB);
	static uint8 DivideAngle(const FVector2D& PointA, const FVector2D& PointB, uint8 DivisionNum);
	//
};

