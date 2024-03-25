// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridUtility.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObject(TEXT("/Game/TacticalCombat/Arts/Grids/DT_GridShapeData.DT_GridShapeData"));
	if (ensure(DataTableObject.Succeeded()))
	{
		GridShapeDataTable = DataTableObject.Object;
	}
	// SpawnGrid(GridLocation, GridTileSize, GridTileCount, GridShape);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::SpawnGrid(const FVector& Location, const FVector& TileSize, const FVector2D& TileCount, EGridShape Shape)
{
	if (GridShapeDataTable == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("There is no such a table"));
		return;
	}
	GridLocation = Location;
	GridTileSize = TileSize;
	GridTileCount = TileCount;
	GridShape = Shape;

	bool bSuccess = TryUpdateInstancedMeshByCurrentShape();
	if(!bSuccess) return;
	InstancedMesh->ClearInstances();
	for (int X = 0; X < TileCount.X; X++)
	{
		for (int Y = 0; Y < TileCount.Y; Y++)
		{
			FTransform NewTileTransform;
			NewTileTransform.SetLocation(
				GetGridBottomLeftCornerLocaion() 
				+ FVector(GridTileSize.X * X, GridTileSize.Y * Y, 1)
			);
			NewTileTransform.SetScale3D(GridTileSize / GridShapeData->Size);
			InstancedMesh->AddInstanceWorldSpace(NewTileTransform);
		}
	}
}

FName AGrid::GetGridShapeName()
{
	const UEnum* GridShapeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGridShape"), true);
	if (GridShapeEnum == nullptr) return FName("Invalid");
	return *GridShapeEnum->GetDisplayNameTextByValue((int32)GridShape).ToString();
}

FVector AGrid::GetGridBottomLeftCornerLocaion()
{
	float X = GridTileCount.X / 2 * GridTileSize.X;
	float Y = GridTileCount.Y / 2 * GridTileSize.Y;
	return GetSnapGridCenterLocation() - FVector(X, Y, 0);
}

FVector AGrid::GetSnapGridCenterLocation()
{
	return UGridUtility::SnapVectorToVector(GridLocation, GridTileSize);
}

bool AGrid::TryUpdateInstancedMeshByCurrentShape()
{
	FName ShapeName = GetGridShapeName();
	GridShapeData = GridShapeDataTable->FindRow<FGridShapeData>(ShapeName, ShapeName.ToString());
	if (GridShapeData == nullptr) return false; // Invalid Data

	// Update Instanced Mesh
	InstancedMesh->SetStaticMesh(GridShapeData->FlatMesh);
	InstancedMesh->SetMaterial(0, GridShapeData->FlatBorderMaterial);
	return true;
}
