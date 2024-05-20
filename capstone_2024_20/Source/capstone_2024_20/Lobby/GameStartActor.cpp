// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartActor.h"

#include "CapCharacter.h"
#include "LevelSequencePlayer.h"
#include "LobbyGameMode.h"
#include "LobbyPlateWidgetComponent.h"
#include "LobbyWidget.h"
#include "RoundProgressControllerWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "capstone_2024_20/01_Network/PlayerListWidget.h"
#include "Kismet/GameplayStatics.h"


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

void AGameStartActor::SkipPressed()
{
	GetWorld()->GetTimerManager().SetTimer(SkipTimerHandle, this, &ThisClass::Skip, 2.1f);
	RoundProgressControllerWidget->StartProgressBar(2.0f);
}

void AGameStartActor::SkipRealesed()
{
	GetWorld()->GetTimerManager().ClearTimer(SkipTimerHandle);
	RoundProgressControllerWidget->StopProgressBar();
}

void AGameStartActor::Skip()
{
	if(HasAuthority() == true)
	{
		GameStart();
		MultiRPC_Skip();
	}
}

void AGameStartActor::MultiRPC_Skip_Implementation()
{
	LevelSequencePlayer->Pause();
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

		TArray<UUserWidget*> FoundWidgets3;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets3, UReadyCharacterWidget::StaticClass(),
		                                              false);

		TArray<AActor*> FoundCharacter;
		UGameplayStatics::GetAllActorsOfClass(World, ACapCharacter::StaticClass(), FoundCharacter);

		for (UUserWidget* Widget : FoundWidgets)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
		for (UUserWidget* Widget : FoundWidgets2)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
		for (UUserWidget* Widget : FoundWidgets3)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}

		// for (AActor* CharacterActor : FoundCharacter)
		// {
		// 	Cast<ACapCharacter>(CharacterActor)->WidgetComponent->SetVisibilityFromBool(false);
		// }
	}

	if (HasAuthority() == true)
	{
		RoundProgressControllerWidget = CreateWidget<URoundProgressControllerWidget>(
			GetWorld(), RoundProgressControllerWidgetClass);
		RoundProgressControllerWidget->AddToViewport();

		RoundProgressControllerWidget->SetPercent(0.0f);
		if (PlayerController)
		{
			PlayerController->InputComponent->BindKey(EKeys::Escape, IE_Pressed,
			                                          this, &ThisClass::SkipPressed);
			PlayerController->InputComponent->BindKey(EKeys::Escape, IE_Released,
			                                          this, &ThisClass::SkipRealesed);

			PlayerController->InputComponent->BindKey(EKeys::R, IE_Pressed,
													  this, &ThisClass::SkipPressed);
			PlayerController->InputComponent->BindKey(EKeys::R, IE_Released,
													  this, &ThisClass::SkipRealesed);
		}
	}

	LevelSequencePlayer->Play();
}
