// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTab.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ComboBoxString.h"
#include "../../Grid/Grid.h"
#include "../SubWidgets/SpinBoxWithLabel.h"
#include "../SubWidgets/SpinBoxWithLabelVector2D.h"
#include "../SubWidgets/SpinBoxWithLabelVector.h"

void UGridTab::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (ReGenCoolTime > 0)
	{
		ReGenCoolTime -= InDeltaTime;
		ReGenCoolTime = FMath::Clamp(ReGenCoolTime, .0f, ReGenCoolTime);
	}
	else if (bReGenFlag)
	{
		UpdateGrid();
		ReGenCoolTime = ReGenDelaySpinBox->GetValue();
		bReGenFlag = false;
	}
}

void UGridTab::NativeConstruct()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), Actors);
	if (Actors.Num() != 0)
	{
		Grid = Cast<AGrid>(Actors[0]);
	}

	if (Grid != nullptr)
	{
		FName MethodName;

		GridShapeComboBox->OnSelectionChanged.AddDynamic(this, &UGridTab::OnGridShapeComboBoxChanged);


		LocationSpinBox->SetValue(Grid->GetLocation());
		TileCountSpinBox->SetValue(Grid->GetTileCount());
		TileSizeSpinBox->SetValue(Grid->GetTileSize());

		MethodName = "OnLocationSpinBoxValueChanged";
		LocationSpinBox->AddValueChangedEvent(this, MethodName);

		MethodName = "OnTileCountSpinBoxValueChanged";
		TileCountSpinBox->AddValueChangedEvent(this, MethodName);

		MethodName = "OnTileSizeSpinBoxValueChanged";
		TileSizeSpinBox->AddValueChangedEvent(this, MethodName);
		

	}
	Super::NativeConstruct();
}

void UGridTab::OnGridShapeComboBoxChanged(FString Value, ESelectInfo::Type seltype)
{
	if (Grid == nullptr) return;
	bReGenFlag = true;
}

void UGridTab::OnLocationSpinBoxValueChanged(FVector Value)
{
	if (Grid == nullptr) return;
	bReGenFlag = true;
}

void UGridTab::OnTileCountSpinBoxValueChanged(FVector2D Value)
{
	if (Grid == nullptr) return;
	bReGenFlag = true;
}

void UGridTab::OnTileSizeSpinBoxValueChanged(FVector Value)
{
	if (Grid == nullptr) return;
	bReGenFlag = true;
}

void UGridTab::UpdateGrid()
{
	if (Grid == nullptr) return;
	EGridShape Shape = (EGridShape)GridShapeComboBox->GetSelectedIndex();
	Grid->SpawnGrid(LocationSpinBox->GetValue(), TileSizeSpinBox->GetValue(), TileCountSpinBox->GetValue(), Shape);
}
