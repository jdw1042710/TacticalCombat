// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Action.h"
#include "Action_IncreaseDecreaseTileHeight.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UAction_IncreaseDecreaseTileHeight : public UAction
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void ExecuteAction(FIntPoint Index) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsInCrease = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
