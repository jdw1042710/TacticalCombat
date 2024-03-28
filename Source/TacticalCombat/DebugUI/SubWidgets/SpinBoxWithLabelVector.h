#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpinBoxWithLabelVector.generated.h"




UCLASS()
class TACTICALCOMBAT_API USpinBoxWithLabelVector : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FValueChangedEvent, FVector);
public:

	UFUNCTION(BlueprintCallable)
	void SetLabelName(FString Name);

	UFUNCTION(BlueprintCallable)
	FVector GetValue() { return Value; }

	UFUNCTION(BlueprintCallable)
	void SetValue(FVector NewValue);

	void AddValueChangedEvent(UObject* UserObject, FName CallbackName);

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
	class USpinBox* ValueSpinBoxX;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USpinBox* ValueSpinBoxY;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USpinBox* ValueSpinBoxZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MinSliderValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MaxSliderValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	FVector Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MinFractDigits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float MaxFractDigits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	bool bAlwaysSnapToDelta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpinBoxWithLabel|SpinBox")
	float Delta;

	FValueChangedEvent ValueChangedEvent;

	UFUNCTION(BlueprintCallable)
	void OnValueXChanged(float NewValueX);
	UFUNCTION(BlueprintCallable)
	void OnValueYChanged(float NewValueY);
	UFUNCTION(BlueprintCallable)
	void OnValueZChanged(float NewValueZ);
	void BroadCastValueChange();
	void UpdateValueSpinBox();
};
