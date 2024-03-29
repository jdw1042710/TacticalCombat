// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTab.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "../../Grid/Grid.h"
#include "../SubWidgets/SpinBoxWithLabel.h"
#include "../SubWidgets/SpinBoxWithLabelVector2D.h"
#include "../SubWidgets/SpinBoxWithLabelVector.h"
#include "../../Level/LevelLoader.h"	
#include "DrawDebugHelpers.h"

void UGridTab::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	TryUpdateGrid(InDeltaTime);
	DrawDebugLines(InDeltaTime);
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

	LevelLoader = GetWorld()->SpawnActor<ALevelLoader>();
	EnvironmentComboBox->OnSelectionChanged.AddDynamic(this, &UGridTab::OnEnvironmentComboBoxChanged);
	LevelLoader->LoadLevel(FName(EnvironmentComboBox->GetSelectedOption()));
	Super::NativeConstruct();
}

void UGridTab::OnEnvironmentComboBoxChanged(FString Value, ESelectInfo::Type seltype)
{
	LevelLoader->LoadLevel(FName(Value));
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

void UGridTab::TryUpdateGrid(float DeltaTime)
{
	if (ReGenCoolTime > 0)
	{
		ReGenCoolTime -= DeltaTime;
		ReGenCoolTime = FMath::Clamp(ReGenCoolTime, .0f, ReGenCoolTime);
	}
	else if (bReGenFlag)
	{
		UpdateGrid();
		ReGenCoolTime = ReGenDelaySpinBox->GetValue();
		bReGenFlag = false;
	}
}

void UGridTab::UpdateGrid()
{
	if (Grid == nullptr) return;
	EGridShape Shape = (EGridShape)GridShapeComboBox->GetSelectedIndex();
	Grid->SpawnGrid(LocationSpinBox->GetValue(), TileSizeSpinBox->GetValue(), TileCountSpinBox->GetValue(), Shape);
}

void UGridTab::DrawDebugLines(float DeltaTime)
{
	if (BoundsDebugLineCheckBox->IsChecked())
	{
		DrawDebugBox(
			GetWorld(),
			Grid->GetCenterLocation(),
			Grid->GetCenterLocation() - Grid->GetBottomLeftLocation(),
			FColor::Yellow,
			false,
			-1.f,
			0,
			5);
	}

	CenterDebugLineTextBlock->SetText(Grid->GetCenterLocation().ToText());
	if (CenterDebugLineCheckBox->IsChecked()) 
	{
		DrawDebugSphere(
			GetWorld(), 
			Grid->GetLocation(), 
			50.0f, 
			3, 
			FColor::Yellow, 
			false, 
			-1.f, 
			0, 
			3);
		DrawDebugSphere(
			GetWorld(), 
			Grid->GetCenterLocation(),
			50.0f, 
			3, 
			FColor::Red,
			false,
			-1.f,
			0,
			3);
	}

	BottomLeftDebugLineTextBlock->SetText(Grid->GetBottomLeftLocation().ToText());
	if (BottomLeftDebugLineCheckBox->IsChecked())
	{
		DrawDebugSphere(
			GetWorld(),
			Grid->GetBottomLeftLocation(),
			50.0f, 
			3, 
			FColor::Red,
			false,
			-1.f, 
			0, 
			3);
	}
}
