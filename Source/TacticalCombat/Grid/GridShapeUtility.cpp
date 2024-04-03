// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridShapeUtility.h"

UDataTable* UGridShapeUtility::GridShapeDataTable = nullptr;

UGridShapeUtility::UGridShapeUtility()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObject(TEXT("/Game/TacticalCombat/Arts/Grids/DT_GridShapeData.DT_GridShapeData"));
	if (ensure(DataTableObject.Succeeded())) 
	{
		UGridShapeUtility::GridShapeDataTable = DataTableObject.Object;
	}
}

FGridShapeData* UGridShapeUtility::GetShapeData(EGridShape Shape)
{
	FName ShapeName = GetGridShapeName(Shape);
	if (UGridShapeUtility::GridShapeDataTable == nullptr) return nullptr;
	return UGridShapeUtility::GridShapeDataTable->FindRow<FGridShapeData>(ShapeName, ShapeName.ToString());
}

FName UGridShapeUtility::GetGridShapeName(EGridShape Shape)
{
	const UEnum* GridShapeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGridShape"), true);
	if (GridShapeEnum == nullptr) return FName("Invalid");
	return *GridShapeEnum->GetDisplayNameTextByValue((int32)Shape).ToString();
}