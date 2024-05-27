// Fill out your copyright notice in the Description page of Project Settings.


#include "CapCharacter.h"

#include "CapInteractionActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "LobbyInteractionWidgetComponent.h"
#include "LobbyPlateWidgetComponent.h"
#include "LobbyPlayerLinearColorFactory.h"
#include "LobbyPlayerState.h"
#include "capstone_2024_20/CharacterChangerComponent.h"
#include "capstone_2024_20/MyAudioInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACapCharacter::Init()
{
	LobbyPlayerState = Cast<ALobbyPlayerState>(GetPlayerState());
	LobbyPlayerState->OnIsReadyChanged.AddLambda([this]
	{
		this->SetReady();
	});

	// const int PlayerNumber = LobbyPlayerState->PlayerNumber;
	// if (PlayerNumber != 1)
	// {
	// 	const FLinearColor PlayerColor = ULobbyPlayerLinearColorFactory::GetLinearColor(PlayerNumber);
	//
	// 	WidgetComponent->ChangeColor(PlayerColor);
	// 	WidgetComponent->SetVisibilityFromBool(false);
	// }
	// else
	// {
	// 	FString Path = TEXT("/Game/WidgetBlueprints/Lobby/BP_RoomManagerWidget.BP_RoomManagerWidget_C");
	// 	WidgetComponent->ChangeWidget(Path);
	// }
	//
	// if (IsLocallyControlled())
	// {
	// 	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// 	if (PlayerController)
	// 	{
	// 		PlayerController->InputComponent->BindKey(EKeys::C, IE_Pressed,
	// 		                                          LobbyPlayerState, &ALobbyPlayerState::SetReady);
	// 	}
	// }
}
void ACapCharacter::RefreshNamePlate_Implementation()
{
	// if (IsLocallyControlled())
	// {
	// 	const FString PlayerName = LobbyPlayerState->GetPlayerName();
	// 	WidgetComponent->SetName(PlayerName);
	// }
}

ACapCharacter::ACapCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterChangerComponent = CreateDefaultSubobject<UCharacterChangerComponent>(TEXT("CharacterChangerComponent"));
	CharacterChangerComponent->SetIsReplicated(true);

	WidgetComponent = CreateDefaultSubobject<ULobbyPlateWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());

	InteractionWidgetComponent = CreateDefaultSubobject<
		ULobbyInteractionWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);

	InitMovement();
}

void ACapCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		GetGameInstance<UMyAudioInstance>()->OnChangeCharacterTypeDelegate.AddUObject(
			CharacterChangerComponent, &UCharacterChangerComponent::Change);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::Init, 1.0f, false);
}

void ACapCharacter::Move(const FInputActionValue& Value)
{
	if (bIsMovement == false)
		return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller == nullptr)
		return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator Yaw(0, Rotation.Yaw, 0);

	const FVector RightVector = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	const FVector ForwardVector = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);

	AddMovementInput(RightVector, MovementVector.X);
	AddMovementInput(ForwardVector, MovementVector.Y);
}

void ACapCharacter::Interact()
{
	if (CapInteractionActor)
	{
		CapInteractionActor->InteractionEnter();
		InteractionWidgetComponent->StartProgressBar(CapInteractionActor->GetLongInteractionThreshold());
	}
}

void ACapCharacter::InteractCancel()
{
	if (CapInteractionActor)
	{
		CapInteractionActor->InteractionExit();
		InteractionWidgetComponent->StopProgressBar();
	}
}

void ACapCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACapCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ThisClass::InteractCancel);
	}
}

void ACapCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                              bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                              const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);


	if(IsLocallyControlled() == false)
		return;
		
	ACapInteractionActor* ActorOhter = Cast<ACapInteractionActor>(Other);
	if (ActorOhter)
	{
		CapInteractionActor = ActorOhter;

		InteractionWidgetComponent->ShowWithText(ActorOhter->KeyText, ActorOhter->ExplainText);
	}
}

void ACapCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	ACapInteractionActor* ActorOhter = Cast<ACapInteractionActor>(OtherActor);
	if (ActorOhter)
	{
		InteractionWidgetComponent->Hide();
	}
}

void ACapCharacter::SetIsMovement(const bool bNewValue)
{
	bIsMovement = bNewValue;
}

void ACapCharacter::SetReady()
{
	WidgetComponent->SetVisibilityFromBool(LobbyPlayerState->IsReady());
}

void ACapCharacter::ServerRPC_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation)
{
	for (int i = 0; i < 10; i++)
	{
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
	ClientRPC_SetLocationAndRotation(NewLocation, NewRotation);
}

void ACapCharacter::ClientRPC_SetLocationAndRotation_Implementation(FVector NewLocation, FRotator NewRotation)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 50000.0f, 0.0f);
	FVector Direction = NewRotation.Vector().GetSafeNormal();
	for (int i = 0; i < 5; i++)
	{
		AddMovementInput(Direction, 1.0f);
	}
}

void ACapCharacter::SetVisibleWigetWithBool(bool IsActive)
{
	if(IsActive == true)
	{
		WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
		InteractionWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
		InteractionWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACapCharacter::InitMovement()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Exponential;
	GetCharacterMovement()->MaxAcceleration = 1400.0f;
}
