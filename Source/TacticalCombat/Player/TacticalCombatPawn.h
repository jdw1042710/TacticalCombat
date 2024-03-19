// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TacticalCombatPawn.generated.h"

UCLASS()
class TACTICALCOMBAT_API ATacticalCombatPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATacticalCombatPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;


	// Zoom
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomSpeed = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomSmoothness = 2.0f;

	float TargetArmLengthDesired;

	void Zoom(float AxisValue);
	void InterpolateZoom(float DeltaTime);

	/// Movement And Rotation
	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSmoothness = 0.2f;

	FVector LocationDesired;

	void MoveVertical(float AxisValue);
	void MoveHorizontal(float AxisValue);

	// Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationDegree = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSmoothness = 0.2f;

	FRotator RotationDesired;

	void RotateClockwise();
	void RotateCounterClockwise();

	void InterpolateLocationAndRotation(float DeltaTime);
};
