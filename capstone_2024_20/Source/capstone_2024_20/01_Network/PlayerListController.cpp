// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListController.h"

#include "PlayerListWidgetCreate.h"
#include "PlayerListWidgetModifier.h"
#include "Kismet/GameplayStatics.h"


APlayerListController::APlayerListController()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	static ConstructorHelpers::FClassFinder<UPlayerListWidget>
	BP_PlayerListWidget(TEXT("/Game/WidgetBlueprints/BP_PlayerListWidget.BP_PlayerListWidget_C"));
	PlayerListWigetClass = BP_PlayerListWidget.Class;
	bReplicates = true;
}

void APlayerListController::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->InputComponent->BindKey(EKeys::G, IE_Pressed,
												  this, &APlayerListController::TTEE);
	}
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Purple,
	                                 TEXT("Begin PPPPP"));

	const ENetRole NetRole = GetLocalRole();
	FString RoleString;

	switch (NetRole)
	{
	case ROLE_None:
		RoleString = "None";
		break;
	case ROLE_SimulatedProxy:
		RoleString = "SimulatedProxy";
		break;
	case ROLE_AutonomousProxy:
		RoleString = "AutonomousProxy";
		break;
	case ROLE_Authority:
		RoleString = "Authority";
		break;
	default:
		RoleString = "Unknown";
		break;
	}

	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Emerald,
		RoleString);
	PlayerListWidgetCreate* PlayerListWigetCreate = new PlayerListWidgetCreate(GetWorld(), PlayerListWigetClass,
	                                                                           &PlayerListWidget, &PlayerListUpdate);
	PlayerListUpdate = PlayerListWigetCreate;
}

void APlayerListController::PostLogin_Implementation(APlayerController* NewPlayer)
{
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
										 TEXT("NOT NOT"));
		
	if (PlayerListUpdate)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
		                                 TEXT("NOT NOT"));
		PlayerListUpdate->PostLogin(NewPlayer);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
		                                 TEXT("nullptr"));
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Emerald,
		                                 TEXT("nullptr2"));
	}
}

void APlayerListController::Logout_Implementation(AController* Exiting)
{
	PlayerListUpdate->Logout(Exiting);
}


APlayerListController* APlayerListController::Create(UWorld* World)
{
	TArray<AActor*> FoundActors;
	APlayerListController* PlayerListController = nullptr;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerListController::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		PlayerListController = Cast<APlayerListController>(FoundActors[0]);
	}
	return PlayerListController;
}

void APlayerListController::TTEE_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Blue,
										 TEXT("aaaaa"));
}
