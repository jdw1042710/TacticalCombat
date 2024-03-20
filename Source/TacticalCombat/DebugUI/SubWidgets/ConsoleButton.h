// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConsoleButton.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UConsoleButton : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetConsoleCommand(FString Text);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ConsoleButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ConsoleText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ConsoleCommand;

	UFUNCTION()
	void SetConsoleText(FString Text);

	UFUNCTION()
	void OnClickConsoleButton();
};
