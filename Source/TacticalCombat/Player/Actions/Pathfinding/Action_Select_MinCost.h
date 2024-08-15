// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Grid/Action_SelectTile.h"
#include "Action_Select_MinCost.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UAction_Select_MinCost : public UAction_SelectTile
{
	GENERATED_BODY()
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ExecuteAction(FIntPoint Index) override;

	virtual void DestroyComponent(bool bPromoteChildren = false) override;

	UPROPERTY(BlueprintReadWrite)
	bool bIncludeDiagonals;
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
