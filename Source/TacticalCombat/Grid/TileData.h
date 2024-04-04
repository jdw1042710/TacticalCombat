// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETileType :uint8
{
	None,
	Ground,
	Obstacle,
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_USTRUCT_BODY()

	FTileData(){}
	FTileData(FIntPoint IndexValue, ETileType TypeValue, FTransform TransformValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
};

UCLASS()
class TACTICALCOMBAT_API UTileDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool IsTileWalkable(ETileType TileType);
};