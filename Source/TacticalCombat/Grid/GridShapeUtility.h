// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridShapeData.h"
#include "Engine/DataTable.h"
#include "GridShapeUtility.generated.h"

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

