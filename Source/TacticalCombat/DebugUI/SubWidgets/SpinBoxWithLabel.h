// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpinBoxWithLabel.generated.h"




UCLASS()
class TACTICALCOMBAT_API USpinBoxWithLabel : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FValueChangedEvent, float);
public:
	UFUNCTION(BlueprintCallable)
	void SetLabelName(FString Name);

	UFUNCTION(BlueprintCallable)
	float GetValue() { return Value; }

	UFUNCTION(BlueprintCallable)
	void SetValue(float NewValue);

	void AddValueChangedEvent(UObject* UserObject, FName CallbackName);
	void AddValueChangedEvent(FOnFloatValueChanged Callback);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Label
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* LabelText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|Label")
	FString LabelName;

	void UpdateLabelText();

	// Spin Box
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USpinBox* ValueSpinBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MinSliderValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MaxSliderValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	int MinFractDigits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	int MaxFractDigits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	bool bAlwaysSnapToDelta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float Delta;

	FValueChangedEvent ValueChangedEvent;

	UFUNCTION(BlueprintCallable)
	void OnValueChanged(float NewValue) { Value = NewValue; ValueChangedEvent.Broadcast(NewValue); };

	void UpdateValueSpinBox();
};
