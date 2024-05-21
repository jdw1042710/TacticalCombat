// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifier.h"

// Sets default values
AGridModifier::AGridModifier()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
}

void AGridModifier::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetStaticMesh();
}

void AGridModifier::SetStaticMesh()
{
	FGridShapeData* ShapeData = UGridShapeUtility::GetShapeData(Shape);
	if (ShapeData == nullptr) return;
	if (ShapeData->Mesh == nullptr) return;
	Mesh->SetStaticMesh(ShapeData->Mesh);
	Mesh->SetMaterial(0, ShapeData->FlatMaterial);
	Mesh->SetScalarParameterValueOnMaterials(FName("IsFilled"), 1);
	FVector Color;
	switch (Type)
	{
	case ETileType::Ground:
		Color = FVector(1, 1, 1);
		break;
	case ETileType::Obstacle:
		Color = FVector(1, 0, 0);
		break;
	default:
		Color = FVector::ZeroVector;
		break;
	}
	Mesh->SetVectorParameterValueOnMaterials(FName("Color"), Color);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	SetActorHiddenInGame(true);
}






