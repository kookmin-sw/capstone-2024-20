// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"

#include "ChatLogBox.h"
#include "capstone_2024_20/Lobby/CapController.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerState.h"

void UChatWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SendButton->OnClicked.AddDynamic(this, &ThisClass::OnClickSendButton);
	EditableText->OnTextCommitted.AddDynamic(this, &ThisClass::OnCommittedEditable);
}

void UChatWidget::OnCommittedEditable(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnKeyEnter();
		GetWorld()->GetFirstPlayerController<ACapController>()->ServerRPC_SendMessage(Text.ToString());
	}
}

void UChatWidget::EnableChat()
{
	GetWorld()->GetTimerManager().ClearTimer(ChatTimerHandle);
	SetVisibility(ESlateVisibility::Visible);
	EditableText->SetKeyboardFocus();

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(EditableText->TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	GetWorld()->GetFirstPlayerController()->SetInputMode(InputModeUIOnly);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UChatWidget::AddChatLog(const EChatType ChatType, const FString& Title, const FString& NewDetail)
{
	if (NewDetail.IsEmpty() == true)
		return;


	EditableText->SetText(FText::GetEmpty());
	UChatLogBox* ChatLogBox = CreateChatLogBox(ChatType, Title, NewDetail);

	if (ChatLogBox)
	{
		ScrollBox->AddChild(ChatLogBox);
		ChatLogs.Add(ChatLogBox);
	}

	if (ChatLogs.Num() >= 25)
	{
		ScrollBox->RemoveChild(ChatLogs[0]);
		ChatLogs.RemoveAt(0);
	}

	ScrollBox->ScrollToEnd();

	SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(ChatTimerHandle, [this]()
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}, 10.0f, false);
}

void UChatWidget::OnKeyEnter()
{
	const FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void UChatWidget::OnClickSendButton()
{
	OnCommittedEditable(EditableText->GetText(), ETextCommit::Type::OnEnter);
}

UChatLogBox* UChatWidget::CreateChatLogBox(EChatType ChatType, const FString& Title, const FString& Detail)
{
	UChatLogBox* ChatLogBox = nullptr;

	if (ChatType == EChatType::Normal)
	{
		ChatLogBox = CreateWidget<UChatLogBox>(GetWorld(), NormalChatLogBoxClass);
		ChatLogBox->SetTitleAndDetail(Title, Detail);
	}
	else if(ChatType == EChatType::Notify)
	{
		ChatLogBox = CreateWidget<UChatLogBox>(GetWorld(), NotifyChatLogBoxClass);
		ChatLogBox->SetTitleAndDetail(Title, Detail);
	}

	
		
	return ChatLogBox;
}