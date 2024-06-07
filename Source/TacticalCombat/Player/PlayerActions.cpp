// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActions.h"
#include "Kismet/GameplayStatics.h"
#include "../Grid/Grid.h"
#include "Actions/Grid/Action_SelectTile.h"

// Sets default values
APlayerActions::APlayerActions()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerActions::SetMouseAction(TSubclassOf<UAction> LeftClickActionClass, TSubclassOf<UAction> RightClickActionClass)
{
	// Left Click Action ���
	if (MouseLeftClickAction != nullptr)
	{
		MouseLeftClickAction->DestroyComponent();
	}

	MouseLeftClickAction = Cast<UAction>(
		AddComponentByClass(
			LeftClickActionClass,
			false,
			FTransform::Identity,
			false)
	);

	// Right Click Action ���
	if (MouseRightClickAction != nullptr)
	{
		MouseRightClickAction->DestroyComponent();
	}

	MouseRightClickAction = Cast<UAction>(
		AddComponentByClass(
			RightClickActionClass,
			false,
			FTransform::Identity,
			false)
	);
	
	OnSelectedTileFlagChanged.Broadcast();
}

bool APlayerActions::IsMouseActionSet(TSubclassOf<UAction> MouseAction) const
{
	bool bIsLeftClickMouseActionSet =
		(MouseLeftClickAction != nullptr)
		&& (MouseAction == MouseLeftClickAction->GetClass());
	bool bIsRightClickMouseActionSet =
		(MouseRightClickAction != nullptr)
		&& (MouseAction == MouseRightClickAction->GetClass());
	return bIsLeftClickMouseActionSet || bIsRightClickMouseActionSet;
}

// Called when the game starts or when spawned
void APlayerActions::BeginPlay()
{
	SetupPlayerInputComponent();
	FindGrid();

	Super::BeginPlay();
}

// APlayerActions ��ü �����ÿ� World���� Grid��ü�� ã�� �Լ�
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

// �� �����Ӹ��� APlayerActions ��ü�� ���� �������� �����ϴ� �Լ�
void APlayerActions::UpdateTileUnderCursor()
{
	if (Grid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grid is null"));
	}
	FIntPoint TileUnderCursor = Grid->GetTileIndexUnderCursor();

	// ���� �����Ӱ� ������ Ÿ���� ��� ����
	if (TileUnderCursor == HoveredTileIndex) return;

	Grid->RemoveStateFromTile(HoveredTileIndex, ETileState::Hovered);

	HoveredTileIndex = TileUnderCursor;
	// Ŀ�� �Ʒ��� Ÿ���� ���� ���
	if (HoveredTileIndex == FIntPoint(-1, -1)) return;

	Grid->AddStateToTile(HoveredTileIndex, ETileState::Hovered);

	//��ư Ȧ����� Action�� ó��
	OnHoveredTileChanged();
}

// ���콺 ��Ŭ���� ȣ��Ǵ� �Լ�
void APlayerActions::MouseLeftClick()
{
	bIsLeftClickDown = true;
	ExecuteMouseLeftClickAction();
}

void APlayerActions::ExecuteMouseLeftClickAction()
{
	if (MouseLeftClickAction == nullptr) return;
	UpdateTileUnderCursor();
	MouseLeftClickAction->ExecuteAction(HoveredTileIndex);
}

void APlayerActions::StopMouseLeftClick()
{
	bIsLeftClickDown = false;
}

// ���콺 ��Ŭ���� ȣ��Ǵ� �Լ�
void APlayerActions::MouseRightClick()
{
	bIsRightClickDown = true;
	ExecuteMouseRightClickAction();
}

void APlayerActions::ExecuteMouseRightClickAction()
{
	if (MouseRightClickAction == nullptr) return;
	UpdateTileUnderCursor();
	MouseRightClickAction->ExecuteAction(HoveredTileIndex);
}

void APlayerActions::StopMouseRightClick()
{
	bIsRightClickDown = false;
}

void APlayerActions::OnHoveredTileChanged()
{
	if (bIsLeftClickDown)
	{
		ExecuteMouseLeftClickAction();
	}
	if (bIsRightClickDown)
	{
		ExecuteMouseRightClickAction();
	}
}

// Called every frame
void APlayerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTileUnderCursor();
}


// �Է� �Լ� Bind
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
			&APlayerActions::MouseLeftClick
		);
		InputComponent->BindAction
		(
			"Select",
			IE_Released,
			this,
			&APlayerActions::StopMouseLeftClick
		);

		InputComponent->BindAction
		(
			"Deselect",
			IE_Pressed,
			this,
			&APlayerActions::MouseRightClick
		);
		InputComponent->BindAction
		(
			"Deselect",
			IE_Released,
			this,
			&APlayerActions::StopMouseRightClick
		);
	}
}


