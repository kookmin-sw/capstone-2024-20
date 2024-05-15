#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidgetElement.generated.h"


class UButton;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE(FOnClickUpgradeDelegate);

UCLASS()
class CAPSTONE_2024_20_API UUpgradeWidgetElement : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	int32 MaxLevel;
	UPROPERTY()
	int32 CurrentLevel;

	FOnClickUpgradeDelegate OnClickUpgradeDelegate;

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* ExplainText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* UpgradeButton;

	UFUNCTION()
	void Upgrade();

	UFUNCTION()
	template <typename UserClass, typename... VarTypes, typename... ParamTypes>
	FORCEINLINE void RegisterUpgradeDelegate(UserClass* InUserObject, typename TMemFunPtrType<false, UserClass, void (ParamTypes..., std::decay_t<VarTypes>...)>::Type InFunc, VarTypes&&... Vars)
	{
		OnClickUpgradeDelegate.AddUObject(InUserObject, InFunc);
	}
};



