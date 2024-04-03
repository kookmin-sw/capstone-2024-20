// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"

#include "Interfaces/ITargetPlatform.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}

void ALobbyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1,
			60.0f, FColor::Emerald,
			TEXT("Has AUtority"));
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->InputComponent->BindKey(EKeys::C, IE_Pressed,
		                                          this, &ThisClass::TestReady);
	}
}

void ALobbyPlayerState::OnRep_IsReady()
{
	bIsCanReady = true;
	OnIsReadyChanged.Execute();
	GEngine->AddOnScreenDebugMessage(-1, 60.0f,
	                                 FColor::Yellow,
	                                 FString::Printf(TEXT("IsReady?: %s"),
	                                                 bIsReady == true ? TEXT("true") : TEXT("false")));
}

void ALobbyPlayerState::Server_SetReady_Implementation(bool IsReady)
{
	bIsReady = IsReady;

	if(HasAuthority())
	{
		OnRep_IsReady();
	}
}

void ALobbyPlayerState::TestReady()
{
	if (bIsCanReady == true)
	{
		Server_SetReady(!bIsReady);
		bIsCanReady = false;

		GEngine->AddOnScreenDebugMessage(-1, 60.0f,
		                                 FColor::Emerald, TEXT("TestREady Click"));
	}
}

bool ALobbyPlayerState::IsReady() const
{
	return bIsReady;
}

void ALobbyPlayerState::SetIsReadyChanged(FIsReadyChanged& NewIsReadyChanged)
{
	OnIsReadyChanged = NewIsReadyChanged;
}
