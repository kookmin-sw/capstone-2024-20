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
	if(CommitMethod == ETextCommit::OnEnter)
	{
		OnKeyEnter();
		GetWorld()->GetFirstPlayerController<ACapController>()->ServerRPC_SendMessage(Text.ToString());
	}
}

void UChatWidget::EnableChat()
{
	EditableText->SetKeyboardFocus();
	
	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(EditableText->TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputModeUIOnly);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UChatWidget::AddChatLog(const EChatType ChatType, const FString& Title,  const FString& NewDetail)
{
	if(NewDetail.IsEmpty() == true)
		return;

	
	EditableText->SetText(FText::GetEmpty());
	UChatLogBox* ChatLogBox = nullptr;
	if(ChatType == EChatType::Normal)
	{
		ChatLogBox = CreateWidget<UChatLogBox>(GetWorld(), NormalChatLogBoxClass);
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

void UChatWidget::OnClickSendButton()
{
	OnCommittedEditable(EditableText->GetText(), ETextCommit::Type::OnEnter);
}
