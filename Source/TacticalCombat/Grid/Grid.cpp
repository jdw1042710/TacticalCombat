// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "GridVisualizer.h"
#include "GridModifier.h"
#include "GridUtility.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGrid::AGrid()
{
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor_GridVisualizer"));
	ChildActor -> SetChildActorClass(AGridVisualizer::StaticClass());
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	GridLocation = GetActorLocation();
	Super::BeginPlay();
}

void AGrid::OnConstruction(const FTransform& Transform)
{
}


// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::SpawnGrid()
{
	SpawnGrid(GetActorLocation(), GridTileSize, GridTileCount, GridShape);
}

void AGrid::SetLocation(FVector Value)
{
	SpawnGrid(Value, GridTileSize, GridTileCount, GridShape);
}

void AGrid::SetGridOffest(float Value)
{
	GridOffset = Value;
	SpawnGrid();
}

FVector AGrid::GetCursorLocationOnGrid()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FHitResult Hit;
	// Line Trace�� �����ϸ� �ش� ���� ��ȯ (Grid Channel)
	bool bHitSuccess;
	bHitSuccess = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, Hit);
	if (bHitSuccess)
	{
		// UE_LOG(LogTemp, Display, TEXT("1 : %s"), *Hit.Location.ToString());
		return Hit.Location;
	}
	// Line Trace�� �����ϸ� �ش� ���� ��ȯ (GoundAndModifier Channel)
	bHitSuccess = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, Hit);
	if (bHitSuccess)
	{
		// UE_LOG(LogTemp, Display, TEXT("2 : %s"), *Hit.Location.ToString());
		return Hit.Location;
	}
	//// ���н� ���Ƿ� ���콺 �����Ϳ� ��鰣�� ���������� ��ȯ
	FVector MouseWorldLocation, MouseWorldDirection;
	float MouseLineTraceLength = 1000.f;;
	bHitSuccess = PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	if (bHitSuccess)
	{
		return FMath::LinePlaneIntersection(MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * MouseLineTraceLength, FPlane(GridLocation, FVector::UpVector));
	}
	return FVector(-1);
}

void AGrid::SetTileCount(FIntPoint Value)
{
	SpawnGrid(GridLocation, GridTileSize, Value, GridShape);
}

void AGrid::SetTileSize(FVector Value)
{
	SpawnGrid(GridLocation, Value, GridTileCount, GridShape);
}

// Ŀ�� �Ʒ��� Ÿ���� ���� ���, (-1, -1)���� ��ȯ
FIntPoint AGrid::GetTileIndexUnderCursor()
{
	FVector Location = GetCursorLocationOnGrid();
	FIntPoint Index = GetTileIndexFromWorldLocation(Location);
	return Index;
}

FIntPoint AGrid::GetTileIndexFromWorldLocation(FVector Location)
{
	FVector LocationOnGrid = Location - GetBottomLeftLocation(); // Grid �������κ��� ������� Location
	FVector TileIndexWithZ;
	FIntPoint Index;
	switch (GridShape)
	{
	case EGridShape::Square:
		TileIndexWithZ = UGridUtility::SnapVectorToVector(LocationOnGrid, GridTileSize) / GridTileSize;
		Index = FIntPoint(TileIndexWithZ.X, TileIndexWithZ.Y);
		break;
	case EGridShape::Hexagon:
		// ���� �ߴ�
		break;
	case EGridShape::Triangle:
		// ���� �ߴ�
		break;
	default:
		break;
	}
	return Index;
}

bool AGrid::GetTileDataFromIndex(FIntPoint Index, FTileData& Data)
{
	if (!IsIndexValid(Index)) return false;
	Data = GridTiles[Index];
	return true;
}

bool AGrid::IsIndexValid(FIntPoint Index)
{
	return GridTiles.Contains(Index);
}


//void PrintStates(FTileData data)
//{
//	TArray<ETileState> tset = data.States;
//	FString log = data.Index.ToString() + FString(" Elements : ");
//	for (auto element : tset)
//	{
//		switch (element)
//		{
//		case ETileState::Hovered:
//			log += FString("Hovered ");
//			break;
//		case ETileState::Selected:
//			log += FString("Selected ");
//			break;
//		case ETileState::None:
//			log += FString("None ");
//			break;
//		}
//	}
//	UE_LOG(LogTemp, Warning, TEXT("%s"), *log);
//}

void AGrid::AddStateToTile(FIntPoint Index, ETileState State)
{
	
	FTileData Data;
	//�ش� Index�� �����ϴ� Ÿ���� �������� �ʴ� ��� ����
	if (!GetTileDataFromIndex(Index, Data)) return;

	// �ش� State�� �����ϴ� ��� ����
	if (Data.States.Contains(State)) return;
	Data.States.AddUnique(State);

	GridTiles[Index] = Data;
	GridVisualizer->UpdateTileVisual(Data);
}


void AGrid::RemoveStateFromTile(FIntPoint Index, ETileState State)
{
	//�ش� Index�� �����ϴ� Ÿ���� �������� �ʴ� ��� ����
	FTileData Data;
	if (!GetTileDataFromIndex(Index, Data)) return;

	// �ش� State�� �������� �ʴ� ��� ����
	if (!Data.States.Contains(State)) return;

	Data.States.Remove(State);
	GridTiles[Index] = Data;
	GridVisualizer->UpdateTileVisual(Data);
}

