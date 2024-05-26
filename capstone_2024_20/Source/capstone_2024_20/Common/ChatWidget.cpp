// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"

#include "ChatLogBox.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerState.h"

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
		AddChatLog(EChatType::Normal, EditTableText);
	}
	
	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(TakeWidget());
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputModeUIOnly);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UChatWidget::AddChatLog(EChatType ChatType, FString& NewDetail)
{
	UChatLogBox* ChatLogBox = nullptr;
	if(ChatType == EChatType::Normal)
	{
		ChatLogBox = CreateWidget<UChatLogBox>(GetWorld(), NormalChatLogBoxClass);
		FString Title = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetPlayerName();
		ChatLogBox->SetTitleAndDetail(Title, NewDetail);
	}

	if(ChatLogBox)
	{
		ScrollBox->AddChild(ChatLogBox);
		ChatLogs.Add(ChatLogBox);
	}

	if(ChatLogs.Num() >= 10)
	{
		ScrollBox->RemoveChild(ChatLogs[0]);
		ChatLogs.RemoveAt(0);
	}
}

void UChatWidget::OnKeyEnter()
{
	const FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}
