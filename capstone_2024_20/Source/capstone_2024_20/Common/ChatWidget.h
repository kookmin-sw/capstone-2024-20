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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChatLogBox> NotifyChatLogBoxClass;

	UPROPERTY()
	TArray<UChatLogBox*> ChatLogs;

	UPROPERTY()
	FTimerHandle ChatTimerHandle;
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnCommittedEditable(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void EnableChat();

	UFUNCTION()
	void AddChatLog(EChatType ChatType, const FString& Title,  const FString& NewDetail);

private:
	UFUNCTION()
	void OnKeyEnter();

	UFUNCTION()
	void OnClickSendButton();

	UFUNCTION()
	UChatLogBox* CreateChatLogBox(EChatType ChatType, const FString& Title, const FString& Detail);
};
