// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"

#include "Components/EditableText.h"

void UChatWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

FReply UChatWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Enter)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UChatWidget::OnKeyEnter, 0.1f, false);
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UChatWidget::EnableChat()
{
	FString EditTableText = EditableText->GetText().ToString();;
	if(EditTableText.IsEmpty() == false)
	{
		
	}
	
	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputModeUIOnly);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UChatWidget::OnKeyEnter()
{
	const FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}
