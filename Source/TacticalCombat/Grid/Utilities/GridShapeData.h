// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GridShapeData.generated.h"


UENUM(BlueprintType)
enum class EGridShape :uint8
{
	None,
	Square,
	Hexagon,
	Triangle
};

USTRUCT(BlueprintType)
struct FGridShapeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Size = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* FlatMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* FlatMaterial;
};

UCLASS()
class TACTICALCOMBAT_API UGridShapeUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGridShapeUtility();

	static FGridShapeData* GetShapeData(EGridShape Shape);
	static FName GetGridShapeName(EGridShape Shape);

private:
	static UDataTable* GridShapeDataTable;
};