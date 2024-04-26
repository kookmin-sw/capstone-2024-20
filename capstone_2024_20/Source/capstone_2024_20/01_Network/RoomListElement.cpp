// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomListElement.h"

#include "NetworkService.h"
#include "RoomData.h"
#include "RoomListElementData.h"
#include "RoomPasswordInputPopupWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URoomListElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	URoomListElementData* Data = Cast<URoomListElementData>(ListItemObject);
	RoomListElementData = Data;
	RoomName->SetText(RoomListElementData->RoomText);
	JoinButton->OnClicked.AddDynamic(this, &URoomListElement::OnClickJoin);

	RPIWidget = RoomListElementData->RoomPasswordInputPopupWidget;

	RoomListElementData->Result.Session.SessionSettings.Get(RoomTEXT::ISPRIVATE, IsPrivate);
	if(IsPrivate == false)
	{
		LockImage->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void URoomListElement::OnClickJoin()
{
	if(IsPrivate == true)
	{
		RPIWidget->SetActive(true);
		const FString NewRoomName = RoomName->GetText().ToString() + RPIWidget->RoomNameSuffix;
		RPIWidget->RoomName->SetText(FText::FromString(NewRoomName));
		RPIWidget->PasswordTextBox->SetText(FText::FromString(""));
		RPIWidget->JoinButton->OnClicked.Clear();
		RPIWidget->JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnClickPasswordJoin);
	}
	else
	{
		RoomListElementData->NetworkService->JoinRoomGameSession(RoomListElementData->Result);
	}
}

void URoomListElement::OnClickPasswordJoin()
{
	FString Password;
	RoomListElementData->Result.Session.SessionSettings.Get(RoomTEXT::PASSWORD, Password);

	
	bool IsCorrect = RPIWidget->PasswordTextBox->GetText().ToString().Equals(Password);
	if(IsCorrect)
	{
		RoomListElementData->NetworkService->JoinRoomGameSession(RoomListElementData->Result);
	}
	else
	{
		RPIWidget->SetActive(false);
	}
}
