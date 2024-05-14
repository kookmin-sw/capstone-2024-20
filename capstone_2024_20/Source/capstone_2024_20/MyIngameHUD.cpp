#include "MyIngameHUD.h"

void AMyIngameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IngameWidgetClass)
	{
		InGameWidget = CreateWidget<UUserWidget>(GetWorld(), IngameWidgetClass);
		if (InGameWidget)
		{
			InGameWidget->AddToViewport();
		}
	}
}
