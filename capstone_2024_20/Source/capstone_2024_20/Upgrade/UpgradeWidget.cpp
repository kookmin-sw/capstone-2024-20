#include "UpgradeWidget.h"

#include "Components/Button.h"

void UUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CloseButton->OnClicked.AddDynamic(this, &UUpgradeWidget::OnClickedCloseButton);
}

void UUpgradeWidget::SetVisibilityWithBool(const bool IsActive)
{
	if(IsActive == true)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUpgradeWidget::OnClickedCloseButton()
{
	SetVisibilityWithBool(false);
}
