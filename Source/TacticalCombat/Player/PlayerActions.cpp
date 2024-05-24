// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "../Grid/Grid.h"
#include "Actions/Action_SelectTile.h"

// Sets default values
APlayerActions::APlayerActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SelectTileAction = CreateDefaultSubobject<UAction_SelectTile>(TEXT("SelectTileAction"));

}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay()
{
	SetupPlayerInputComponent();
	FindGrid();

	Super::BeginPlay();
}

void APlayerActions::FindGrid()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AGrid::StaticClass(),
		Actors
	);
	if (ensure(Actors.Num() > 0))
	{
		Grid = Cast<AGrid>(Actors[0]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There Is No Grid Actor In World"));
	}
}


void APlayerActions::UpdateTileUnderCursor()
{
	if (Grid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid is null"));
	}
	FIntPoint TileUnderCursor = Grid->GetTileIndexUnderCursor();

	// 이전 프레임과 동일한 타일인 경우 종료
	if (TileUnderCursor == HoveredTileIndex) return;

	Grid->RemoveStateFromTile(HoveredTileIndex, ETileState::Hovered);

	HoveredTileIndex = TileUnderCursor;
	// 커서 아래에 타일이 없는 경우
	if (HoveredTileIndex == FIntPoint(-1, -1)) return;

	Grid->AddStateToTile(HoveredTileIndex, ETileState::Hovered);
}
void APlayerActions::SelectTile()
{
	if (!bSelectTileActionFlag) return;
	UpdateTileUnderCursor();
	SelectTileAction->ExecuteAction(HoveredTileIndex);
}
void APlayerActions::DeselectTile()
{
	if (!bSelectTileActionFlag) return;
	UpdateTileUnderCursor();
	SelectTileAction->ExecuteAction(HoveredTileIndex);
}

// Called every frame
void APlayerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTileUnderCursor();
}

void APlayerActions::SetupPlayerInputComponent()
{
	EnableInput(GetWorld()->GetFirstPlayerController());
	if (InputComponent)
	{
		InputComponent->BindAction
		(
			"Select",
			IE_Pressed,
			this,
			&APlayerActions::SelectTile
		);
		InputComponent->BindAction
		(
			"Deselect",
			IE_Pressed,
			this,
			&APlayerActions::DeselectTile
		);
	}
}


