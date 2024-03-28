// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinBoxWithLabelVector.h"
#include "Components/TextBlock.h"
#include "Components/SpinBox.h"
#include "Delegates/DelegateSignatureImpl.inl"

void USpinBoxWithLabelVector::NativePreConstruct()
{
	UpdateLabelText();
	UpdateValueSpinBox();
	Super::NativePreConstruct();
}

void USpinBoxWithLabelVector::NativeConstruct()
{
	ValueSpinBoxX->OnValueChanged.AddDynamic(this, &USpinBoxWithLabelVector::OnValueXChanged);
	ValueSpinBoxY->OnValueChanged.AddDynamic(this, &USpinBoxWithLabelVector::OnValueYChanged);
	ValueSpinBoxZ->OnValueChanged.AddDynamic(this, &USpinBoxWithLabelVector::OnValueZChanged);

	Super::NativeConstruct();
}

void USpinBoxWithLabelVector::NativeDestruct()
{
	Super::NativeDestruct();
	ValueChangedEvent.Clear();
}


void USpinBoxWithLabelVector::SetLabelName(FString Name)
{
	LabelName = Name;
	UpdateLabelText();
}

void USpinBoxWithLabelVector::SetValue(FVector NewValue)
{
	Value = NewValue;
	if (ValueSpinBoxX->GetValue() != Value.X 
		|| ValueSpinBoxY->GetValue() != Value.Y
		|| ValueSpinBoxZ->GetValue() != Value.Z)
	{
		ValueSpinBoxX->SetValue(Value.X);
		ValueSpinBoxY->SetValue(Value.Y);
		ValueSpinBoxZ->SetValue(Value.Z);
	}
}

void USpinBoxWithLabelVector::AddValueChangedEvent(UObject* UserObject, FName CallbackName)
{
	ValueChangedEvent.AddUFunction(UserObject, CallbackName);
}

void USpinBoxWithLabelVector::UpdateLabelText()
{
	LabelText->SetText(FText::FromString(LabelName));
}

void USpinBoxWithLabelVector::OnValueXChanged(float NewValueX)
{
	Value.X = NewValueX;
	BroadCastValueChange();
}

void USpinBoxWithLabelVector::OnValueYChanged(float NewValueY)
{
	Value.Y = NewValueY;
	BroadCastValueChange();
}

void USpinBoxWithLabelVector::OnValueZChanged(float NewValueZ)
{
	Value.Z = NewValueZ;
	BroadCastValueChange();
}

void USpinBoxWithLabelVector::BroadCastValueChange()
{
	ValueChangedEvent.Broadcast(Value);
}

void USpinBoxWithLabelVector::UpdateValueSpinBox()
{
	TArray<USpinBox*> SpinBoxes;
	SpinBoxes.Add(ValueSpinBoxX);
	SpinBoxes.Add(ValueSpinBoxY);
	SpinBoxes.Add(ValueSpinBoxZ);
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

