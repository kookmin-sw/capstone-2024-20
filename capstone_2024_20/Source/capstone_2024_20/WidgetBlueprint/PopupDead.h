﻿#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupDead.generated.h"

class AMyCharacter;

UCLASS()
class CAPSTONE_2024_20_API UPopupDead : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTextByReviveCooldown(float ReviveCooldown) const;
};
