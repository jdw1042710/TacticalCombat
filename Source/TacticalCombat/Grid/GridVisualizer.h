// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileData.h"
#include "GridVisualizer.generated.h"

class AGrid;

UCLASS()
class TACTICALCOMBAT_API AGridVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridVisualizer();

	void Initialize(AGrid* NewGrid);

	void UpdateTileVisual(FTileData Tile);

	void DestroyGridVisual();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UChildActorComponent* ChildActor;

	AGrid* Grid;
	class AGridMeshInstance* GridMeshInstance;

};
