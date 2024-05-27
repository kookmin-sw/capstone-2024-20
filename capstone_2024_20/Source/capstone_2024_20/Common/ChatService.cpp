#include "ChatService.h"
#include "./ChatWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"


AChatService::AChatService()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AChatService::BeginPlay()
{
	Super::BeginPlay();
	
	ChatWidget = CreateWidget<UChatWidget>(GetWorld(), ChatWidgetClass);
	if (ChatWidget)
	{
		ChatWidget->AddToViewport();
	}
}

void AChatService::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChatService::EnableChat()
{
	ChatWidget->EnableChat();
}

void AChatService::AddChatLog(EChatType ChatType, const FString& Title, const FString& NewDetail)
{
	ChatWidget->AddChatLog(ChatType, Title, NewDetail);
}

void AChatService::SendNotifyMessage(const FString& Text)
{
	if(HasAuthority() == false)
		return;
	
	MulticastRPC_ReceiveMessage(EChatType::Notify,
		TEXT("공지"), Text);
}

void AChatService::MulticastRPC_ReceiveMessage_Implementation(EChatType ChatType, const FString& Title,
                                                              const FString& Text)
{
	if(ChatType == EChatType::Normal)
	{
		FString NormalTitle = *Title;
		FString PlayerName = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetPlayerName();
		if(NormalTitle.Equals(PlayerName) == true)
		{
			NormalTitle.Append(TEXT("(나)"));
		}

		AddChatLog(ChatType, NormalTitle, Text);
		return;
	}
	
	AddChatLog(ChatType, Title, Text);
}
