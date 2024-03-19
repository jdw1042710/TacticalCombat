// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugMenu.generated.h"

/**
 * 
 */
UCLASS()
class TACTICALCOMBAT_API UDebugMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	TArray<class UButton*> Buttons;

	int selectedButton = 0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button_Tab1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button_Tab2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button_Tab3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DefaultColor = FLinearColor(0x66, 0x66, 0x66, 0xff);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ClickedColor = FLinearColor(0x99, 0x99, 0x99, 0xff);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher_TabContent;

	void OnClickButtonTab(int TabNum);

	void UpdateButtonsColor();

	UFUNCTION()
	void OnclickButtonTab1();
	UFUNCTION()
	void OnclickButtonTab2();
	UFUNCTION()
	void OnclickButtonTab3();

};
