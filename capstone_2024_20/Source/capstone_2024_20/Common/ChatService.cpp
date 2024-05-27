#include "ChatService.h"
#include "./ChatWidget.h"
#include "Blueprint/UserWidget.h"


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

void AChatService::MulticastRPC_ReceiveMessage_Implementation(EChatType ChatType, const FString& Title,
	const FString& Text)
{
	AddChatLog(ChatType, Title, Text);
}
