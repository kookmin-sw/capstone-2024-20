#include "PopupDead.h"

#include "capstone_2024_20/MyCharacter.h"
#include "Components/TextBlock.h"

void UPopupDead::SetTextByReviveCooldown(const float ReviveCooldown) const
{
	UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TXT_Info")));
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("사망하였습니다.\n%d초 뒤 부활합니다."), FMath::CeilToInt(ReviveCooldown))));
}
