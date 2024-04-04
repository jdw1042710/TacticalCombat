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

void AGridMeshInstance::AddInstance(FTransform WorldTransform, FIntPoint Index)
{
	// Remove if this index is already used.
	RemoveInstance(Index);
	InstancedMesh->AddInstanceWorldSpace(WorldTransform);
	InstanceIndexes.Add(Index);
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


