// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Action.h"
#include "Action_FindPathToTarget.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UAction_FindPathToTarget : public UAction
{
	GENERATED_BODY()
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void ExecuteAction(FIntPoint Index) override;

	virtual void DestroyComponent(bool bPromoteChildren = false) override;

	UFUNCTION(BlueprintCallable)
	void SetIncludeDiagonals(bool flag) { bIncludeDiagonals = flag; }
	UFUNCTION(BlueprintCallable)
	bool GetIncludeDiagonals() { return bIncludeDiagonals; }

	UFUNCTION(BlueprintCallable)
	void SetDelayBetweenIterations(float Delay) { DelayBetweenIterations = Delay; }
	UFUNCTION(BlueprintCallable)
	float GetDelayBetweenIterations() { return DelayBetweenIterations; }
protected:
	bool bIncludeDiagonals = false;

	float DelayBetweenIterations = 0.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddStateOnTiles(TArray<FIntPoint> Path);
};
