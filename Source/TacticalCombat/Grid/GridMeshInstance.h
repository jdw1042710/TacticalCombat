// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridMeshInstance.generated.h"

UCLASS()
class TACTICALCOMBAT_API AGridMeshInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridMeshInstance();

	void Initialize(UStaticMesh* NewMesh, UMaterialInstance* Material, FLinearColor Color, ECollisionEnabled::Type CollisionType);
	void AddInstance(FTransform WorldTransform, FIntPoint Index);
	void RemoveInstance(FIntPoint Index);
	void ClearInstances();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* InstancedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FIntPoint> InstanceIndexes;
};
