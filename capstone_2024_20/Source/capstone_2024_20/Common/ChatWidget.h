#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"


class UChatLogBox;
class UScrollBox;
class UEditableText;

enum EChatType
{
	Normal,
	Notify,
};

UCLASS()
class CAPSTONE_2024_20_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UEditableText* EditableText;

	UPROPERTY(meta=(BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta=(BindWidget))
	TSubclassOf<UChatLogBox> NormalChatLogBoxClass;
	
	UPROPERTY()
	TArray<UChatLogBox*> ChatLogs;

	virtual void NativePreConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION()
	void EnableChat();

	UFUNCTION()
	void AddChatLog(EChatType ChatType, FString& NewDetail);

private:
	void OnKeyEnter();
};
