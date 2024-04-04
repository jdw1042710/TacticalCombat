// Fill out your copyright notice in the Description page of Project Settings.


#include "GridVisualizer.h"
#include "GridMeshInstance.h"
#include "Grid.h"


// Sets default values
AGridVisualizer::AGridVisualizer()
{
	SetActorTickEnabled(false);
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor_GridMeshInstance"));
	SetRootComponent(Root);
	ChildActor->SetupAttachment(Root);
	ChildActor->SetChildActorClass(AGridMeshInstance::StaticClass());

}

void AGridVisualizer::Initialize(AGrid* NewGrid)
{
	Grid = NewGrid;
	GridMeshInstance = Cast<AGridMeshInstance>(ChildActor->GetChildActor());
	if (!ensure(GridMeshInstance != nullptr)) return;

	DestroyGridVisual();
	GridMeshInstance->Initialize(
		Grid->GetCurrentShapeData().Mesh,
		Grid->GetCurrentShapeData().FlatBorderMaterial,
		FLinearColor::Black,
		ECollisionEnabled::QueryOnly);
}

void AGridVisualizer::UpdateTileVisual(FTileData Tile)
{
	GridMeshInstance->RemoveInstance(Tile.Index);
	if (UTileDataUtility::IsTileWalkable(Tile.Type)) 
	{
		GridMeshInstance->AddInstance(Tile.Transform, Tile.Index);
	}
}

void AGridVisualizer::DestroyGridVisual()
{
	GridMeshInstance->ClearInstances();
}

// Called when the game starts or when spawned
void AGridVisualizer::BeginPlay()
{
	Super::BeginPlay();
	
}

