// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridPathfinding.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALCOMBAT_API UGridPathfinding : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridPathfinding();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// ���� Index�� ������ Ÿ���� ��ȯ
	/// </summary>
	/// <param name="Index"> ���� Index </param>
	/// <param name="bIncludeDiagonals"> �밢�� Ÿ�ϵ� �����Ͽ� ���� ��ȯ���� ������ ����</param>
	/// <returns> ���� Index�� Neighbor Tiles </returns>
	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> GetValidTileNeighbors(FIntPoint Index, bool bIncludeDiagonals = false);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class AGrid* Grid;

	TArray<FIntPoint> GetNeighborIndexes(FIntPoint Index, bool bIncludeDiagonals = false);
	TArray<FIntPoint> GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals = false);
		
};