void AGrid::SpawnGrid(const FVector& Location, const FVector& TileSize, const FIntPoint& TileCount, EGridShape Shape, bool bAlwaysSpawn)
{
	SetActorLocation(Location);
	GridLocation = Location;
	GridTileSize = TileSize;
	GridTileCount = TileCount;
	GridShape = Shape;

	GridVisualizer = Cast<AGridVisualizer>(ChildActor->GetChildActor());
	if (!ensure(GridVisualizer != nullptr)) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Grid Visualizer Is Not Created"));
		return; 
	}

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
			FIntPoint Index(X, Y);
			FTransform NewTileTransform
			(
				GetTileRotationFromGridIndex(Index),
				GetTileLocationFromGridIndex(Index),
				GetTileScale()
			);
			if (bAlwaysSpawn)
			{
				FTileData Data = FTileData(FIntPoint(X, Y), ETileType::Ground, NewTileTransform);
				AddGridTile(Data);
			}
			else 
			{
				// Add Tile By Trace Result
				FVector ImpactPoint;
				ETileType TileType = ETileType::None;
				TraceForGround(NewTileTransform.GetLocation(), ImpactPoint, TileType);
				FVector TracedLoctation = NewTileTransform.GetLocation();
				TracedLoctation.Z = FMath::GridSnap<float>(ImpactPoint.Z, GridTileSize.Z);
				NewTileTransform.SetLocation(TracedLoctation);
				FTileData Data = FTileData(FIntPoint(X, Y), TileType, NewTileTransform);
				AddGridTile(Data);
			}
		}
	}
}

void AGrid::DestoryGrid()
{
	GridTiles.Empty();
	GridVisualizer->DestroyGridVisual();
}

void AGrid::AddGridTile(const FTileData& Tile)
{
	GridTiles.Add(Tile.Index, Tile);
	GridVisualizer->UpdateTileVisual(Tile);
}

void AGrid::RemoveGridTile(const FIntPoint& Index)
{
	GridTiles.Remove(Index);
	GridVisualizer->UpdateTileVisual(
		FTileData(
			Index,
			ETileType::None,
			FTransform::Identity
		)
	);
}

FVector AGrid::GetGridBottomLeftCornerLocaion()
{
	// GridTileCount�� ������ Ȧ�� ���� ¦���� ������ִ� Offset
	// �� �߾��� Tile�� ��Ȯ�� ����� �� Snap�� �ǵ��� Offset��ŭ�� ���ش�.
	FIntPoint TileCountOffset;
	// Center���� Left Bottm������ TileCount�� ������� Divider;
	FIntPoint TileCountDivider;
	// �밢�� ������ ����
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
		TileCountDivider = FIntPoint(3, 2); // Y������ Grid�� �ݾ� ���� �����ϹǷ� 4�� ������
		HalfDiagVector = FVector(FVector2D((GridTileCount - TileCountOffset) / TileCountDivider) * FVector2D(GridTileSize), 0);
		HalfDiagVector = UGridUtility::SnapVectorToVector(HalfDiagVector, GridTileSize * FVector(1.5, 1, 1));
		break;
	case EGridShape::Triangle:
		TileCountOffset = FIntPoint(1, 1);
		TileCountDivider = FIntPoint(2, 4); // Y������ Grid�� �ݾ� ���� �����ϹǷ� 4�� ������
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
		Multiplier.X = 2; // ���ﰢ�� Tile������ Snap�� ���� ����
		break;
	}
	return UGridUtility::SnapVectorToVector(GridLocation, GridTileSize * Multiplier);
}

FVector AGrid::GetTileLocationFromGridIndex(FIntPoint Index)
{
	FVector2D Multiplier(1, 1);
	FVector Offset(0);
	switch (GridShape)
	{
	case EGridShape::Hexagon:
		Multiplier = FVector2D(0.75, 1);
		Offset = FVector(0, UGridUtility::IsEven(Index.X) ? 0 : GridTileSize.Y * 0.5, 0);
		break;
	case EGridShape::Triangle:
		Multiplier = FVector2D(1, 0.5);
		break;
	}
	return 	GetGridBottomLeftCornerLocaion()
		+ GridTileSize * FVector(FVector2D(Index) * Multiplier, 0)
		+ Offset // For Hexagon
		+ FVector::UpVector * GridOffset;
}

FQuat AGrid::GetTileRotationFromGridIndex(FIntPoint Index)
{
	FQuat TileRotation = FQuat::Identity;
	if (GridShape == EGridShape::Triangle)
	{
		float Yaw = (UGridUtility::IsEven(Index.X) ? 180 : 0) + (UGridUtility::IsEven(Index.Y)? 180 : 0);
		TileRotation = FRotator(
			0, 
			Yaw,
			0)
			.Quaternion();
	}
	return TileRotation;
}

FVector AGrid::GetTileScale()
{
	return GridTileSize / GridShapeData.Size;
}

bool AGrid::TryUpdateInstancedMeshByCurrentShape()
{
	FGridShapeData* DataPointer = UGridShapeUtility::GetShapeData(GridShape);
	if (DataPointer == nullptr) return false; // Invalid Data
	GridShapeData = *DataPointer;
	// Update Instanced Mesh
	GridVisualizer->Initialize(this);
	return true;
}

bool AGrid::TraceForGround(FVector TraceLocation, FVector& OutLocation, ETileType& TileType)
{
	TArray<FHitResult> Hits;
	const float Range = 500.f;
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
	if (Hits.Num() > 0) 
	{
		OutLocation = GetLocationFromHits(Hits, TileType);
	}
	return Hits.Num() > 0;
}

FVector AGrid::GetLocationFromHits(const TArray<FHitResult>& Hits, ETileType& TileType)
{
	FVector Result = FVector::ZeroVector;
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
