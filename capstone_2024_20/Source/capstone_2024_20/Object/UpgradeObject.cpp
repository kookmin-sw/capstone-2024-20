#include "UpgradeObject.h"
#include "Blueprint/UserWidget.h"

AUpgradeObject::AUpgradeObject()
{
	CanBeOperated = true;
}

void AUpgradeObject::BeginPlay()
{
	Super::BeginPlay();
}

void AUpgradeObject::Operate()
{
	Super::Operate();
	
	const auto PopupUpgradeRef = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/WidgetBlueprints/UpgradeObject/BP_UpgradeWidget.BP_UpgradeWidget_C'");
	if (const auto PopupUpgradeClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr,PopupUpgradeRef); PopupUpgradeClass != nullptr)
	{
		if (UUserWidget* PopupUpgrade = CreateWidget<UUserWidget>(GetWorld(), PopupUpgradeClass); PopupUpgrade != nullptr)
		{
			PopupUpgrade->AddToViewport();

			// enable mouse cursor
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
	}	
}
