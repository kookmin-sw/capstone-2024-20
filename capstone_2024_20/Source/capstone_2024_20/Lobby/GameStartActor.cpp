// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartActor.h"

#include "CapCharacter.h"
#include "LevelSequencePlayer.h"
#include "LobbyGameMode.h"
#include "LobbyWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "capstone_2024_20/01_Network/PlayerListWidget.h"


AGameStartActor::AGameStartActor()
{
	PrimaryActorTick.bCanEverTick = true;
	LongInteractionThreshold = 2.0f;

	ExplainText = TEXT("게임시작");
}

void AGameStartActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGameStartActor::InteractionEnter()
{
	Super::InteractionEnter();
}

void AGameStartActor::InteractionLongEnter()
{
	Super::InteractionLongEnter();
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (HasAuthority() == true && GameMode->IsReadyAllPlayer())
	{
		FTimerHandle TImerHandle;
		GetWorld()->GetTimerManager().SetTimer(TImerHandle, this, &ThisClass::GameStart, 22.0f);
		Multicast_PlaySequence();
	}
}

void AGameStartActor::InteractionExit()
{
	Super::InteractionExit();
}

void AGameStartActor::SetVisibleWidget(UUserWidget* NewUserWidget)
{
	VisibleWidget = NewUserWidget;
}

void AGameStartActor::GameStart()
{
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GameStart();
}

void AGameStartActor::Multicast_PlaySequence_Implementation()
{
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	ALevelSequenceActor* LevelSequenceActor;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		LevelSequence,
		PlaybackSettings,
		LevelSequenceActor
	);


	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		ACapCharacter* ClientCharacter = Cast<ACapCharacter>(PlayerController->GetCharacter());
		ClientCharacter->SetVisibleWigetWithBool(false);
	}
	

	UWorld* World = GetWorld();
	if (World)
	{
		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets, UPlayerListWidget::StaticClass(), false);

		TArray<UUserWidget*> FoundWidgets2;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets2, ULobbyWidget::StaticClass(), false);

		for (UUserWidget* Widget : FoundWidgets)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
		for (UUserWidget* Widget : FoundWidgets2)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}

		for (UUserWidget* Widget : FoundWidgets2)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	LevelSequencePlayer->Play();
}
