// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UDebugMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Buttons.Add(nullptr);

	Buttons.Add(Button_Tab1);
	if (Button_Tab1 != nullptr) 
	{
		Button_Tab1->OnClicked.AddDynamic(this, &UDebugMenu::OnclickButtonTab1);
	}

	Buttons.Add(Button_Tab2);
	if (Button_Tab2 != nullptr)
	{
		Button_Tab2->OnClicked.AddDynamic(this, &UDebugMenu::OnclickButtonTab2);
	}

	Buttons.Add(Button_Tab3);
	if (Button_Tab3 != nullptr)
	{
		Button_Tab3->OnClicked.AddDynamic(this, &UDebugMenu::OnclickButtonTab3);
	}
	UpdateButtonsColor();
}

void UDebugMenu::OnClickButtonTab(int TabNum)
{
	selectedButton = WidgetSwitcher_TabContent->GetActiveWidgetIndex() == TabNum ? 0 : TabNum;
	WidgetSwitcher_TabContent->SetActiveWidgetIndex(selectedButton);
	UpdateButtonsColor();
}

void UDebugMenu::UpdateButtonsColor()
{
	for (int i = 0; i < Buttons.Num(); ++i)
	{
		if (Buttons[i] == nullptr) continue;
		Buttons[i]->SetBackgroundColor(i == selectedButton ? ClickedColor : DefaultColor);
	}
}

void UDebugMenu::OnclickButtonTab1()
{
	OnClickButtonTab(1);
}

void UDebugMenu::OnclickButtonTab2()
{
	OnClickButtonTab(2);
}

void UDebugMenu::OnclickButtonTab3()
{
	OnClickButtonTab(3);
}
