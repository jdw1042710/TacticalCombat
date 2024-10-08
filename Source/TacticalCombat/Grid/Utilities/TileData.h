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

UENUM(BlueprintType)
enum class ETileState :uint8
{
	None,
	Hovered,
	Selected,
	Neighbor,
	IsInPath,
	IsDiscovered,
	IsAnalyesd,
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_USTRUCT_BODY()

	FTileData(){}
	FTileData(FIntPoint IndexValue, ETileType TypeValue, FTransform TransformValue, TArray<ETileState> StateValues = TArray<ETileState>());

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETileState> States;
};

UCLASS()
class TACTICALCOMBAT_API UTileDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool IsTileWalkable(ETileType TileType);
};