// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridShapeData.h"
#include "TileData.h"
#include "GridModifier.generated.h"

UCLASS()
class TACTICALCOMBAT_API AGridModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridModifier();

	inline ETileType GetType() { return Type; }
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGridShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType Type;

	UFUNCTION(BlueprintCallable)
	void SetStaticMesh();
};
