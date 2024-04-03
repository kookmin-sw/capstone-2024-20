// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

#include "UnrealWidgetFwd.h"
#include "Components/WidgetComponent.h"


// Sets default values
ALobbyCharacter::ALobbyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void ALobbyCharacter::Init()
{
	LobbyPlayerState = Cast<ALobbyPlayerState>(GetPlayerState());
	GEngine->AddOnScreenDebugMessage(-1, 60.0f,
	                                 FColor::Orange, TEXT("Init Character"));
	LobbyPlayerState->OnIsReadyChanged.BindDynamic(this, &ALobbyCharacter::SetReady);

	
	if(IsLocallyControlled())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->InputComponent->BindKey(EKeys::C, IE_Pressed,
													  LobbyPlayerState, &ALobbyPlayerState::SetReady);
		}
	}
}

// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::Init, 2.0f, false);

	ReadyCharacterWidget = Cast<UReadyCharacterWidget>(
		WidgetComponent->GetWidget());

	ReadyCharacterWidget->ChnageColor(FLinearColor::Red);
	ReadyCharacterWidget->SetVisibilityFromBool(false);

}

// Called every frame
void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALobbyCharacter::SetReady()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Silver,
		TEXT("Set Ready"));
	ReadyCharacterWidget->SetVisibilityFromBool(LobbyPlayerState->IsReady());
}