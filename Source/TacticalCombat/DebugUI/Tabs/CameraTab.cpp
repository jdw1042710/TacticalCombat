// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTab.h"
#include "Kismet/GameplayStatics.h"
#include "../../Player/TacticalCombatPawn.h"
#include "../SubWidgets/SpinBoxWithLabel.h"

void UCameraTab::NativeConstruct()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Pawn != nullptr) 
	{
		Player = Cast<ATacticalCombatPawn>(Pawn);
	}
	if (Player != nullptr) 
	{
		FName MethodName;

		MethodName = "SetMovementSpeed";
		MovementSpeedSpinBox->AddValueChangedEvent(Player, MethodName);
		MovementSpeedSpinBox->SetValue(Player->GetMovementSpeed());
		MethodName = "SetMovementSmoothness";
		MovementSmoothnessSpinBox->AddValueChangedEvent(Player, MethodName);
		MovementSmoothnessSpinBox->SetValue(Player->GetMovementSmoothness());


		MethodName = "SetRotationDegree";
		RotationSpeedSpinBox->AddValueChangedEvent(Player, MethodName);
		RotationSpeedSpinBox->SetValue(Player->GetRotationDegree());
		MethodName = "SetRotationSmoothness";
		RotationSmoothnessSpinBox->AddValueChangedEvent(Player, MethodName);
		RotationSmoothnessSpinBox->SetValue(Player->GetRotationSmoothness());

		MethodName = "SetZoomSpeed";
		ZoomSpeedSpinBox->AddValueChangedEvent(Player, MethodName);
		ZoomSpeedSpinBox->SetValue(Player->GetZoomSpeed());
		MethodName = "SetZoomSmoothness";
		ZoomSmoothnessSpinBox->AddValueChangedEvent(Player, MethodName);
		ZoomSmoothnessSpinBox->SetValue(Player->GetZoomSmoothness());
		MethodName = "SetZoomMin";
		ZoomMinSpinBox->AddValueChangedEvent(Player, MethodName);
		ZoomMinSpinBox->SetValue(Player->GetZoomMin());
		MethodName = "SetZoomMax";
		ZoomMaxSpinBox->AddValueChangedEvent(Player, MethodName);
		ZoomMaxSpinBox->SetValue(Player->GetZoomMax());

	}
	Super::NativeConstruct();
}
