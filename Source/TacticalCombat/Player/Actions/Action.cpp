// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "../PlayerActions.h"

// Sets default values
UAction::UAction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// 
void UAction::BeginPlay()
{
	Super::BeginPlay();
	PlayerActions = Cast<APlayerActions>(GetOwner());
	if (!ensure(PlayerActions != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("Action Component는 PlayerActions Actor에서만 동작합니다."));
	}

	// To Show Component In Editor
	GetOwner()->AddInstanceComponent(this);
}

void UAction::DestroyComponent(bool bPromoteChildren)
{
	// To Hide(=Remove) Component In Editor
	GetOwner()->RemoveInstanceComponent(this);
	Super::DestroyComponent(bPromoteChildren);
}

void UAction::ExecuteAction(FIntPoint Index)
{
}

void UAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
