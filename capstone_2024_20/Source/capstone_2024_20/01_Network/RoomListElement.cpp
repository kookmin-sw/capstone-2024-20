// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomListElement.h"

#include "NetworkService.h"
#include "RoomData.h"
#include "RoomListElementData.h"
#include "RoomPasswordInputPopupWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void URoomListElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	URoomListElementData* Data = Cast<URoomListElementData>(ListItemObject);
	RoomListElementData = Data;
	RoomName->SetText(RoomListElementData->RoomText);
	JoinButton->OnClicked.AddDynamic(this, &URoomListElement::OnClickJoin);

	RPIWidget = RoomListElementData->RoomPasswordInputPopupWidget;
}

void URoomListElement::OnClickJoin()
{
	bool IsPrivate;
	RoomListElementData->Result.Session.SessionSettings.Get(RoomTEXT::ISPRIVATE, IsPrivate);

	if(IsPrivate == true)
	{
		RPIWidget->SetActive(true);
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
