// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "RoomListElement.generated.h"

class URoomPasswordInputPopupWidget;
class UImage;
/**
 * 
 */
UCLASS()
class CAPSTONE_2024_20_API URoomListElement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* RoomName;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta=(BindWidget))
	UImage* LockImage;
	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	class URoomListElementData* RoomListElementData;
	URoomPasswordInputPopupWidget* RPIWidget;
public:
	UFUNCTION()
	void OnClickJoin();
	UFUNCTION()
	void OnClickPasswordJoin();
};
