// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelLoader.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

// Sets default values
ALevelLoader::ALevelLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelLoader::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelLoader::LoadLevel(FName LevelName, bool bForce)
{
	// Prevent Load Same Level
	if (!bForce && LoadedLevelName == LevelName)
	{
		UE_LOG(LogTemp, Display, TEXT("It is Same Level"));
		return;
	}

	// Unload Exist Level
	if (LoadedLevel != nullptr) 
	{
		LoadedLevel->SetShouldBeLoaded(false);
		LoadedLevel->SetShouldBeVisible(false);
	}

	// Load New Level;
	LoadedLevelName = LevelName;
	LoadedLevel = UGameplayStatics::GetStreamingLevel(this, LevelName);
	if (LoadedLevel == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("There is no level such as %s"), *LevelName.ToString());
		return;
	}
	LoadedLevel->SetShouldBeLoaded(true);
	LoadedLevel->SetShouldBeVisible(true);

}

