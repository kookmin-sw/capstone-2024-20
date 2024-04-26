// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomPasswordInputPopupWidget.generated.h"

class UTextBlock;
class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API URoomPasswordInputPopupWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnCloseButtonClicked();

public:
	const FString RoomNameSuffix = TEXT(" 방");
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* RoomName;

	UPROPERTY(EditAnywhere, meta=(BindWIdget))
	UButton* CloseButton;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UEditableTextBox* PasswordTextBox;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* JoinButton;
	
	UFUNCTION()
	void SetActive(bool IsActive);
};
