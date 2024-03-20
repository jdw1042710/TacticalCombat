// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UConsoleButton::NativePreConstruct()
{
	SetConsoleText(ConsoleCommand);
	Super::NativePreConstruct();
}

void UConsoleButton::NativeConstruct()
{
	Super::NativeConstruct();
	ConsoleButton->OnClicked.AddDynamic(this, &UConsoleButton::OnClickConsoleButton);
}

void UConsoleButton::SetConsoleCommand(FString Text)
{
	ConsoleCommand = Text;
	SetConsoleText(Text);
}

void UConsoleButton::SetConsoleText(FString Text)
{
	ConsoleText->SetText(FText::FromString(Text));
}

void UConsoleButton::OnClickConsoleButton()
{
	GEngine->Exec(GetWorld(), *ConsoleCommand);
}
