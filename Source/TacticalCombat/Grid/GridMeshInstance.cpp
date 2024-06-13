// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMeshInstance.h"

// Sets default values
AGridMeshInstance::AGridMeshInstance()
{
	SetActorTickEnabled(false);
	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	InstancedMesh->ClearInstances();
	InstancedMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	InstancedMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	InstancedMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InstancedMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	InstancedMesh->NumCustomDataFloats = 4;
}

void AGridMeshInstance::Initialize(
	UStaticMesh* NewMesh,
	UMaterialInstance* Material,
	FLinearColor Color,
	ECollisionEnabled::Type CollisionType
	)
{
	InstancedMesh->SetStaticMesh(NewMesh);
	InstancedMesh->SetMaterial(0, Material);
	InstancedMesh->SetVectorParameterValueOnMaterials(FName("Color"), FVector(Color));
	InstancedMesh->SetCollisionEnabled(CollisionType);
}

void AGridMeshInstance::AddInstance(FTransform WorldTransform, FIntPoint Index, const TArray<ETileState>& States)
{
	// Remove if this index is already used.
	RemoveInstance(Index);
	// Add Z Position + 1 For Visual
	WorldTransform.SetLocation(WorldTransform.GetLocation() + FVector::UpVector);
	InstancedMesh->AddInstanceWorldSpace(WorldTransform);
	int32 InstanceIndex = InstanceIndexes.Add(Index);
	FLinearColor Color = GetColorFromStates(States);
	InstancedMesh->SetCustomDataValue(InstanceIndex, 0, Color.R);
	InstancedMesh->SetCustomDataValue(InstanceIndex, 1, Color.G);
	InstancedMesh->SetCustomDataValue(InstanceIndex, 2, Color.B);
	InstancedMesh->SetCustomDataValue(InstanceIndex, 3, Color.A); // isFilled
}

void AGridMeshInstance::RemoveInstance(FIntPoint Index)
{
	int32 InstanceIndex = InstanceIndexes.Find(Index);
	if (InstanceIndex == INDEX_NONE) return;
	InstancedMesh->RemoveInstance(InstanceIndex);
	InstanceIndexes.RemoveAt(InstanceIndex);
}

void AGridMeshInstance::ClearInstances()
{
	InstancedMesh->ClearInstances();
	InstanceIndexes.Empty();
}

// Called when the game starts or when spawned
void AGridMeshInstance::BeginPlay()
{
	Super::BeginPlay();
	
}

FLinearColor AGridMeshInstance::GetColorFromStates(const TArray<ETileState>& States)
{
	for (auto State : States) 
	{
		switch (State)
		{
		case ETileState::Selected:
			// Orange
			return FLinearColor(0.8, 0.2, 0, 1);
		case ETileState::Hovered:
			// Orange ~ Yellow
			return FLinearColor(0.8, 0.5, 0.15, 1);
		case ETileState::Neighbor:
			//Pink
			return FLinearColor(1, 0.75, 0.79, 1);
		}
	}
	return FLinearColor(0, 0, 0, 0);
}


