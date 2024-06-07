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
	// Left Click Action 등록
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

	// Right Click Action 등록
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

// APlayerActions 객체 생성시에 World상의 Grid객체를 찾는 함수
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

// 매 프레임마다 APlayerActions 객체의 상태 변수들을 갱신하는 함수
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

	//버튼 홀드시의 Action을 처리
	OnHoveredTileChanged();
}

// 마우스 좌클릭시 호출되는 함수
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

// 마우스 우클릭시 호출되는 함수
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


// 입력 함수 Bind
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


