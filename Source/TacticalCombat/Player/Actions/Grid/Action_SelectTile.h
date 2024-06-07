// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Action.h"
#include "Action_SelectTile.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UAction_SelectTile : public UAction
{
	GENERATED_BODY()
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void ExecuteAction(FIntPoint Index) override;

	virtual void DestroyComponent(bool bPromoteChildren = false) override;
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
