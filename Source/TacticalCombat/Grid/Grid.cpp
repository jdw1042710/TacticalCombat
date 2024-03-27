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
	GridTileCount = TileCount.RoundToVector();
	GridShape = Shape;

	bool bSuccess = TryUpdateInstancedMeshByCurrentShape();
	if(!bSuccess) return;
	InstancedMesh->ClearInstances();
	for (int X = 0; X < TileCount.X; X++)
	{
		for (int Y = 0; Y < TileCount.Y; Y++)
		{
			FTransform NewTileTransform;
			NewTileTransform.SetLocation(GetTileLocationFromGridIndex(X, Y));
			NewTileTransform.SetRotation(GetTileRotationFromGridIndex(X, Y));
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
	// GridTileCount�� ������ Ȧ�� ���� ¦���� ������ִ� Offset
	// �� �߾��� Tile�� ��Ȯ�� ����� �� Snap�� �ǵ��� Offset��ŭ�� ���ش�.
	FVector2D TileCountOffset;
	// Center���� Left Bottm������ TileCount�� ������� Divider;
	FVector2D TileCountDivider;
	// �밢�� ������ ����
	FVector HalfDiagVector;
	switch (GridShape)
	{
	case EGridShape::Square:
		TileCountOffset = FVector2D(
			UGridUtility::IsEven(GridTileCount.X) ? 0 : 1,
			UGridUtility::IsEven(GridTileCount.Y) ? 0 : 1);
		TileCountDivider = FVector2D(2);
		HalfDiagVector = FVector((GridTileCount - TileCountOffset) / TileCountDivider * FVector2D(GridTileSize), 0);
		break;
	case EGridShape::Hexagon:
		// TileCountOffset = FVector2D(1, 1);
		TileCountDivider = FVector2D(3, 2); // Y������ Grid�� �ݾ� ���� �����ϹǷ� 4�� ������
		HalfDiagVector = FVector((GridTileCount - TileCountOffset) / TileCountDivider * FVector2D(GridTileSize), 0);
		HalfDiagVector = UGridUtility::SnapVectorToVector(HalfDiagVector, GridTileSize * FVector(1.5, 1, 1));
		break;
	case EGridShape::Triangle:
		TileCountOffset = FVector2D(1, 1);
		TileCountDivider = FVector2D(2, 4); // Y������ Grid�� �ݾ� ���� �����ϹǷ� 4�� ������
		HalfDiagVector = FVector((GridTileCount - TileCountOffset) / TileCountDivider * FVector2D(GridTileSize), 0);
		HalfDiagVector = UGridUtility::SnapVectorToVector(HalfDiagVector, GridTileSize * FVector(2, 1, 1));
		break;
	}
	return GetSnapGridCenterLocation() - HalfDiagVector;
}

FVector AGrid::GetSnapGridCenterLocation()
{
	FVector Multiplier(1, 1, 1);
	switch (GridShape)
	{
	case EGridShape::Hexagon:
		Multiplier.X = 1.5;
		break;
	case EGridShape::Triangle:
		Multiplier.X = 2; // ���ﰢ�� Tile������ Snap�� ���� ����
		break;
	}
	return UGridUtility::SnapVectorToVector(GridLocation, GridTileSize * Multiplier);
}

FVector AGrid::GetTileLocationFromGridIndex(int IndexX, int IndexY)
{
	FVector2D Index(IndexX, IndexY);
	FVector2D Multiplier(1, 1);
	FVector Offset;
	switch (GridShape)
	{
	case EGridShape::Hexagon:
		Multiplier = FVector2D(0.75, 1);
		Offset = FVector(0, UGridUtility::IsEven(IndexX) ? 0 : GridTileSize.Y * 0.5, 0);
		break;
	case EGridShape::Triangle:
		Multiplier = FVector2D(1, 0.5);
		break;
	}
	return 	GetGridBottomLeftCornerLocaion()
		+ GridTileSize * FVector(Index * Multiplier, 0)
		+ Offset // For Hexagon;
		+ FVector::UpVector; // To Avoid Collapse With Floor;
}

FQuat AGrid::GetTileRotationFromGridIndex(int IndexX, int IndexY)
{
	FQuat TileRotation = FQuat::Identity;
	if (GridShape == EGridShape::Triangle)
	{
		float Yaw = (UGridUtility::IsEven(IndexX) ? 180 : 0) + (UGridUtility::IsEven(IndexY)? 180 : 0);
		TileRotation = FRotator(
			0, 
			Yaw,
			0)
			.Quaternion();
	}
	return TileRotation;
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
