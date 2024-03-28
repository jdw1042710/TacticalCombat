// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinBoxWithLabelVector2D.h"
#include "Components/TextBlock.h"
#include "Components/SpinBox.h"
#include "Delegates/DelegateSignatureImpl.inl"

void USpinBoxWithLabelVector2D::NativePreConstruct()
{
	UpdateLabelText();
	UpdateValueSpinBox();
	Super::NativePreConstruct();
}

void USpinBoxWithLabelVector2D::NativeConstruct()
{
	ValueSpinBoxX->OnValueChanged.AddDynamic(this, &USpinBoxWithLabelVector2D::OnValueXChanged);
	ValueSpinBoxY->OnValueChanged.AddDynamic(this, &USpinBoxWithLabelVector2D::OnValueYChanged);
	Super::NativeConstruct();
}

void USpinBoxWithLabelVector2D::NativeDestruct()
{
	Super::NativeDestruct();
	ValueChangedEvent.Clear();
}


void USpinBoxWithLabelVector2D::SetLabelName(FString Name)
{
	LabelName = Name;
	UpdateLabelText();
}

void USpinBoxWithLabelVector2D::SetValue(FVector2D NewValue)
{
	Value = NewValue;
	if (ValueSpinBoxX->GetValue() != Value.X || ValueSpinBoxY->GetValue() != Value.Y)
	{
		ValueSpinBoxX->SetValue(Value.X);
		ValueSpinBoxY->SetValue(Value.Y);
	}
}

void USpinBoxWithLabelVector2D::AddValueChangedEvent(UObject* UserObject, FName CallbackName)
{
	ValueChangedEvent.AddUFunction(UserObject, CallbackName);
}

void USpinBoxWithLabelVector2D::UpdateLabelText()
{
	LabelText->SetText(FText::FromString(LabelName));
}

void USpinBoxWithLabelVector2D::OnValueXChanged(float NewValueX)
{
	Value.X = NewValueX;
	BroadCastValueChange();
}

void USpinBoxWithLabelVector2D::OnValueYChanged(float NewValueY)
{
	Value.Y = NewValueY;
	BroadCastValueChange();
}

void USpinBoxWithLabelVector2D::BroadCastValueChange()
{
	ValueChangedEvent.Broadcast(Value);
}

void USpinBoxWithLabelVector2D::UpdateValueSpinBox()
{
	TArray<USpinBox*> SpinBoxes;
	SpinBoxes.Add(ValueSpinBoxX);
	SpinBoxes.Add(ValueSpinBoxY);

	for (auto SpinBox : SpinBoxes) 
	{
		SpinBox->SetMinValue(MinValue);
		SpinBox->SetMaxValue(MaxValue);
		SpinBox->SetMinSliderValue(MinSliderValue);
		SpinBox->SetMaxSliderValue(MaxSliderValue);
		SpinBox->SetMinFractionalDigits(MinFractDigits);
		SpinBox->SetMaxFractionalDigits(MaxFractDigits);
		SpinBox->SetAlwaysUsesDeltaSnap(bAlwaysSnapToDelta);
		SpinBox->SetDelta(Delta);
	}

	SetValue(Value);
}

