// Fill out your copyright notice in the Description page of Project Settings.


#include "GridUtility.h"

FVector UGridUtility::SnapVectorToVector(const FVector& Location, const FVector& GridSize)
{
	float X = FMath::GridSnap<float>(Location.X, GridSize.X);
	float Y = FMath::GridSnap<float>(Location.Y, GridSize.Y);
	float Z = FMath::GridSnap<float>(Location.Z, GridSize.Z);
	return FVector(X, Y, Z);
}