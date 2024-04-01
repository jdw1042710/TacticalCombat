// Fill out your copyright notice in the Description page of Project Settings.


#include "SpinBoxWithLabel.h"
#include "Components/TextBlock.h"
#include "Components/SpinBox.h"
#include "Delegates/DelegateSignatureImpl.inl"

void USpinBoxWithLabel::NativePreConstruct()
{
	UpdateLabelText();
	UpdateValueSpinBox();
	Super::NativePreConstruct();
}

void USpinBoxWithLabel::NativeConstruct()
{
	ValueSpinBox->OnValueChanged.AddDynamic(this, &USpinBoxWithLabel::OnValueChanged);
	Super::NativeConstruct();
}

void USpinBoxWithLabel::NativeDestruct()
{
	Super::NativeDestruct();
	ValueChangedEvent.Clear();
}


void USpinBoxWithLabel::SetLabelName(FString Name)
{
	LabelName = Name;
	UpdateLabelText();
}

void USpinBoxWithLabel::SetValue(float NewValue)
{
	Value = NewValue;
	if (ValueSpinBox->GetValue() != Value)
	{
		ValueSpinBox->SetValue(Value);
	}
}

void USpinBoxWithLabel::AddValueChangedEvent(UObject* UserObject, FName CallbackName)
{
	ValueChangedEvent.AddUFunction(UserObject, CallbackName);
}

void USpinBoxWithLabel::AddValueChangedEvent(FOnFloatValueChanged Callback)
{
	ValueChangedEvent.Add(Callback);
}

void USpinBoxWithLabel::UpdateLabelText()
{
	LabelText->SetText(FText::FromString(LabelName));
}

void USpinBoxWithLabel::UpdateValueSpinBox()
{
	if (ValueSpinBox->GetMinValue() != MinValue) 
	{
		ValueSpinBox->SetMinValue(MinValue);
	}
	if (ValueSpinBox->GetMaxValue() != MaxValue)
	{
		ValueSpinBox->SetMaxValue(MaxValue);
	}

	if (ValueSpinBox->GetMinSliderValue() != MinSliderValue)
	{
		ValueSpinBox->SetMinSliderValue(MinSliderValue);
	}
	if (ValueSpinBox->GetMaxSliderValue() != MaxSliderValue)
	{
		ValueSpinBox->SetMaxSliderValue(MaxSliderValue);
	}
	
	if (ValueSpinBox->GetValue() != Value)
	{
		ValueSpinBox->SetValue(Value);
	}
	

	if (ValueSpinBox->GetMinFractionalDigits() != MinFractDigits)
	{
		ValueSpinBox->SetMinFractionalDigits(MinFractDigits);
	}
	if (ValueSpinBox->GetMaxFractionalDigits() != MaxFractDigits)
	{
		ValueSpinBox->SetMaxFractionalDigits(MaxFractDigits);
	}
	

	if (ValueSpinBox->GetAlwaysUsesDeltaSnap() != bAlwaysSnapToDelta)
	{
		ValueSpinBox->SetAlwaysUsesDeltaSnap(bAlwaysSnapToDelta);
	}
	if (ValueSpinBox->GetDelta() != Delta)
	{
		ValueSpinBox->SetDelta(Delta);
	}

}

