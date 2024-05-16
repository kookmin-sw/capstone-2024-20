#include "LobbyInteractionWidgetComponent.h"

#include "LobbyRoundProgressWidget.h"
#include "capstone_2024_20/Common/RoundProgressWidget.h"


ULobbyInteractionWidgetComponent::ULobbyInteractionWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void ULobbyInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	LobbyRoundProgressWidget = GetRoundProgressWidget<ULobbyRoundProgressWidget>();
}


void ULobbyInteractionWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULobbyInteractionWidgetComponent::ShowWithText(FString& Key, FString& Explain)
{
	LobbyRoundProgressWidget->SetKeyText(Key);
	LobbyRoundProgressWidget->SetExplainText(Explain);
	Show();
}

