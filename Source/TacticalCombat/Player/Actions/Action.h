// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Action.generated.h"

// Owner Actor는 항상 PlayerAction이어야 함.
UCLASS()
class TACTICALCOMBAT_API UAction : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UAction();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void ExecuteAction(FIntPoint Index);
protected:

	class APlayerActions* PlayerActions;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
