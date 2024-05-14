#include "MyIngameHUD.h"

void AMyIngameHUD::BeginPlay()
{
	Super::BeginPlay();

	InGameWidget = CreateWidget<UUserWidget>(GetWorld(), IngameWidgetClass);
	InGameWidget->AddToViewport();
}
