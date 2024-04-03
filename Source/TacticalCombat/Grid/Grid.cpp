// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridModifier.h"
#include "GridUtility.h"
#include "GridShapeUtility.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	InstancedMesh->ClearInstances();
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();

}

void AGrid::OnConstruction(const FTransform& Transform)
{
	// SpawnGrid(GetActorLocation(), GridTileSize, GridTileCount, GridShape);
}


// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::SetLocation(FVector Value)
{
	SpawnGrid(Value, GridTileSize, GridTileCount, GridShape);
}

void AGrid::SetGridOffest(float Value)
{
	GridOffestFromGround = Value;
	SpawnGrid(GridLocation, GridTileSize, GridTileCount, GridShape);
}

void AGrid::SetTileCount(FIntPoint Value)
{
	SpawnGrid(GridLocation, GridTileSize, Value, GridShape);
}

void AGrid::SetTileSize(FVector Value)
{
	SpawnGrid(GridLocation, Value, GridTileCount, GridShape);
}

void AGrid::SpawnGrid(const FVector& Location, const FVector& TileSize, const FIntPoint& TileCount, EGridShape Shape, bool bAlwaysSpawn)
{
	SetActorLocation(Location);
	GridLocation = Location;
	GridTileSize = TileSize;
	GridTileCount = TileCount;
	GridShape = Shape;
	InstancedMesh->ClearInstances();
	bool bSuccess = TryUpdateInstancedMeshByCurrentShape();
	if (!bSuccess) 
	{
		UE_LOG(LogTemp, Error, TEXT("Fail To Load Instance Mesh"));
		return;
	}
	// For Debugging
	// FlushPersistentDebugLines(GetWorld());
	for (int X = 0; X < TileCount.X; X++)
	{
		for (int Y = 0; Y < TileCount.Y; Y++)
		{
			FTransform NewTileTransform;
			NewTileTransform.SetLocation(GetTileLocationFromGridIndex(X, Y));
			NewTileTransform.SetRotation(GetTileRotationFromGridIndex(X, Y));
			NewTileTransform.SetScale3D(GridTileSize / GridShapeData->Size);
			FVector ImpactPoint;
			bool bTraceSuccess = TraceForGround(NewTileTransform.GetLocation(), 500.0f, ImpactPoint);;
			if (bAlwaysSpawn || bTraceSuccess)
			{
				if (bTraceSuccess) 
				{
					FVector TracedLoctation = NewTileTransform.GetLocation();
					TracedLoctation.Z = FMath::GridSnap<float>(ImpactPoint.Z, GridTileSize.Z) + GridOffestFromGround;
					NewTileTransform.SetLocation(TracedLoctation);
				}
				InstancedMesh->AddInstanceWorldSpace(NewTileTransform);
			}
		}
	}
}



FVector AGrid::GetGridBottomLeftCornerLocaion()
{
	// GridTileCount의 각각의 홀수 값을 짝수로 만들어주는 Offset
	// 정 중앙의 Tile을 정확히 벗어났을 때 Snap이 되도록 Offset만큼을 빼준다.
	FIntPoint TileCountOffset;
	// Center에서 Left Bottm까지의 TileCount를 얻기위한 Divider;
	FIntPoint TileCountDivider;
	// 대각선 벡터의 절반
	FVector HalfDiagVector;
	switch (GridShape)
	{
	case EGridShape::Square:
		TileCountOffset = FIntPoint(
			UGridUtility::IsEven(GridTileCount.X) ? 0 : 1,
			UGridUtility::IsEven(GridTileCount.Y) ? 0 : 1);
		TileCountDivider = FIntPoint(2);
		HalfDiagVector = FVector(FVector2D((GridTileCount - TileCountOffset) / TileCountDivider) * FVector2D(GridTileSize), 0);
		break;
	case EGridShape::Hexagon:
		// TileCountOffset = FVector2D(1, 1);
		TileCountDivider = FIntPoint(3, 2); // Y방향은 Grid가 반씩 겹쳐 존재하므로 4로 나눈다
		HalfDiagVector = FVector(FVector2D((GridTileCount - TileCountOffset) / TileCountDivider) * FVector2D(GridTileSize), 0);
		HalfDiagVector = UGridUtility::SnapVectorToVector(HalfDiagVector, GridTileSize * FVector(1.5, 1, 1));
		break;
	case EGridShape::Triangle:
		TileCountOffset = FIntPoint(1, 1);
		TileCountDivider = FIntPoint(2, 4); // Y방향은 Grid가 반씩 겹쳐 존재하므로 4로 나눈다
		HalfDiagVector = FVector(FVector2D((GridTileCount - TileCountOffset) / TileCountDivider) * FVector2D(GridTileSize), 0);
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
		Multiplier.X = 2; // 역삼각형 Tile에서는 Snap을 하지 않음
		break;
	}
	return UGridUtility::SnapVectorToVector(GridLocation, GridTileSize * Multiplier);
}

FVector AGrid::GetTileLocationFromGridIndex(int IndexX, int IndexY)
{
	FVector2D Index(IndexX, IndexY);
	FVector2D Multiplier(1, 1);
	FVector Offset(0);
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
		+ FVector::UpVector * GridOffestFromGround; // To Avoid Collapse With Floor;
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
	GridShapeData = UGridShapeUtility::GetShapeData(GridShape);
	if (GridShapeData == nullptr) return false; // Invalid Data

	// Update Instanced Mesh
	InstancedMesh->SetStaticMesh(GridShapeData->FlatMesh);
	InstancedMesh->SetMaterial(0, GridShapeData->FlatBorderMaterial);
	return true;
}

bool AGrid::TraceForGround(FVector TraceLocation, float Range, FVector& OutLocation)
{
	TArray<FHitResult> Hits;
	FVector RangeVecter = FVector::UpVector * Range;
	float Radius = GridTileSize.X / (GridShape == EGridShape::Triangle ? 5 : 3);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	ECollisionChannel Channel = ECC_GameTraceChannel1;
	GetWorld()->SweepMultiByChannel(
		Hits,
		TraceLocation + RangeVecter,
		TraceLocation - RangeVecter,
		FQuat::Identity,
		Channel,
		CollisionShape
	);
	ETileType TileType = ETileType::None;
	OutLocation = GetLocationFromHits(Hits, TileType);
	return IsTileWalkable(TileType);
}

FVector AGrid::GetLocationFromHits(const TArray<FHitResult>& Hits, ETileType& TileType)
{
	FVector Result = FVector::ZeroVector;
	//for (auto Hit : Hits)
	//{
	//	AGridModifier* Modifier = Cast<AGridModifier>(Hit.Actor);
	//	if (Modifier != nullptr) 
	//	{
	//		TileType = Modifier->GetType();
	//	}
	//	else 
	//	{
	//		Result = Hit.ImpactPoint;
	//		TileType = ETileType::Ground;
	//	}
	//}
	if (Hits.Num() > 0) 
	{
		Result = Hits[0].ImpactPoint;
		AGridModifier* Modifier = Cast<AGridModifier>(Hits[0].Actor);
		if (Modifier != nullptr)
		{
			TileType = Modifier->GetType();
		}
		else
		{
			TileType = ETileType::Ground;
		}
	}

	return Result;
}

bool AGrid::IsTileWalkable(ETileType TileType)
{
	return TileType == ETileType::Ground;
}
