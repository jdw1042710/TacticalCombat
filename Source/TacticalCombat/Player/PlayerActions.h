// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerActions.generated.h"

class AGrid;
UCLASS()
class TACTICALCOMBAT_API APlayerActions : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerActions();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline AGrid* GetGrid() { return Grid; };

	inline FIntPoint GetSelectedTile() { return SelectedTileIndex; };

	inline void SetSelectedTile(FIntPoint SelectedIndex) { SelectedTileIndex = SelectedIndex; };

	inline void SetSelectedTileFlag(bool bFlag) { bSelectTileActionFlag = bFlag; };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent();

	AGrid* Grid;

	UPROPERTY()
	class UAction* SelectTileAction;

	bool bSelectTileActionFlag = false;

	void FindGrid();

	UPROPERTY(VisibleAnywhere)
	FIntPoint HoveredTileIndex;

	UPROPERTY(VisibleAnywhere)
	FIntPoint SelectedTileIndex;

	UFUNCTION(BlueprintCallable)
	void UpdateTileUnderCursor();

	UFUNCTION(BlueprintCallable)
	void SelectTile();

	UFUNCTION(BlueprintCallable)
	void DeselectTile();
};
