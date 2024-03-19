// Fill out your copyright notice in the Description page of Project Settings.


#include "TacticalCombatPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATacticalCombatPawn::ATacticalCombatPawn()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(Root);
	SpringArm->SetupAttachment(Root);
	Camera->SetupAttachment(SpringArm);

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATacticalCombatPawn::BeginPlay()
{
	Super::BeginPlay();
	TargetArmLengthDesired = SpringArm->TargetArmLength;
	LocationDesired = GetActorLocation();
	RotationDesired = GetActorRotation();
}

// Called every frame
void ATacticalCombatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InterpolateZoom(DeltaTime);
	InterpolateLocationAndRotation(DeltaTime);
}

// Called to bind functionality to input
void ATacticalCombatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis
		(
			"Zoom",
			this,
			&ATacticalCombatPawn::Zoom
		);

		PlayerInputComponent->BindAxis
		(
			"MoveVertical",
			this,
			&ATacticalCombatPawn::MoveVertical
		);

		PlayerInputComponent->BindAxis
		(
			"MoveHorizontal",
			this,
			&ATacticalCombatPawn::MoveHorizontal
		);

		PlayerInputComponent->BindAction
		(
			"RotateClockwise",
			IE_Pressed,
			this,
			&ATacticalCombatPawn::RotateClockwise
		);

		PlayerInputComponent->BindAction
		(
			"RotateCounterClockwise",
			IE_Pressed,
			this,
			&ATacticalCombatPawn::RotateCounterClockwise
		);
	}

}

void ATacticalCombatPawn::Zoom(float AxisValue)
{
	TargetArmLengthDesired += -AxisValue * ZoomSpeed;
}

void ATacticalCombatPawn::InterpolateZoom(float DeltaTime)
{
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLengthDesired, DeltaTime, 1/ZoomSmoothness);
}

void ATacticalCombatPawn::MoveVertical(float AxisValue)
{
	LocationDesired += GetActorForwardVector() * MovementSpeed * AxisValue;
}

void ATacticalCombatPawn::MoveHorizontal(float AxisValue)
{
	LocationDesired += GetActorRightVector() * MovementSpeed * AxisValue;
}

void ATacticalCombatPawn::RotateClockwise()
{
	RotationDesired += FRotator(0, 1, 0) * -RotationDegree;
}

void ATacticalCombatPawn::RotateCounterClockwise()
{
	RotationDesired += FRotator(0, 1, 0) * RotationDegree;
}

void ATacticalCombatPawn::InterpolateLocationAndRotation(float DeltaTime)
{
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), LocationDesired, DeltaTime, 1 / MovementSmoothness);
	FQuat NewRotation = FQuat::Slerp(GetActorRotation().Quaternion(), RotationDesired.Quaternion(), DeltaTime / RotationSmoothness);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}
