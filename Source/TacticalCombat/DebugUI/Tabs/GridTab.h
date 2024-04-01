// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GridTab.generated.h"

/**
 * 
 */
class USpinBoxWithLabel;
class USpinBoxWithLabelVector2D;
class USpinBoxWithLabelVector;
class UCheckBox;
class UTextBlock;
UCLASS()
class TACTICALCOMBAT_API UGridTab : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void NativeConstruct() override;

	class AGrid* Grid;

	class ALevelLoader* LevelLoader;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UComboBoxString* EnvironmentComboBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UComboBoxString* GridShapeComboBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* ReGenDelaySpinBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabelVector* LocationSpinBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabelVector2D* TileCountSpinBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabelVector* TileSizeSpinBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* GridOffsetSpinBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* BoundsDebugLineCheckBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* CenterDebugLineCheckBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CenterDebugLineTextBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* BottomLeftDebugLineCheckBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* BottomLeftDebugLineTextBlock;

	UFUNCTION()
	void OnEnvironmentComboBoxChanged(FString Value, ESelectInfo::Type seltype);

	UFUNCTION()
	void OnGridShapeComboBoxChanged(FString Value, ESelectInfo::Type seltype);

	UFUNCTION()
	void OnLocationSpinBoxValueChanged(FVector Value);

	UFUNCTION()
	void OnTileCountSpinBoxValueChanged(FVector2D Value);

	UFUNCTION()
	void OnTileSizeSpinBoxValueChanged(FVector Value);

	UFUNCTION()
	void OnGridOffsetSpinBoxValueChanged(float Value);

	void TryUpdateGrid(float DeltaTime);
	void UpdateGrid();
	bool bReGenFlag = false;
	float ReGenCoolTime = 0;

	void DrawDebugLines(float DeltaTime);
};
