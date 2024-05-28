// Fill out your copyright notice in the Description page of Project Settings.


#include "CapController.h"

#include "CapCharacter.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "MappingContextSwitcher.h"
#include "capstone_2024_20/MyAudioInstance.h"
#include "capstone_2024_20/Common/ChatService.h"
#include "capstone_2024_20/Common/ChatWidget.h"
#include "GameFramework/PlayerState.h"

ACapController::ACapController()
{
	MappingContextSwitcher = CreateDefaultSubobject<UMappingContextSwitcher>(TEXT("MappingContextSwitcher"));
}

void ACapController::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AChatService> It(GetWorld()); It; ++It)
	{
		if (*It)
		{
			ChatService = *It;
		}
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if(IsLocalPlayerController() == true)
		{
			ServerRPC_ChangeName(GetGameInstance<UMyAudioInstance>()->PlayerName);
		}
	}, 1.0f, false);
}

void ACapController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Started, this, &ThisClass::OnEnter);
	}
}

void ACapController::OnEnter()
{
	if(ChatService)
	{
		ChatService->EnableChat();
	}
}

void ACapController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RefreshMappingContext(GetPawn());
}

void ACapController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (IsLocalPlayerController() == false)
		return;

	RefreshMappingContext(GetPawn());
}

void ACapController::RefreshMappingContext(APawn* InPawn) const
{
	IMappingContextGetter* CapPawn = Cast<IMappingContextGetter>(InPawn);
	if (CapPawn == nullptr)
		return;

	MappingContextSwitcher->ReplaceMappingContext(CapPawn->GetMappingContext());
}

void ACapController::ServerRPC_ChangeName_Implementation(const FString& Text)
{
	if(HasAuthority() == false)
		return;

	GetPlayerState<APlayerState>()->SetPlayerName(Text);
}

void ACapController::ServerRPC_SendMessage_Implementation(const FString& Text)
{
	if(HasAuthority() == false)
		return;

	ChatService->MulticastRPC_ReceiveMessage(EChatType::Normal, PlayerState->GetPlayerName(), Text);
}
