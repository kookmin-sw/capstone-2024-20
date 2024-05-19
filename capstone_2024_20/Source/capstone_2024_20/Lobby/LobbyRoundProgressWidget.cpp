#include "LobbyRoundProgressWidget.h"

#include "Components/TextBlock.h"

void ULobbyRoundProgressWidget::SetKeyText(FString& NewText)
{
	KeyText->SetText(FText::FromString(NewText));
}

void ULobbyRoundProgressWidget::SetExplainText(FString& NewText)
{
	ExplainText->SetText(FText::FromString(NewText));
}
