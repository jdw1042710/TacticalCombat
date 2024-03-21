// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraTab.generated.h"

/**
 * 
 */
class USpinBoxWithLabel;
UCLASS()
class TACTICALCOMBAT_API UCameraTab : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	class ATacticalCombatPawn* Player;

	// movement
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USpinBoxWithLabel* MovementSpeedSpinBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* MovementSmoothnessSpinBox;

	// rotation
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* RotationSpeedSpinBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* RotationSmoothnessSpinBox;

	// Zoom
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* ZoomSpeedSpinBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* ZoomSmoothnessSpinBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* ZoomMinSpinBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USpinBoxWithLabel* ZoomMaxSpinBox;
};
