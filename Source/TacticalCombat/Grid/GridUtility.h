// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridUtility.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UGridUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static FVector SnapVectorToVector(const FVector& Location, const FVector& GridSize);
	UFUNCTION(BlueprintCallable)
	static bool IsEven(float Value);
};
