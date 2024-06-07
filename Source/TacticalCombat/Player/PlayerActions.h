// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerActions.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectedTileFlagChanged);

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


	UFUNCTION(BlueprintCallable)
	void SetMouseAction(TSubclassOf<UAction> LeftClickAction, TSubclassOf<UAction> RightClickAction);
	UFUNCTION(BlueprintCallable)
	bool IsMouseActionSet(TSubclassOf<UAction> MouseAction) const;

	UPROPERTY(BlueprintAssignable)
	FOnSelectedTileFlagChanged OnSelectedTileFlagChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent();

	AGrid* Grid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	class UAction* MouseLeftClickAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	class UAction* MouseRightClickAction;

	UPROPERTY(VisibleAnywhere)
	bool bIsLeftClickDown = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsRightClickDown = false;

	void FindGrid();

	UPROPERTY(VisibleAnywhere)
	FIntPoint HoveredTileIndex;

	UPROPERTY(VisibleAnywhere)
	FIntPoint SelectedTileIndex;

	UFUNCTION(BlueprintCallable)
	void UpdateTileUnderCursor();

	UFUNCTION(BlueprintCallable)
	void MouseLeftClick();

	UFUNCTION(BlueprintCallable)
	void ExecuteMouseLeftClickAction();

	UFUNCTION(BlueprintCallable)
	void StopMouseLeftClick();

	UFUNCTION(BlueprintCallable)
	void MouseRightClick();

	UFUNCTION(BlueprintCallable)
	void ExecuteMouseRightClickAction();

	UFUNCTION(BlueprintCallable)
	void StopMouseRightClick();

	UFUNCTION()
	void OnHoveredTileChanged();
};
