// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridShapeData.h"
#include "Grid.generated.h"

UCLASS()
class TACTICALCOMBAT_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* InstancedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector GridTileSize = FVector(100, 100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D GridTileCount = FVector2D(1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridShape GridShape = EGridShape::Square;

	UDataTable* GridShapeDataTable;
	FGridShapeData* GridShapeData;

	UFUNCTION(BlueprintCallable)
	void SpawnGrid(const FVector& Location, const FVector& TileSize, const FVector2D& TileCount, EGridShape Shape);

	FName GetGridShapeName();
	FVector GetGridBottomLeftCornerLocaion();
	FVector GetSnapGridCenterLocation();
	bool TryUpdateInstancedMeshByCurrentShape();
};
