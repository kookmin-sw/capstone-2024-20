#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"


class UButton;
class UChatLogBox;
class UScrollBox;
class UEditableText;

UENUM()
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
	UButton* SendButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChatLogBox> NormalChatLogBoxClass;

	UPROPERTY()
	TArray<UChatLogBox*> ChatLogs;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION()
	void OnCommittedEditable(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void EnableChat();

	UFUNCTION()
	void AddChatLog(EChatType ChatType, FString& NewDetail);

private:
	UFUNCTION()
	void OnKeyEnter();

	UFUNCTION()
	void OnClickSendButton();
};
